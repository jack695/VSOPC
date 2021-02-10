//
// Created by jack on 22/04/2020.
//

#include "ast/ast.hh"
#include "llvmGen.hh"
#include <fstream>

using namespace std;

LLVMCodeGen::LLVMCodeGen(bool compile, std::string srcFile) : builder() {
    this->compile = compile;
    this->srcFile = srcFile;
}

void LLVMCodeGen::visit(ProgramNode& program){
    // Inherit all the methods of each class before renaming it. If not, a bug occurs since 2 same methods will have
    // different name after the renaming.
    for(auto& cl:program.getClasses()){
        cl->getAllMethods();
    }
    //Rename the methods to avoid collision due to inheritance and put self in each method
    renameMethods(&program);
    // Index the fields
    ClassNode* object;
    for(auto& cl: program.getClasses()){
        if(cl->getName() == "Object"){
            object = cl;
        }
    }
    indexMethods(*object);
    indexFields(*object);

    // Add the types
    for(auto& cl: program.getClasses()){
        symbol_table.addType(cl->getName(), cl->getName(), cl);
    }

    for(auto& cl: program.getClasses()){
        cl->accept(*this);
    }

    fstream output;
    if(compile){
        std::string delimiter = ".vsop";
        std::string outputFile = srcFile.substr(0, srcFile.find(delimiter)) + ".ll";
        output.open (outputFile, std::fstream::in | std::fstream::out | std::fstream::trunc );
        builder.IR(output);
        builder.compile(srcFile.substr(0, srcFile.find(delimiter)));
    } else {
        builder.IR(cout);
    }

    output.close();
}

void LLVMCodeGen::visit(ClassNode& cl) {

    if(cl.getName() == "Object")
        return;

    // Create the type, the vtable and the vtable type
    builder.initTypes(cl);
    // Create the new and init methods for each class
    defineNewMethod(cl);
    defineInitMethod(cl);

    for(auto& method: cl.getMethods()){
        method->accept(*this);
    }
}

void LLVMCodeGen::visit(FieldNode& field){
    string val = "";
    if(field.getExpr()){
        field.getExpr()->accept(*this);
        val = field.getExpr()->getVal();
    }

    field.setVal(val);
}

void LLVMCodeGen::visit(MethodNode& method){
    builder.beginFunction(method);

    // Push first the fields in an outer scope
    symbol_table.pushScope();
    auto formal = method.getFormals().begin();
    // The first formal is always self
    for(auto& field: method.getParent()->getAllFields()){
        string elt_ptr = builder.createGEP("%" + (*formal)->getName(), builder.nameToType(method.getParent()->getName(), false), builder.nameToType(method.getParent()->getName()), {0, field->getIndex()}, field->getName()+"_ptr");
        // we also need to keep track of the pointer in case of assignement
        symbol_table.addVariable(field->getName(), elt_ptr, field->getType(), NULL);
    }
    // push self in scope
    symbol_table.addVariable((*formal)->getName(), "%" + (*formal)->getName(), builder.nameToType(method.getParent()->getName()), NULL);
    ++formal; // A bug occured iw try to store self in a new var

    // Record the function arguments in the NamedValues map.
    symbol_table.pushScope();
    for(; formal != method.getFormals().end(); ++formal){
        // alloc and store the formal to support mutable var
        (*formal)->accept(*this);
    }

    method.getBlock()->accept(*this);

    builder.createRet(method.getBlock()->getVal(), builder.nameToType(method.getBlock()->getType()));
    symbol_table.exitScope();
    symbol_table.exitScope();
}

void LLVMCodeGen::visit(FormalNode& formal){
    string type = builder.nameToType(formal.getType());

    // Create an alloca for this variable to support mutable variables
    string alloc = builder.createAlloca(type);
    // Store the initial value into the alloca.
    builder.createStore("%" + formal.getName(), alloc, type);

    symbol_table.addVariable(formal.getName(), alloc, type, NULL);
}

