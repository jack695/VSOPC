//
// Created by jack on 02/04/2020.
//

#include "semanticVisitor.hh"
#include "visitor/visitor.hh"
#include "ast/ast.hh"
#include <math.h>

SemanticVisitor::SemanticVisitor(){
    symbol_table.addType("int32", "int32", NULL);
    symbol_table.addType("bool", "bool", NULL);
    symbol_table.addType("string", "string", NULL);
    symbol_table.addType("unit", "unit", NULL);

    sem_error = false;
}

void SemanticVisitor::visit(IfExpr& expr) {
    ExprNode *ifExpr = expr.getIfExpr();
    ifExpr->accept(*this);
    ExprNode *thenExpr = expr.getThenExpr();
    thenExpr->accept(*this);
    ExprNode *elseExpr = expr.getElseExpr();
    if (expr.getElseExpr() != NULL) {
        expr.getElseExpr()->accept(*this);
    }

    // Check the conditional
    if (expr.getIfExpr()->getType() != ValueExpr::BOOL) {
        error(expr.getIfExpr()->location, "The condition is not a boolean");
    }

    //Check the type are known: No need it is done in the previous pass ( pass 1)
    /*if(symbol_table.getType(thenExpr->getType()) == NULL || (elseExpr != NULL && symbol_table.getType(elseExpr->getType()) == NULL)){
        return ;
    }*/

    if(elseExpr == NULL){
        expr.setType(ValueExpr::UNIT);
    } else {
        if (thenExpr->getType() == ValueExpr::UNIT || elseExpr->getType() == ValueExpr::UNIT) {
            // One is unit
            expr.setType(ValueExpr::UNIT);
        } else if (isPrimitive(thenExpr->getType()) && isPrimitive(elseExpr->getType())) {
            // Both are primitives
            if(thenExpr->getType() == elseExpr->getType()){
                expr.setType(thenExpr->getType());
            } else {
                error(thenExpr->location, "The type of the 'then':" + thenExpr->getType() + " and 'else': " + elseExpr->getType() + " expressions do not match. Same primitive type should be used." );
            }
        } else if (isPrimitive(thenExpr->getType()) || isPrimitive(elseExpr->getType())) {
            // Only one is a primitive
            error(thenExpr->location, "The type of the 'then':" + thenExpr->getType() + " and 'else': " + elseExpr->getType() + " expressions do not match. Same primitive type should be used." );
        } else {
            ClassNode* cl1 = (ClassNode *) std::get<1>(*symbol_table.getType(thenExpr->getType()));
            ClassNode* cl2 = (ClassNode *) std::get<1>(*symbol_table.getType( elseExpr->getType()));
            ClassNode* ancestor = commonAncestor(cl1, cl2);

            if(ancestor != NULL)
                expr.setType(ancestor->getName());
            else
                error(thenExpr->location, "The type of the 'then':" + thenExpr->getType() + " and 'else': " + elseExpr->getType() + " expressions do not match. They do not have any ancestor in common." );
        }
    }
}

void SemanticVisitor::visit(WhileExpr& expr) {
    ExprNode *whileExpr = expr.getWhileExpr();
    whileExpr->accept(*this);
    ExprNode *doExpr = expr.getDoExpr();
    doExpr->accept(*this);

    // Check the conditional
    if (whileExpr->getType() != ValueExpr::BOOL) {
        error(whileExpr->location, "The condition is not a boolean");
    }

    expr.setType(ValueExpr::UNIT);
}

void SemanticVisitor::visit(LetExpr& expr) {
    ExprNode* init = expr.getInitExpr();
    symbol_table.pushScope();

    if(expr.getName() == "self"){
        error(expr.location, "It is forbidden to declare a variable called 'self'.");
    }

    if(init){
        init->accept(*this);

        //Check the consistence between the declared type and the initializer
        if(! isA(init->getType(), expr.getVarType())){
            if(init->getType() != ValueExpr::UNKNOWN)
                error(expr.location, "The variable has type " + expr.getVarType() + " while the initializer is of type: " + expr.getInitExpr()->getType());
            return;
        }
    }

    symbol_table.addVariable(expr.getName(), expr.getName(), expr.getVarType(), &expr);

    // Now that the variable has been pushed into the scope, evaluate the body
    ExprNode* body = expr.getInExpr();
    body->accept(*this);

    expr.setType(body->getType());

    symbol_table.exitScope();
}

