//
// Created by jack on 06/04/2020.
//

#ifndef PART_2_EXTENDEDPRINTVISITOR_HH
#define PART_2_EXTENDEDPRINTVISITOR_HH


#include "visitor/visitor.hh"
#include "simplePrintVisitor.hh"

/**
 * Visitor that simply dumps the ast in a textual form. It differs from the simple printer by the fact it also prints
 * the type of each expression.
 */
class ExtendedPrintVisitor : public PrintVisitor {
public:
    ~ExtendedPrintVisitor() = default;

    void visit(ProgramNode& program) override ;

    void visit(IfExpr& expr) override ;

    void visit(WhileExpr& expr) override ;

    void visit(LetExpr& expr) override ;

    void visit(AssignExpr& expr) override ;

    void visit(UnOpExpr& expr) override ;

    void visit(BinOpExpr& expr) override ;

    void visit(CallExpr& expr) override ;

    void visit(NewExpr& expr) override ;

    void visit(BlockExpr& expr) override ;

    void visit(ValueExpr& expr) override ;
};


#endif //PART_2_EXTENDEDPRINTVISITOR_HH