void LLVMCodeGen::visit(IfExpr& expr){
    expr.getIfExpr()->accept(*this);
     string condVal = expr.getIfExpr()->getVal();

    // Convert condition to a bool by comparing equal to 1
    condVal = builder.createBinOp("=", "i1", condVal, "1");

    // Get the labels for the blocks
    string labelThen = builder.getLabel("then");
    string labelElse = builder.getLabel("else");
    string labelCont = builder.getLabel("ifcont");

    builder.popBlock();
    if(expr.getElseExpr())
        builder.createCondBr(condVal, labelThen, labelElse);
    else
        builder.createCondBr(condVal, labelThen, labelCont);

    builder.newBlock(labelThen);
    builder.pushBlock(labelThen);
    expr.getThenExpr()->accept(*this);
    string thenV = expr.getThenExpr()->getVal();
    // Check for necessary cast
    if(expr.getType() != ValueExpr::UNIT && expr.getType() != expr.getThenExpr()->getType()){
        thenV = builder.createBitCast(thenV, builder.nameToType(expr.getThenExpr()->getType()), builder.nameToType(expr.getType()));
    }
    builder.createBr(labelCont);

    string elseV;
    if(expr.getElseExpr()){
        builder.newBlock(labelElse);
        builder.pushBlock(labelElse);
        expr.getElseExpr()->accept(*this);
        elseV = expr.getElseExpr()->getVal();
        // Check for necessary cast
        if(expr.getType() != ValueExpr::UNIT && expr.getType() != expr.getElseExpr()->getType()){
            elseV = builder.createBitCast(elseV, builder.nameToType(expr.getElseExpr()->getType()), builder.nameToType(expr.getType()));
        }
        builder.createBr(labelCont);
    }


    builder.newBlock(labelCont);
    std::vector<string> var, branches;

    string b2;
    if(expr.getElseExpr())
        b2 = builder.popBlock();
    string b1 = builder.popBlock();
    string phiNode;
    if(expr.getType() != ValueExpr::UNIT){
        if(expr.getElseExpr())
            phiNode = builder.createPhi(builder.nameToType(expr.getType()), {thenV, elseV}, {b1, b2});
        else
            phiNode = builder.createPhi(builder.nameToType(expr.getType()), {thenV}, {b1});
        expr.setVal(phiNode);
    } else
        expr.setVal("()");
    builder.pushBlock(labelCont);
}

void LLVMCodeGen::visit(WhileExpr& expr){
    string condLabel = builder.getLabel("cond");
    string endLabel = builder.getLabel("end");
    string doLabel = builder.getLabel("do");
    // Jump to cond block
    builder.createBr(condLabel);

    // Set the code
    builder.newBlock(condLabel);
    expr.getWhileExpr()->accept(*this);
    string condVal = expr.getWhileExpr()->getVal();
    // Convert condition to a bool by comparing equal to 1
    condVal = builder.createBinOp("=", "i1", condVal, "1");

    builder.createCondBr(condVal, doLabel, endLabel);

    // Loop block
    builder.newBlock(doLabel);
    // Set the code
    expr.getDoExpr()->accept(*this);
    string doVal = expr.getDoExpr()->getVal();
    // Go back to the condition
    builder.createBr(condLabel);

    // Emit end loop block.
    builder.newBlock(endLabel);
}

void LLVMCodeGen::visit(LetExpr& expr){
    symbol_table.pushScope();
    string init;
    if(expr.getInitExpr()){
        expr.getInitExpr()->accept(*this);
        init = expr.getInitExpr()->getVal();
    } else {
        init = builder.defaultValue(expr.getVarType());
    }

    // Create an alloca for this variable.
    string alloca = builder.createAlloca(builder.nameToType(expr.getVarType()));

    if(expr.getInitExpr() && expr.getVarType() != expr.getInitExpr()->getType()){
        init = builder.createBitCast(init, builder.nameToType(expr.getInitExpr()->getType()), builder.nameToType(expr.getVarType()));
    }
    builder.createStore(init, alloca, builder.nameToType(expr.getVarType()));


    // Add arguments to variable symbol table.
    symbol_table.addVariable(expr.getName(), alloca, expr.getVarType(), NULL);

    expr.getInExpr()->accept(*this);
    expr.setVal(expr.getInExpr()->getVal());

    symbol_table.exitScope();
}