void SemanticVisitor::visit(AssignExpr& expr) {
    expr.getInitExpr()->accept(*this);

    if(expr.getName() == "self"){
        error(expr.location, "It is forbidden to assign a value to the 'self' object.");
    }

    std::list<Variable> variables = symbol_table.getVariable(expr.getName());
    if(variables.empty()){
        error(expr.location, "The variable " + expr.getName() + " is not declared.");
    } else {
        // Use the last pushed variable
        Variable var = variables.front();
        std::string type = std::get<2>(var);
        if(! isA(expr.getInitExpr()->getType(), type)){
            if(expr.getInitExpr()->getType() != ValueExpr::UNKNOWN)
                error(expr.location, "The variable has type " + type + " while the initializer is of type: " + expr.getInitExpr()->getType());
        } else {
            expr.setType(type);
            return;
        }
    }

    expr.setType(expr.getInitExpr()->getType());
}

void SemanticVisitor::visit(UnOpExpr& expr) {
    expr.getExpr()->accept(*this);

    if(expr.getOp() == "-") {
        if(expr.getExpr()->getType() == ValueExpr::INT32){
            // If the values are known, already compute the value.
            /*if (expr.getExpr()->getVal() != NULL){
                int exp = *((int *)expr.getExpr()->getVal());
                int* val = new int;
                *val = - exp;
                expr.setVal(val);
            }*/

            expr.setType(ValueExpr::INT32);
        } else {
            error(expr.getExpr()->location, "The operand is not of type " + ValueExpr::INT32);
        }
    } else if (expr.getOp() == "isnull") {
        if (isPrimitive(expr.getExpr()->getType()) || expr.getExpr()->getType() == ValueExpr::UNIT){
            error(expr.getExpr()->location, "The isnull operator cannot be used on primitives types. " + expr.getExpr()->getType());
        } else {
            expr.setType(ValueExpr::BOOL);
        }
    } else if (expr.getOp() == "not") {
        if(expr.getExpr()->getType() == ValueExpr::BOOL){
            // If the values are known, already compute the value.
            /*if (expr.getExpr()->getVal() != NULL){
                bool exp = *((int *)expr.getExpr()->getVal());
                bool* val = new bool;
                *val = ! exp;
                expr.setVal(val);
            }*/

            expr.setType(ValueExpr::BOOL);
        } else {
            error(expr.getExpr()->location, "The operand is not of type " + ValueExpr::BOOL);
        }
    } else {
        error(expr.location, "Internal error: Unknown operator." + expr.getOp());
    }
}

