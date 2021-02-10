//
// Created by jack on 15/03/2020.
//

#include <err.h>
#include "simplePrintVisitor.hh"
#include "ast/ast.hh"

/***************** PRINT VISITOR CLASS ***********************/

void PrintVisitor::visit(ProgramNode& program) {
    std::cout << "[";
    for (auto it = program.getClasses().begin(); it != program.getClasses().end(); it++){
        (*it)->accept(*this);
        if (it != --program.getClasses().end())
            std::cout << ", " << std::endl << " ";
    }
    std::cout << "]";
}

void PrintVisitor::visit(ClassNode& cl) {
    std::cout << "Class(" << cl.getName() << ", " << cl.getParentName() << ", " << std::endl << "   [";

    for (auto it = cl.getFields().begin(); it != cl.getFields().end(); it++){
        (*it)->accept(*this);
        if (it != --cl.getFields().end())
            std::cout << ", " << std::endl << "    ";
    }
    std::cout << "], " << std::endl << "   [";
    for (auto it = cl.getMethods().begin(); it != cl.getMethods().end(); it++){
        (*it)->accept(*this);
        if (it != --cl.getMethods().end())
            std::cout << ", " << std::endl << "    ";
    }
    std::cout << "]" << ")";
}

void PrintVisitor::visit(FieldNode& field) {
    std::cout << "Field(" << field.getName() << ", " << field.getType();
    if ( field.getExpr() ) {
        std::cout << ", ";
        field.getExpr()->accept(*this);
    }
    std::cout << ")";
}

void PrintVisitor::visit(MethodNode& method) {
    std::cout << "Method(" << method.getName() << ", [";
    for (auto it = method.getFormals().begin(); it != method.getFormals().end(); it++){
        (*it)->accept(*this);
        if (it != --method.getFormals().end())
            std::cout << ", ";
    }
    std::cout << "], " << method.getType() << ", ";
    method.getBlock()->accept(*this);
    std::cout << ")";
}

void PrintVisitor::visit(FormalNode& formal) {
    std::cout << formal.getName() << ":" << formal.getType();
}

void PrintVisitor::visit(IfExpr& expr) {
    std::cout << "If(";

    expr.getIfExpr()->accept(*this);
    std::cout << ", ";
    expr.getThenExpr()->accept(*this);

    if (expr.getElseExpr()){
        std::cout << ", ";
        expr.getElseExpr()->accept(*this);
    }

    std::cout << ")" << std::endl;
}

void PrintVisitor::visit(WhileExpr& expr) {
    std::cout << "While(";
    expr.getWhileExpr()->accept(*this);
    std::cout << ", ";
    expr.getDoExpr()->accept(*this);
    std::cout << ")" << std::endl;
}

void PrintVisitor::visit(LetExpr& expr) {
    std::cout << "Let(" << expr.getName() << ", " << expr.getVarType() << ", ";
    if(expr.getInitExpr()){
        expr.getInitExpr()->accept(*this);
        std::cout << ", ";
    }
    expr.getInExpr()->accept(*this);


    std::cout << ")" << std::endl;
}

void PrintVisitor::visit(AssignExpr& expr) {
    std::cout << "Assign(" << expr.getName() << ", ";
    expr.getInitExpr()->accept(*this);
    std::cout << ")";
}

void PrintVisitor::visit(UnOpExpr& expr) {
    std::cout << "UnOp(" << expr.getOp() << ", ";
    expr.getExpr()->accept(*this);
    std::cout << ")";
}

void PrintVisitor::visit(BinOpExpr& expr) {
    std::cout << "BinOp(" << expr.getOp() << ", ";
    expr.getLeftExpr()->accept(*this);
    std::cout << ", ";
    expr.getRightExpr()->accept(*this);
    std::cout << ")";
}

void PrintVisitor::visit(CallExpr& expr) {
    std::cout << "Call(";
    expr.getObject()->accept(*this);
    std::cout << ", " << expr.getMethod() << ", [";
    // Display the args
    for (auto it = expr.getArgs().begin(); it != expr.getArgs().end(); it++){
        (*it)->accept(*this);
        if (it != --expr.getArgs().end())
            std::cout << ", ";
    }
    std::cout << "])";
}

void PrintVisitor::visit(NewExpr& expr) {
    std::cout << "New(" << expr.getType() << ")";
}

void PrintVisitor::visit(BlockExpr& expr) {
    if(expr.getExpr().size() > 1)
        std::cout << "[";

    for (auto it = expr.getExpr().begin(); it != expr.getExpr().end(); it++){
        (*it)->accept(*this);
        if (it != --expr.getExpr().end())
            std::cout << ", ";
    }

    if(expr.getExpr().size() > 1)
        std::cout << "]";
}

void PrintVisitor::visit(ValueExpr& expr) {
    if (expr.getValue() == "unit"){
        std::cout << "()";
    }
    else{
        std::cout << expr.getValue();
    }
}