void LLVMCodeGen::visit(AssignExpr& expr){
    expr.getInitExpr()->accept(*this);
    string val = expr.getInitExpr()->getVal();

    // Look up the name
    // Get the pointer to the variable from the most inner scope
    string ptr = (std::get<1>(symbol_table.getVariable(expr.getName(), false).front()));
    if (ptr.empty())
        std::cerr << "Internal error: Unknown variable: " + expr.getName() << std::endl;

    //Need a cast
    if(expr.getType() != expr.getInitExpr()->getType()){
        val = builder.createBitCast(val, builder.nameToType(expr.getInitExpr()->getType()), builder.nameToType(expr.getType()));
    }
    builder.createStore(val, ptr, builder.nameToType(expr.getType()));


    expr.setVal(val);
}

void LLVMCodeGen::visit(UnOpExpr& expr){
    expr.getExpr()->accept(*this);
    string value = expr.getExpr()->getVal();

    string v = builder.createUnOp(expr.getOp(), builder.nameToType(expr.getExpr()->getType()), value);
    expr.setVal(v);
}

void LLVMCodeGen::visit(BinOpExpr& expr){
    expr.getLeftExpr()->accept(*this);
    string left = expr.getLeftExpr()->getVal();
    string right;
    string type = builder.nameToType(expr.getLeftExpr()->getType());

    // Lazy 'and'
    if(expr.getOp() == "and" && left == "0"){
        right = "0";
    } else {
        expr.getRightExpr()->accept(*this);
        right = expr.getRightExpr()->getVal();
    }

    // Check for the eventual cast
    if(expr.getOp() == "="){
        if(expr.getLeftExpr()->getType() == expr.getLeftExpr()->getType() && expr.getLeftExpr()->getType() == ValueExpr::UNIT){
            left = "1";
            right = "1";
            type = builder.nameToType(ValueExpr::INT32);
        } else if (expr.getLeftExpr()->getType() != expr.getRightExpr()->getType()) {
            left = builder.createBitCast(left, builder.nameToType(expr.getLeftExpr()->getType()), builder.nameToType("Object"));
            right = builder.createBitCast(right, builder.nameToType(expr.getRightExpr()->getType()), builder.nameToType("Object"));
            type = builder.nameToType("Object");
        }

    }

    string v = builder.createBinOp(expr.getOp(), type, left, right);

    expr.setVal(v);
}

void LLVMCodeGen::visit(CallExpr& expr){
    string callerType = expr.getObject()->getType();
    ClassNode* cl = (ClassNode*) std::get<1>(*symbol_table.getType(callerType));
    //Ok this is a bit dirty. The methodName is the original name, so need to format it
    string originalMethodName = expr.getMethod();
    MethodNode* method;
    for(auto& m: cl->getAllMethods()){
        string formattedName = m->getParent()->getName() + "_" + originalMethodName;
        if(m->getName() == formattedName){
            method = m;
        }
    }

    // Check the caller expression
    string caller;
    if(expr.getObject()){
        expr.getObject()->accept(*this);
        caller = expr.getObject()->getVal();
    } else {
        std::cerr << "Internal error. Should not happen 'call expr'" << std::endl;
        exit(5);
    }

    // Dynamic dispatch
    string vtable = builder.createGEP(caller, builder.nameToType(callerType, false), builder.nameToType(callerType), {0, 0}); // Get the pointer to the vtable
    vtable = builder.createLoad(vtable, builder.nameToVtableType(callerType)+"*", builder.nameToVtableType(callerType)+"**"); // Get the vtable and provide a pointer to the vtable
    string callee = builder.createGEP(vtable, builder.nameToVtableType(callerType), builder.nameToVtableType(callerType)+"*", {0, method->getIndex()}); // Get the pointer to the pointer to the function (lel)
    callee = builder.createLoad(callee, builder.prototype(*method), builder.prototype(*method)+"*"); // Get the pointer to the function

    // Set the args.
    std::vector<string> args;
    // Check if need for a cast if a parent method is called
    if(method->getParent()->getName() != expr.getObject()->getType()){
        caller = builder.createBitCast(caller, builder.nameToType(expr.getObject()->getType()), builder.nameToType(method->getParent()->getName()));
    }
    args.push_back(caller);
    auto formal = method->getFormals().begin();
    formal++; // skip the first arg, which is self
    for (auto& arg: expr.getArgs()) {
        arg->accept(*this);
        string argVal = arg->getVal();
        // Check if need for a cast
        if( arg->getType() != (*formal)->getType()){
            argVal = builder.createBitCast(argVal, builder.nameToType(arg->getType()), builder.nameToType((*formal)->getType()));
        }
        args.push_back(argVal);

        ++formal;
    }

    std::vector<string> args_types;
    for(auto& formal: method->getFormals()){
        args_types.push_back(builder.nameToType(formal->getType()));
    }

    string val = builder.createCall(callee, builder.nameToType(method->getType()), args, args_types);
    if(method->getType() != ValueExpr::UNIT)
        expr.setVal(val);
}

