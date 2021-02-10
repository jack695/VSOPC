//
// Created by jack on 09/05/2020.
//

#include <assert.h>
#include <sstream>
#include "Builder.hh"
#include "runtime.h"
#include <string>
using namespace std;

Builder::Builder() {
    // Add basic types
    symbol_table.addType("int32", "int32", NULL);
    symbol_table.addType("bool", "bool", NULL);
    symbol_table.addType("string", "string", NULL);
    symbol_table.addType("unit", "unit", NULL);
    str_cnt = 0;
}

void Builder::IR(std::ostream& output){
    output << runtime << "\n" << declarations.str() << "\n" << file.str();
}

void Builder::compile(string exe) {
    string ir = "llvm-as " + exe + ".ll";
    system(&ir[0]); // compile ir to bitcode
    string bitcode = "llc -filetype=obj " + exe + ".bc";
    system(&bitcode[0]);
    string exec = "clang " + exe + ".o -o " + exe;
    system(&exec[0]);
    string rm = "rm " + exe + ".ll";
    //system(&rm[0]);
    rm = "rm " + exe + ".bc";
    system(&rm[0]);
    rm = "rm " + exe + ".o";
    system(&rm[0]);
}

Builder::~Builder() {

}

void Builder::initTypes(ClassNode &node) {
    if(node.getName() == "Object")
        return;

    ClassNode* c = &node;
    symbol_table.addType(node.getName(), node.getName(), c);

    // Declare type
    file << "%" + node.getName() << " = type { %" << node.getName() << "_vtable_type*";
    for(auto& field: node.getAllFields()){
        file << ", " << nameToType(field->getType());
    }
    file << " }" << endl;

    // Dtm the prototypes
    vector<string> prototypes;
    for(auto method : node.getAllMethods()){
        prototypes.push_back(prototype(*method));
    }

    // Declare vtable type
    string vtable_type = "%" + node.getName() + "_vtable_type";
    file << vtable_type << " = type { ";
    for(unsigned int i=0; i<prototypes.size(); ++i){
        file << prototypes[i];
        if(i != prototypes.size()-1)
            file << ", ";
    }
    file << " }" << endl;

    // Declare the vtable
    file << "@" << node.getName() << "_vtable = global " << vtable_type << " { ";
    auto methods = node.getAllMethods();
    assert(methods.size() == prototypes.size());
    for(unsigned int i=0; i<prototypes.size(); ++i){
        file << prototypes[i] << " @" << methods[i]->getName();
        if(i != prototypes.size()-1)
            file << ", ";
    }
    file << " }" << endl;
}

std::string Builder::instrName(std::string name){
    std::string instrName;

    if(name.empty()){
        instrName = "%" + to_string(instr_cnt);
        instr_cnt++;
    } else {
        instrName = "%" + name;
        // Check if there is already a var with this name
        auto it = varNames.find(name);
        if(it == varNames.end())
            varNames[name] = 1;
        else {
            unsigned cnt = it->second;
            instrName += std::to_string(cnt) ;
            it->second++;
        }
    }

    return instrName;
}

string Builder::createAlloca(std::string type, std::string name) {
    std::string varName = instrName(name);
    file << indent_lvl << varName << " = alloca " + type << std::endl;
    return varName;
}

string Builder::createBitCast(string var, string type1, string type2, string name){
    std::string varName = instrName(name);
    file << indent_lvl << varName << " = bitcast " << type1 << " " << var << " to " + type2 << std::endl;
    return varName;
}

string Builder::createCall(string var, string type, std::vector<string> args, std::vector<string> args_types, string name){
    assert(args.size() == args_types.size());
    std::string varName;

    if(type != "void"){
        varName = instrName(name);
        file << indent_lvl << varName << " = call " << type << " " << var << "(";
    } else {
        file << indent_lvl << "call " << type << " " << var << "(";
    }

    for(unsigned i = 0; i < args.size(); ++i){
        file << args_types[i] << " " << args[i];
        if(i != args.size() -1)
            file << ", ";
    }

    file << ")" << std::endl;
    return varName;
}

