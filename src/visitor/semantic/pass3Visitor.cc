//
// Created by jack on 05/04/2020.
//

#include "pass3Visitor.hh"
#include "ast/ast.hh"
#include "visitor/printer/extendedPrintVisitor.hh"
#include <stdlib.h>

/***************** CHECK VISITOR CLASS ***********************/

MethodVisitor::MethodVisitor(){
    symbol_table.addType("int32", "int32", NULL);
    symbol_table.addType("bool", "bool", NULL);
    symbol_table.addType("string", "string", NULL);
    symbol_table.addType("unit", "unit", NULL);

    sem_error = false;
}

void MethodVisitor::visit(ProgramNode& program) {
    const std::map<std::string, std::list<ClassNode *>> &nameToClass = (program.getNameToClasses());

    // The classes are globally accessible
    for (auto& cl: program.getClasses()){
        symbol_table.addType(cl->getName(), cl->getName(), cl);
    }

    symbol_table.pushScope();
    // The methods are globally accessible
    for (auto& cl: program.getClasses()){
        for(auto& method: cl->getMethods()){
            symbol_table.addMethod(method->getName(), method->getName(), method);
        }
    }

    ClassNode* object = nameToClass.find("Object")->second.front();
    object->accept(*this);

    symbol_table.exitScope();
}

void MethodVisitor::visit(ClassNode& cl) {
    symbol_table.pushScope();

    symbol_table.addVariable("self", "self", cl.getName(), &cl);
    // Add all the fields in the scope
    for (FieldNode *field: cl.getFields()){
        symbol_table.addVariable(field->getName(), field->getName(), field->getType(), field);
    }

    // The pre defined methods of the class Object have no body -> visiting them will make an error
    if(cl.getName() != "Object"){
        for(auto& method: cl.getMethods()){
            method->accept(*this);
        }
    }

    for (auto& child: cl.getChildren()){
        child->accept(*this);
    }

    symbol_table.exitScope();
}

void MethodVisitor::visit(FieldNode& field) {

}

void MethodVisitor::visit(MethodNode& method) {
    symbol_table.pushScope();

    // Check the types of the formals
    for (const auto& formal: method.getFormals()){
        formal->accept(*this);
    }

    method.getBlock()->accept(*this);

    if(! isA(method.getBlock()->getType(), method.getType())){
        if(method.getBlock()->getType() != ValueExpr::UNKNOWN)
            error(method.location, "The declared type " + method.getType() + " and the type of the block do not correspond " + method.getBlock()->getType());
    }

    symbol_table.exitScope();
}

void MethodVisitor::visit(FormalNode& formal) {
    if(! symbol_table.addVariable(formal.getName(), formal.getName(), formal.getType(), &formal)){
        Variable& var = symbol_table.getVariable(formal.getName(), true).front();
        error(formal.location, "A formal with the same name has already been declared." );
    }

}