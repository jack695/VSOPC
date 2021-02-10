//
// Created by jack on 02/04/2020.
//

#include "pass2Visitor.hh"
#include "ast/ast.hh"
#include "pass3Visitor.hh"
#include <stdlib.h>
/***************** CHECK VISITOR CLASS ***********************/

FieldVisitor::FieldVisitor(){
    symbol_table.addType("int32", "int32", NULL);
    symbol_table.addType("bool", "bool", NULL);
    symbol_table.addType("string", "string", NULL);
    symbol_table.addType("unit", "unit", NULL);

    sem_error = false;

    pass3 = MethodVisitor();
}

void FieldVisitor::visit(ProgramNode& program) {
    const std::map<std::string, std::list<ClassNode *>> &nameToClass = (program.getNameToClasses());

    // The classes are globally accessible
    for (auto& cl: program.getClasses()){
        symbol_table.addType(cl->getName(), cl->getName(), cl);
    }

    ClassNode* object = nameToClass.find("Object")->second.front();
    object->accept(*this);

    if(!sem_error){
        program.accept(pass3);
        sem_error = pass3.getError();
    }
}

void FieldVisitor::visit(ClassNode& cl) {

    for(auto& field: cl.getFields()){
        field->accept(*this);
    }

    for (auto& child: cl.getChildren()){
        child->accept(*this);
    }
}

void FieldVisitor::visit(FieldNode& field) {
    if(field.getName() == "self"){
        error(field.location, "It is forbidden to declare a field as 'self' ");
    }

    if(symbol_table.getType(field.getType()) == NULL)
        error(field.location, "The type of this field is unknown: " + field.getType());
    if (field.getExpr() != NULL){
        field.getExpr()->accept(*this);
        if (! isA(field.getExpr()->getType(), field.getType())){
            // if the expr has type unknown it means that an error has probably occured.
            if (!sem_error && field.getExpr()->getType() == ValueExpr::UNKNOWN)
                error(field.location, "Internal error. Unknown expression type for the field: " + field.getName() + " :" + field.getType());
            else
                error(field.location, "The types of the field and the initializer do not match: " + field.getType() + " " + field.getExpr()->getType());
        }
    }
}

void FieldVisitor::visit(MethodNode& method) {

}

void FieldVisitor::visit(FormalNode& formal) {

}