void Builder::createRet(std::string var, std::string type) {
    if(type == "void")
        file << indent_lvl << "ret " << type << std::endl ;
    else
        file << indent_lvl << "ret " << type << " " << var << std::endl ;
    endFunction();
}

std::string Builder::createGEP(std::string var, std::string type, std::string ptrType, std::vector<unsigned int> idx, std::string name) {
    std::string varName = instrName(name);
    file << indent_lvl << varName << " = getelementptr " << type << ", " << ptrType << " " << var;
    for(unsigned int i=0; i < idx.size(); ++i){
        file << ", i32 " << idx[i];
    }
    file << std::endl;

    return varName;
}

void Builder::createStore(std::string src, std::string dst, std::string type){
    file << indent_lvl << "store " << type << " " << src << ", " << type << "* " << dst << endl;
}

std::string Builder::defaultValue(std::string type) {

    if(type == ValueExpr::STRING){
        std::string ptrEmptyStr = instrName("");
        file << indent_lvl << ptrEmptyStr << " = getelementptr [ 1 x i8 ], [ 1 x i8 ]* @.empty, i32 0, i32 0" << endl;
        return ptrEmptyStr;
    }  else if (type == ValueExpr::INT32) {
        return "0";
    } else if (type == ValueExpr::BOOL) {
        return "false";
    } else if (type == ValueExpr::UNIT) {
        return "void";
    } else {
        return "null";
    }
}

void Builder::beginFunction(MethodNode& cl){
    beginFunction(cl.getName(), nameToType(cl.getType()), cl.getFormals());
}

void Builder::beginFunction(std::string methodName, std::string returnType, std::list<FormalNode*> args) {
    file << "define " << returnType << " @" << methodName << "(";
    for(auto formal= args.begin(); formal != args.end(); ++formal){
        file << nameToType((*formal)->getType()) << " %" << (*formal)->getName();
        if(formal != --args.end())
            file << ", ";
    }
    file << ") {" << std::endl;
    file << "entry: " << std::endl;
    blocks.push_back("%entry");

    instr_cnt = 0;
    indent_lvl = "  ";
}

std::string Builder::prototype(MethodNode& method){
    string prototype = nameToType(method.getType()) + " (";
    for(auto field = method.getFormals().begin(); field != method.getFormals().end(); ++field){
        prototype += nameToType((*field)->getType());
        if(field != --method.getFormals().end())
            prototype += ", ";
    }
    prototype += ")*";
    return prototype;
}

void Builder::endFunction() {
    file << "}" << std::endl << std::endl;
    indent_lvl = "";
    instr_cnt = 0;
    varNames.clear();
    labels.clear();
    blocks.clear();
}

std::string Builder::createLoad(std::string var, std::string type, std::string ptrType, std::string name) {
    std::string varName = instrName(name);
    file << indent_lvl << varName << " = load " << type << ", " << ptrType << " " << var << std::endl;

    return varName;
}

string Builder::createBinOp(std::string op, std::string type, std::string left, std::string right, std::string name) {
    std::string instr;
    if(op == "="){
        instr = "icmp eq";
    } else if (op == "+" || op == "-" || op == "*" || op == "/" || op == "^") {
        switch (op[0]) {
            case '+':
                instr = "add";
                break;
            case '-':
                instr = "sub";
                break;
            case '*':
                instr = "mul";
                break;
            case '/':
                instr = "sdiv";
                break;
            case '^':
                // Call a function predefined in runtime
                string val = createCall("@exp_int", "i32", {left, right}, {"i32", "i32"});
                return val;
        }
    } else if (op == "<=" || op == "<") {
        if(op == "<=") {
            instr = "icmp sle";
        } else {
            instr = "icmp slt";
        }
    } else if (op == "and"){
        instr = "and";
    } else {
        std::cerr << "Internal error: Unknown operator." + op;
        exit(5);
    }

    std::string varName = instrName(name);
    file << indent_lvl << varName << " = " << instr << " " << type << " " << left << ", " << right << std::endl;
    return varName;
}

