//
// Created by jack on 15/03/2020.
//

#ifndef PART_2_VISITOR_HH
#define PART_2_VISITOR_HH

#include "parser-scanner/location.hh"

class ProgramNode;
class ClassNode;
class FieldNode;
class MethodNode;
class FormalNode;
class ExprNode;
class IfExpr;
class WhileExpr;
class LetExpr;
class AssignExpr;
class UnOpExpr;
class BinOpExpr;
class CallExpr;
class NewExpr;
class BlockExpr;
class ValueExpr;

class Visitor {
public:
    virtual ~Visitor() = default;
    virtual void visit(ProgramNode& program) = 0;
    virtual void visit(ClassNode& cl) = 0;
    virtual void visit(FieldNode& field) = 0;
    virtual void visit(MethodNode& method) = 0;
    virtual void visit(FormalNode& formal) = 0;
    virtual void visit(IfExpr& expr) = 0;
    virtual void visit(WhileExpr& expr) = 0;
    virtual void visit(LetExpr& expr) = 0;
    virtual void visit(AssignExpr& expr) = 0;
    virtual void visit(UnOpExpr& expr) = 0;
    virtual void visit(BinOpExpr& expr) = 0;
    virtual void visit(CallExpr& expr) = 0;
    virtual void visit(NewExpr& expr) = 0;
    virtual void visit(BlockExpr& expr) = 0;
    virtual void visit(ValueExpr& expr) = 0;
};

#endif //PART_2_VISITOR_HH