void LLVMCodeGen::visit(NewExpr& expr){
    string method_name = "@_" + expr.getType() + "_new";
    string value = builder.createCall(method_name, builder.nameToType(expr.getType()), {},  {});
    expr.setVal(value);
}

void LLVMCodeGen::visit(BlockExpr& expr){
    for (auto& exp: expr.getExpr())
        exp->accept(*this);

    expr.setVal(expr.getExpr().back()->getVal());
}

void LLVMCodeGen::visit(ValueExpr& expr){
    if(expr.CONSTANT) {
        if (expr.getType() == ValueExpr::STRING) {
            string str;
            unsigned size = convertEscapeChar(expr.getValue(), &str);
            string var = builder.createGlobalStr(str, size);
            expr.setVal(var);
        } else if (expr.getType() == ValueExpr::BOOL) {
            if(expr.getValue() == "true")
                expr.setVal("1");
            else
                expr.setVal("0");
        } else {
                expr.setVal(expr.getValue());
        }
    } else {
        // Get the variable from the most inner scope
        string ptr = (std::get<1>(symbol_table.getVariable(expr.getValue(), false).front()));
        if (ptr.empty())
            std::cerr << "Internal error: Unknown variable: " + expr.getValue() << std::endl;
        // We store only pointers except for self
        string val;
        if(expr.getValue() != "self")
            val = builder.createLoad(ptr, builder.nameToType(expr.getType()), builder.nameToType(expr.getType())+"*");
        else
            val = ptr;

        expr.setVal(val);
    }
}

/****************************** PRIVATE METHODS *******************************************/

void LLVMCodeGen::renameMethods(ProgramNode* program){
    for(auto& cl: program->getClasses()){
        for(auto& method: cl->getMethods()){
            method->getFormals().push_front(new FormalNode("self", cl->getName(), yy::location()));
            method->setName(  cl->getName() + "_" + method->getName());
        }
    }
}

void LLVMCodeGen::indexFields(ClassNode& cl) {
    // The first index is always for the vtable
    int i = 1;

    // If the class has a parent and this latter has fields, starts the index after the first shared field
    if(cl.getParent() != NULL && !cl.getParent()->getFields().empty()){
        i = cl.getParent()->getFields().back()->getIndex() + 1;
    }

    for(auto& field: cl.getFields()){
        field->setIndex(i);
        i++;
    }

    for(auto& child: cl.getChildren()){
        indexFields(*child);
    }
}

void LLVMCodeGen::indexMethods(ClassNode &cl) {
    // The first index is always for the vtable
    int i = 0;

    // If the class has a parent and this latter has fields, starts the index after the first shared field
    if(cl.getParent() != NULL && !cl.getParent()->getMethods().empty()){
        i = cl.getParent()->getMethods().back()->getIndex() + 1;
    }

    for(auto& method: cl.getMethods()){
        method->setIndex(i);
        i++;
    }

    for(auto& child: cl.getChildren()){
        indexMethods(*child);
    }
}

