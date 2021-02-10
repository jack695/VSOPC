//
// Created by jack on 22/04/2020.
//

#ifndef PART_2_SIMPLEPRINTVISITOR_HH
#define PART_2_SIMPLEPRINTVISITOR_HH

#include "visitor/visitor.hh"
#include "ast/symbol_table.hh"
#include "parser-scanner/location.hh"

/***************** PRINT VISITOR CLASS ***********************/

/**
 * Visitor that simply dumps the ast in a textual form.
 */
class PrintVisitor : public Visitor {
public:
    ~PrintVisitor() = default;

    void visit(ProgramNode& program) override ;

    void visit(ClassNode& cl) override ;

    void visit(FieldNode& field) override ;

    void visit(MethodNode& method) override ;

    void visit(FormalNode& formal) override ;

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


#endif //PART_2_SIMPLEPRINTVISITOR_HH
