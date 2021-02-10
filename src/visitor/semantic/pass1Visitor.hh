//
// Created by jack on 02/04/2020.
//

#ifndef PART_2_PASS1VISITOR_HH
#define PART_2_PASS1VISITOR_HH

#include "visitor/visitor.hh"
#include "semanticVisitor.hh"
#include "pass2Visitor.hh"

/***************** CHECK VISITOR CLASS ***********************/
/**
 * This class is in charge to check the classes and the inheritance constraints ( as seen in the VSOP manual)
 * This class check also that every Type used in the code exists and is either a primitive, either a class.
 */

class ClassVisitor : public SemanticVisitor {
public:
    ClassVisitor();
    ~ClassVisitor() = default;

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

private:
    FieldVisitor pass2;
};


#endif //PART_2_PASS1VISITOR_HH
