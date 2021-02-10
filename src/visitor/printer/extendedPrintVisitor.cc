//
// Created by jack on 06/04/2020.
//

#include "extendedPrintVisitor.hh"
#include "ast/ast.hh"

void ExtendedPrintVisitor::visit(ProgramNode& program){
    std::cout << "[";
    for (auto it = program.getClasses().begin(); it != program.getClasses().end(); it++){
        // we do not display the class generated by default
        if((*it)->getName() != "Object"){
            (*it)->accept(*this);
            if (it != --program.getClasses().end())
                std::cout << ", " << std::endl << " ";
        }
    }
    std::cout << "]";
}

void ExtendedPrintVisitor::visit(IfExpr& expr) {
    PrintVisitor::visit(expr);
    std::cout << " : " << expr.getType();
}

void ExtendedPrintVisitor::visit(WhileExpr& expr) {
    PrintVisitor::visit(expr);
    std::cout << " : " << expr.getType();
}

void ExtendedPrintVisitor::visit(LetExpr& expr) {
    PrintVisitor::visit(expr);
    std::cout << " : " << expr.getType();
}

void ExtendedPrintVisitor::visit(AssignExpr& expr) {
    PrintVisitor::visit(expr);
    std::cout << " : " << expr.getType();
}

void ExtendedPrintVisitor::visit(UnOpExpr& expr) {
    PrintVisitor::visit(expr);
    std::cout << " : " << expr.getType();
}

void ExtendedPrintVisitor::visit(BinOpExpr& expr) {
    PrintVisitor::visit(expr);
    std::cout << " : " << expr.getType();
}

void ExtendedPrintVisitor::visit(CallExpr& expr) {
    PrintVisitor::visit(expr);
    std::cout << " : " << expr.getType();
}

void ExtendedPrintVisitor::visit(NewExpr& expr) {
    PrintVisitor::visit(expr);
    std::cout << " : " << expr.getType();
}

void ExtendedPrintVisitor::visit(BlockExpr& expr) {
    PrintVisitor::visit(expr);
    if(expr.getExpr().size() > 1)
        std::cout << " : " << expr.getType();
}

void ExtendedPrintVisitor::visit(ValueExpr& expr) {
    PrintVisitor::visit(expr);
    std::cout << " : " << expr.getType();
}