void LLVMCodeGen::defineNewMethod(ClassNode& cl){
    // The Object has already an implemented function
    if(cl.getName() == "Object"){
        return;
    }

    // Build the return type
    string returnType = builder.nameToType(cl.getName());
    // Starts with _ to avoid collision with user defined methods
    string methodName = "_" + cl.getName() + "_" + "new";

    builder.beginFunction(methodName, returnType, {});

    // Here we want the struct itself
    string type = builder.nameToType(cl.getName(), false);
    string ptrType = builder.nameToType(cl.getName());
    //Alloc the structure
    string sizeAsPtr = builder.createGEP("null", type, ptrType, {1});
    string sizeAsI32 = builder.createPtrToInt(sizeAsPtr, ptrType, "i64");
    string newObj = builder.createCall("@malloc", "i8*", {sizeAsI32}, {"i64"});
    newObj = builder.createBitCast(newObj, "i8*", ptrType);

    string ptrParentType = builder.nameToType(cl.getParentName());

    //Make a cast to parent type
    string cast = builder.createBitCast(newObj, ptrType, ptrParentType);

    // Call the parent initializer
    std::string initName = cl.getParent()->getName() + "_init";
    newObj = builder.createCall("@_" + initName, ptrParentType, {cast}, {ptrParentType});

    //Make a cast to object type
    cast = builder.createBitCast(newObj, ptrParentType, ptrType);

    // Call the child initializer
    initName = cl.getName() + "_init";
    newObj = builder.createCall("@_" + initName, ptrType, {cast}, {ptrType});

    // Finish off the function.
    this->builder.createRet(newObj, ptrType);
}

void LLVMCodeGen::defineInitMethod(ClassNode& cl){
    // The Object has already an implemented function
    if(cl.getName() == "Object"){
        return;
    }

    // Build the return type
    string type = builder.nameToType(cl.getName(), false);
    string ptrType = builder.nameToType(cl.getName());
    string ptrParentType =  builder.nameToType(cl.getParentName());
    // Starts with _ to avoid collision with user defined methods
    string methodName = "_" + cl.getName() + "_init";

    FormalNode* self = new FormalNode("self", cl.getName(), yy::location());
    builder.beginFunction(methodName, ptrType, {self});
    delete self;

    // Store the vtable in the new object
    string vtabletype = "%" + cl.getName() + "_vtable_type*";
    string vtable = builder.createGEP("%self", type, ptrType, {0, 0});
    builder.createStore("@" + cl.getName() + "_vtable", vtable, vtabletype);

    // Initialize the fields
    // Set the fields
    for (auto& node: cl.getFields()){
        string elt = builder.createGEP("%self",type, ptrType, {0, node->getIndex()});
        // check if there is an initializer for this field
        node->accept(*this);
        string val = node->getVal();

        if(val.empty()){
            builder.createStore(builder.defaultValue(node->getType()), elt, builder.nameToType(node->getType()));
        } else {
            builder.createStore(val, elt, builder.nameToType(node->getType()));
        }
    }

    builder.createRet("%self", ptrType);
}

unsigned LLVMCodeGen::convertEscapeChar(std::string str, std::string* formatted) {
    unsigned size = 0;
    for(unsigned i =0; i < str.size(); size++){
        string substr = str.substr(i, 4);
        if (substr == "\\x0a"){
            formatted->append("\\0A");
            i+=4;
        } else if(substr == "\\x08") {
            formatted->append("\\08");
            i+=4;
        } else if(substr == "\\x09") {
            formatted->append("\\09");
            i+=4;
        } else if(substr == "\\x0d") {
            formatted->append("\\0D");
            i+=4;
        } else if(substr == "\\x5c") {
            formatted->append("\\5C");
            i+=4;
        } else if(substr == "\\x22") {
            formatted->append("\\22");
            i+=4;
        } else {
            *formatted += str[i];
            i++;
        }
    }

    // -2 because the \" do not count for the initialization of the global string
    return size-2;
}