string
Builder::createUnOp(std::string op, std::string type, std::string var, std::string name) {
    std::string varName = instrName(name);
    std::string instr;

    if(op == "-") {
        instr = "mul";
        file << indent_lvl << varName << " = " << instr << " " << type << " -1, " << var << std::endl;
        return varName;
    } else if (op == "isnull") {
        instr = "icmp eq";
        file << indent_lvl << varName << " = " << instr << " " << type << " " << var << ", " << "null" << std::endl;
        return varName;
    } else if (op == "not") {
        instr = "xor";
        file << indent_lvl << varName << " = " << instr << " " << type << " " << var << ", " << "1" << std::endl;
        return varName;
    } else {
        std::cerr << "Internal error: Unknown operator." + op;
        exit(5);
    }
}

void Builder::newBlock(std::string block) {
    // Remove the % char
    file << endl << block.substr(1) << ":" << endl;
}

std::string Builder::getLabel(std::string attempt) {
    auto it = labels.find(attempt);

    if(it == labels.end())
        labels[attempt] = 1;
    else {
        unsigned cnt = it->second;
        attempt += std::to_string(cnt) ;
        it->second++;
    }

    return "%" + attempt;
}

void Builder::createCondBr(std::string val, std::string label1, std::string label2) {
    file << indent_lvl << "br i1 " << val << ", label " << label1 << ", label " << label2 << endl;
}

void Builder::createBr(std::string label) {
    file << indent_lvl << "br label " << label << endl;
}

string Builder::createPhi(std::string type, std::vector<string> vars, std::vector<string> branches, std::string name) {
    std::string varName = instrName(name);

    file << indent_lvl << varName << " = phi  " << type << " ";
    for(unsigned int i=0; i < vars.size(); ++i){
        file << "[ " << vars[i] << ", " << branches[i] << " ]";
        if(i != vars.size() - 1)
            file << ", ";
    }
    file << endl;

    return varName;
}

string Builder::createPtrToInt(std::string var, std::string type, std::string name, std::string intType) {
    std::string varName = instrName(name);
    file << indent_lvl << varName << " = ptrtoint " << type << " " << var << " to " << intType << endl;
    return varName;
}

std::string Builder::createGlobalStr(std::string val, unsigned size, std::string name) {
    // number of char + terminator
    string type = "[ " + std::to_string(size + 1) + " x i8 ]";
    string strName;

    auto it = globalStrings.find(val);
    if(it == globalStrings.end()){
        strName = "@." + std::to_string(str_cnt);
        globalStrings[val] = strName;

        string formatted = val;
        formatted.insert(0, "c");
        formatted.insert(formatted.size()-1, "\\00");
        // declare global var at the beginning
        declarations << strName << " = internal constant " << type << " " << formatted << endl;

        str_cnt++;
    } else {
        strName = it->second;
    }

    std::string varName = instrName(name);
    file << indent_lvl << varName << " = getelementptr " << type << ", " << type << "* " << strName << ", i32 0, i32 0" << endl;

    return varName;
}

std::string Builder::popBlock(){
    string s = blocks.back();
    blocks.pop_back();
    return s;
}

void Builder::pushBlock(std::string block){
    blocks.push_back(block);
}

std::string Builder::nameToVtableType(std::string name) {
    return "%" + name + "_vtable_type";
}

std::string Builder::nameToType(std::string name, bool pointer){
    if(name == ValueExpr::INT32) {
        return "i32";
    } else if (name == ValueExpr::BOOL) {
        return "i1";
    } else if (name == ValueExpr::STRING) {
        return "i8*";
    } else if (name == ValueExpr::UNIT) {
        return "void";
    } else {
        if(pointer)
            return "%" + name + "*";
        else
            return  "%" + name;
    }
}