void SemanticVisitor::visit(BinOpExpr& expr) {
    expr.getLeftExpr()->accept(*this);
    expr.getRightExpr()->accept(*this);

    if(expr.getOp() == "="){
        std::string type_left = expr.getLeftExpr()->getType();
        std::string type_right = expr.getRightExpr()->getType();

        // If the left or the right operand is a primitive type
        if(type_left == ValueExpr::INT32 || type_left == ValueExpr::STRING || type_left == ValueExpr::BOOL || type_left == ValueExpr::UNIT || type_right == ValueExpr::INT32 || type_right == ValueExpr::STRING || type_right == ValueExpr::BOOL || type_right == ValueExpr::UNIT) {
            if (type_right != type_left)
                error(expr.getLeftExpr()->location,"The operator '=' is used on incompatible type: " + type_left + " and " + type_right);
            else{
                // TODO try to evaluate the value
                expr.setType(ValueExpr::BOOL);
            }

        } else {
            // None of the operand is a primitive type
            // TODO try to evaluate the value
            expr.setType(ValueExpr::BOOL);
        }

    } else if (expr.getOp() == "+" || expr.getOp() == "-" || expr.getOp() == "*" || expr.getOp() == "/" || expr.getOp() == "^") {
        if(expr.getLeftExpr()->getType() == ValueExpr::INT32 && expr.getRightExpr()->getType() == ValueExpr::INT32){
            // If the values are known, already compute the value.
            /*if (expr.getLeftExpr()->getVal() != NULL && expr.getRightExpr()->getVal() != NULL){
                int l = *((int *)expr.getLeftExpr()->getVal());
                int r = *((int *)expr.getRightExpr()->getVal());
                int* val = new int;

                if (expr.getOp() == "+") {
                    *val = r + l;
                } else if (expr.getOp() == "-") {
                    *val = r - l;
                } else if (expr.getOp() == "*") {
                    *val = r * l;
                } else if (expr.getOp() == "/") {
                    *val = r / l;
                } else if (expr.getOp() == "^") {
                    *val = pow(l, r);
                }

                expr.setVal(val);
            }*/

            expr.setType(ValueExpr::INT32);
        } else if(expr.getLeftExpr()->getType() != ValueExpr::INT32) {
            error(expr.getLeftExpr()->location, "The left operand(" + expr.getLeftExpr()->getType() + ") is not of type " + ValueExpr::INT32);
        } else {
            error(expr.getRightExpr()->location, "The right operand(" + expr.getLeftExpr()->getType() + ") is not of type " + ValueExpr::INT32);
        }
    } else if (expr.getOp() == "<=" || expr.getOp() == "<") {
        if(expr.getLeftExpr()->getType() == ValueExpr::INT32 && expr.getRightExpr()->getType() == ValueExpr::INT32){
            // If the values are already known, initialize the value
            /*if (expr.getLeftExpr()->getVal() != NULL && expr.getLeftExpr()->getVal() != NULL) {
                int l = *((int *) expr.getLeftExpr()->getVal());
                int r = *((int *) expr.getRightExpr()->getVal());
                bool *val = new bool;

                if (expr.getOp() == "<=") {
                    *val = l <= r;
                } else if (expr.getOp() == "<") {
                    *val = l < r;
                }

                expr.setVal(val);
            }*/
            expr.setType(ValueExpr::BOOL);
        } else if (expr.getLeftExpr()->getType() != ValueExpr::INT32) {
            error(expr.getLeftExpr()->location, "The left operand(" + expr.getLeftExpr()->getType() + ") is not of type " + ValueExpr::INT32);
        } else {
            error(expr.getRightExpr()->location, "The right operand(" + expr.getLeftExpr()->getType() + ") is not of type " + ValueExpr::INT32);
        }
    } else if (expr.getOp() == "and"){
        if (expr.getLeftExpr()->getType() == ValueExpr::BOOL && expr.getRightExpr()->getType() == ValueExpr::BOOL) {
            // If the values are already known, initialize the value
            /*if (expr.getLeftExpr()->getVal() != NULL && expr.getRightExpr()->getVal() != NULL){
                int l = *((int *)expr.getLeftExpr()->getVal());
                int r = *((int *)expr.getRightExpr()->getVal());
                bool* val = new bool;
                *val = l && r;
                expr.setVal(val);
            }*/
            expr.setType(ValueExpr::BOOL);
        } else if (expr.getLeftExpr()->getType() != ValueExpr::BOOL) {
            error(expr.getLeftExpr()->location, "The left operand(" + expr.getLeftExpr()->getType() + ") is not of type " + ValueExpr::BOOL);
        } else {
            error(expr.getRightExpr()->location, "The right operand(" + expr.getLeftExpr()->getType() + ") is not of type " + ValueExpr::BOOL);
        }

    } else {
        error(expr.location, "Internal error: Unknown operator." + expr.getOp());
    }
}

void SemanticVisitor::visit(CallExpr& expr) {
    ClassNode* caller;

    //Find the class of the caller
    if(expr.getObject() == NULL){
        caller = (ClassNode*) std::get<3>(symbol_table.getVariable("self").front());
    } else {
        expr.getObject()->accept(*this);

        if(isPrimitive(expr.getObject()->getType()) || expr.getObject()->getType() == "UNIT"){
            error( expr.getObject()->location, "It is forbidden to call a method on an instance that is not an object: " + expr.getObject()->getType());
            return;
        } else if (expr.getObject()->getType() == ValueExpr::UNKNOWN) {
            error(expr.getObject()->location, "Internal error, unknown type of the caller " + expr.getObject()->getType());
            return;
        } else {
            caller = (ClassNode*) std::get<1>(*symbol_table.getType(expr.getObject()->getType()));
        }
    }

    // Check if the class contains a method of this name
    MethodNode* callee = caller->hasMethod(expr.getMethod());

    if(! callee){
        error(expr.location, "The class " + expr.getObject()->getType() + " does not have any method " + expr.getMethod());
        return;
    }

    for(auto& arg: expr.getArgs()){
        arg->accept(*this);
    }

    // Check the consistency between args and formals
    if(expr.getArgs().size() != callee->getFormals().size()){
        error(expr.getArgs().front()->location, "The method " + callee->getName() + " requires " + std::to_string(callee->getFormals().size()) + " arguments while " + std::to_string(expr.getArgs().size()) + " have been provided.");
        return;
    }
    auto arg = expr.getArgs().begin();
    auto formal = callee->getFormals().begin();
    while(arg != expr.getArgs().end()){
        if(! isA((*arg)->getType(), (*formal)->getType())){
            error((*arg)->location, "The type of this arg: " + (*arg)->getType() + " does not correspond to the expected one: " + (*formal)->getType());
        }

        ++arg;
        ++formal;
    }

    expr.setType(callee->getType());
}

