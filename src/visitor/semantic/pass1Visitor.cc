//
// Created by jack on 02/04/2020.
//

#include "pass1Visitor.hh"
#include "visitor/visitor.hh"
#include "ast/ast.hh"
#include "pass2Visitor.hh"

/***************** CHECK VISITOR CLASS ***********************/

ClassVisitor::ClassVisitor(){
    symbol_table.addType("int32", "int32", NULL);
    symbol_table.addType("bool", "bool", NULL);
    symbol_table.addType("string", "string", NULL);
    symbol_table.addType("unit", "unit", NULL);

    sem_error = false;

    pass2 = FieldVisitor();
}

void ClassVisitor::visit(ProgramNode& program) {
    program.initObjectClass();
    program.initMaps();
    bool exit = false; // Indicates if the method should exit or continue checking semantics
    const std::map<std::string, std::list<ClassNode *>> &nameToClass = (program.getNameToClasses());

    //Check if some classes are redefined.
    for (const auto& m: nameToClass){
        if (m.second.size() > 1 && m.second.front()->getName() != "Object"){
            yy::location loc;
            std::string msg = "The class " + m.first + " is declared multiple times:";
            for (auto cl = m.second.begin(); cl != m.second.end(); ++cl){
                if(cl == m.second.begin())
                    loc = (*cl)->location;
                else
                    msg += "\n   " + *(*cl)->location.begin.filename + ":" + std::to_string((*cl)->location.begin.line) + ":" + std::to_string((*cl)->location.begin.column);
            }

            error(loc, msg);

        } else if (m.second.size() > 1 && m.second.front()->getName() == "Object") {
            yy::location loc;
            std::string msg = "The class Object must not be defined by the user.";
            for (auto cl = m.second.begin(); cl != m.second.end(); ++cl){
                // The first one is the Object defined by the system.
                if(cl != m.second.begin()){
                    loc = (*cl)->location;
                    error(loc, msg);
                    exit = true;
                }
            }
        }
    }

    // Check that every parent exists
    for (const auto& m: nameToClass){
        for (const auto& cl: m.second){
            if (cl->getName() != "Object" && nameToClass.find(cl->getParentName()) == nameToClass.end()){
                error((yy::location &) cl->location, "The class " + cl->getName() + " extends the class " + cl->getParentName() + " which does not exist.");
                exit = true;
            }
        }
    }

    // Check that the main Class exists
    if (nameToClass.find("Main") == nameToClass.end()){
        error((const yy::location &) program.location.end, "The class Main has not been declared.");

    } else {
        bool mainFound = false;
        for (auto& method: nameToClass.find("Main")->second.front()->getMethods() ){

            if (method->getName() == "main") {
                mainFound = true;
                if(method->getType() != ValueExpr::INT32)
                    error(method->location, "The main function should return an " + ValueExpr::INT32);
                if(!method->getFormals().empty())
                    error(method->getFormals().front()->location, "The main function should have no formals. ");
            }
        }

        if(! mainFound){
            error((const yy::location &) nameToClass.find("Main")->second.front()->location, "The class Main does not define any 'main' method.");
        }
    }

    if (exit)
        return;

    // Update every class such that it contains pointer to its children classes
    for (const auto& m: nameToClass){
        ClassNode* child = m.second.front();
        // The Object class is the only one without parent
        if (child->getName() != "Object"){
            ClassNode* parent = nameToClass.find(child->getParentName())->second.front();

            child->setParent(parent);
            parent->addChild(child);
        }
    }

    // Check there is no cycle in the class' inheritance.
    ClassNode* object = nameToClass.find("Object")->second.front();
    object->visitChildren(program.getVisitedClasses());

    //All the classes should be visited since the inheritance struct is a tree whose root is the Object class
    std::list<std::list<ClassNode*>> cycles;
    for (const auto& m: program.getVisitedClasses()){
        bool visited = m.second;
        if ( !visited ){
            std::list<ClassNode*> cycle;

            ClassNode* unvisited = nameToClass.find(m.first)->second.front();
            unvisited->visitChildren(program.getVisitedClasses(), cycle);

            cycles.push_back(cycle);
        }
    }

    std::string msg;
    // Detect cycles
    for (const auto& cycle: cycles){
        msg = "Inheritance cycle detected";
        yy::location loc;
        for (const auto& cl: cycle){
            loc = cl->location;
            msg += "\n   " + *loc.begin.filename + ":" + std::to_string(loc.begin.line) + ":" + std::to_string(loc.begin.column) + ":" + cl->getName();
        }
        loc = cycle.front()->location;
        error(loc, msg);
        exit = true;
    }

    if (exit)
        return;

    // Add the classes in the types recognized by the program
    for (const auto& m: nameToClass){
        for (const auto& cl: m.second){
            if(! symbol_table.addType(cl->getName(), cl->getName(), (ClassNode *) &cl)) {
                std::cerr << "Internal error. Several types with the same name." << std::endl;
                exit = true;
            }
        }
    }

    if (exit)
        return;

    // Launch the check part of each class
    object->accept(*this);

    // If no error, launch pass2
    if(!sem_error){
        program.accept(pass2);
        sem_error = pass2.getError();
    }
}

void ClassVisitor::visit(ClassNode& cl) {
    symbol_table.pushScope();

    for (FieldNode *field: cl.getFields()){
        std::list<Variable> variables = symbol_table.getVariable(field->getName());

        // check if a field has already been  declared
        if(!variables.empty()) {
            // get the position of the variable declared firstly
            FieldNode* previous_decl = (FieldNode*) std::get<3>(variables.front());
            yy::location previous_declaration = previous_decl->location;
            error(field->location, "The field " + field->getName() + " is already defined here: " + *previous_declaration.begin.filename + ":" + std::to_string(previous_declaration.begin.line) + ":" + std::to_string(previous_declaration.begin.column) );
        }
        // Check there is no field called 'self'.
        else if (field->getName() == "self"){
            error(field->location, "It is forbidden to declare a field as '" + field->getName() + "'");
        }
        else {
            symbol_table.addVariable(field->getName(), field->getName(), field->getType(), field);
        }
    }

    for (MethodNode* method: cl.getMethods()){
        // Set the class as parent of the methodF
        method->setParent(&cl);
        std::list<Method> methods = symbol_table.getMethod(method->getName(), true);

        // check if a method has already been declared within this class
        if (!methods.empty()){
            // get the position of the method declared firstly
            MethodNode* previous_decl = (MethodNode*) std::get<2>(methods.back());
            yy::location previous_declaration = previous_decl->location;
            error(method->location, "The method " + method->getName() + " is already defined in the class: " + *previous_declaration.begin.filename + ":" + std::to_string(previous_declaration.begin.line) + ":" + std::to_string(previous_declaration.begin.column) );
        } else {
            // Check the methods declared above in the hierarchy
            methods = symbol_table.getMethod(method->getName());
            // check if a method has already been  declared
            if(!methods.empty()) {
                // get the position of the method declared firstly
                MethodNode* previous_decl = (MethodNode*) std::get<2>(methods.back());
                // Check that the child redefinitoion is conform to the parent method
                if (! method->samePrototype(*previous_decl)) {
                    yy::location previous_declaration = previous_decl->location;
                    error(method->location, "The method " + method->getName() + " is already defined here and does not have the same prototype: " + *previous_declaration.begin.filename + ":" + std::to_string(previous_declaration.begin.line) + ":" + std::to_string(previous_declaration.begin.column) );
                } else {
                    symbol_table.addMethod(method->getName(), method->getName(), method);
                }
            } else {
                // No redefinition of this method above in the hierarchy
                symbol_table.addMethod(method->getName(), method->getName(), method);
            }
        }
    }

    //Simple tests to check that types used exist
    for(auto& field: cl.getFields()){
        field->accept(*this);
    }
    for(auto& method: cl.getMethods()){
        method->accept(*this);
    }

    for (const auto & child: cl.getChildren()){
        child->accept(*this);
    }

    symbol_table.exitScope();
}

void ClassVisitor::visit(FieldNode& field) {
    if(field.getExpr() != NULL)
        field.getExpr()->accept(*this);
    if(symbol_table.getType(field.getType()) == NULL){
        error(field.location, "Unknown type: " + field.getType());
    }
}

void ClassVisitor::visit(MethodNode& method) {
    for (auto& formal: method.getFormals()){
        formal->accept(*this);
    }
    method.getBlock()->accept(*this);

    if(symbol_table.getType(method.getType()) == NULL){
        error(method.location, "Unknown type: " + method.getType());
    }
}

void ClassVisitor::visit(FormalNode& formal) {
    if(symbol_table.getType(formal.getType()) == NULL){
        error(formal.location, "Unknown type: " + formal.getType());
    }
    if(formal.getName() == "self"){
        error(formal.location, "It is forbidden to call a formal 'self'");
    }
}

void ClassVisitor::visit(IfExpr& expr) {
    expr.getIfExpr()->accept(*this);
    expr.getThenExpr()->accept(*this);
    if(expr.getElseExpr() != NULL)
        expr.getElseExpr()->accept(*this);
}

void ClassVisitor::visit(WhileExpr& expr) {
    expr.getWhileExpr()->accept(*this);
    expr.getDoExpr()->accept(*this);
}

void ClassVisitor::visit(LetExpr& expr) {
    if (expr.getInitExpr())
        expr.getInitExpr()->accept(*this);
    expr.getInExpr()->accept(*this);

    if(symbol_table.getType(expr.getVarType()) == NULL){
        error(expr.location, "Unknown type: " + expr.getVarType());
    }
}

void ClassVisitor::visit(AssignExpr& expr) {
    if(expr.getInitExpr() != NULL)
        expr.getInitExpr()->accept(*this);
}

void ClassVisitor::visit(UnOpExpr& expr) {
    expr.getExpr()->accept(*this);
}

void ClassVisitor::visit(BinOpExpr& expr) {
    expr.getLeftExpr()->accept(*this);
    expr.getRightExpr()->accept(*this);
}

void ClassVisitor::visit(CallExpr& expr) {
    expr.getObject()->accept(*this);
    for(auto & arg: expr.getArgs())
        arg->accept(*this);
}

void ClassVisitor::visit(NewExpr& expr) {
    if(symbol_table.getType(expr.getType()) == NULL){
        error(expr.location, "Unknown type: " + expr.getType());
    }
}

void ClassVisitor::visit(BlockExpr& expr) {
    for(auto& exp: expr.getExpr())
        exp->accept(*this);
}

void ClassVisitor::visit(ValueExpr& expr) {

}