void SemanticVisitor::visit(NewExpr& expr) {
    if(symbol_table.getType(expr.getType()) == NULL)
        error(expr.location, "The type of this field is unknown: " + expr.getType());

    // No need to init the type. Already done at the construction of this object
}

void SemanticVisitor::visit(BlockExpr& expr) {
    symbol_table.pushScope();

    for (const auto& ex: expr.getExpr()){
        ex->accept(*this);
    }

    // The type of an expression is the type of the last expression
    expr.setType(expr.getExpr().back()->getType());

    symbol_table.exitScope();
}

void SemanticVisitor::visit(ValueExpr& expr) {
    if (expr.getType() == ValueExpr::UNKNOWN){
        std::list<Variable> variables = symbol_table.getVariable(expr.getValue());

        // Check if the variable has been declared previously and set the type value of the expr
        if(variables.empty()) {
            error(expr.location, "The identifier " + expr.getValue() + " has not been declared.");
        } else {
            // set the type of this expression
            std::string type = std::get<2>(variables.front());
            expr.setType(type);
        }
    } else if (expr.getType().empty() || expr.getType() == ValueExpr::UNKNOWN) {
        error(expr.location, "Internal error: This expression has no known type.");
    } else {
        expr.setType(expr.getType());
        if(expr.getType() == ValueExpr::BOOL) {
            /*bool* val = new bool;
            if (expr.getValue() == "true"){
                *val = true;
            } else {
                *val = false;
            }
            expr.setVal(val);*/
        } else if (expr.getType() == ValueExpr::INT32) {
            /*int* val = new int;
            *val = std::stoi(expr.getValue());
            expr.setVal(val);*/
        } else if (expr.getType() == ValueExpr::STRING) {
            /*std::string* val = new std::string;
            *val = expr.getValue();
            expr.setVal(val);*/
        } else if (expr.getType() == ValueExpr::UNIT) {
            /*std::string* val = new std::string;
            *val = expr.getValue();
            expr.setVal(val);*/
        } else {
            error(expr.location, "Internal error: This expression has no known type.");
        }
    }
}

void SemanticVisitor::error(const yy::location& l, const std::string m) {
    sem_error = true;
    std::cerr << *l.begin.filename << ":" << l.begin.line << ":" << l.begin.column << ": semantic error: " << m << std::endl;
}

bool SemanticVisitor::isPrimitive(std::string type) {
    return (type == ValueExpr::BOOL || type == ValueExpr::INT32 || type == ValueExpr::STRING);
}

bool SemanticVisitor::isA(std::string type1, std::string type2){
    Type* t1 = symbol_table.getType(type1);
    Type* t2 = symbol_table.getType(type2);

    if(t1 == NULL || t2 ==NULL){
        return false;
    }

    if(isPrimitive(type1) || isPrimitive(type2) || type1 == ValueExpr::UNIT || type2 == ValueExpr::UNIT ){
        return type1 == type2;
    } else {
        ClassNode* ancestor = commonAncestor((ClassNode *) std::get<1>(*t1), (ClassNode *) std::get<1>(*t2));
        // The second class should be the first common ancestor with the first one
        return ancestor == (ClassNode *) std::get<1>(*t2);
    }
}

ClassNode* SemanticVisitor::commonAncestor(ClassNode* type1, ClassNode* type2) {
    std::list<ClassNode*> parents_1 = type1->getAncestors();
    std::list<ClassNode*> parents_2 = type2->getAncestors();

    for (auto& p1: parents_1) {
        for (auto& p2: parents_2) {
            if (p1->getName() == p2->getName())
                return p1;
        }
    }
    return NULL;
}

bool SemanticVisitor::getError() {
    return sem_error;
}