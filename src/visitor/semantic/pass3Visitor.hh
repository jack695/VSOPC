//
// Created by jack on 05/04/2020.
//

#ifndef PART_2_PASS3VISITOR_HH
#define PART_2_PASS3VISITOR_HH


#include "visitor/visitor.hh"
#include "semanticVisitor.hh"
#include "visitor/printer/extendedPrintVisitor.hh"

/**
 * This class is in charge to check the type constraints as described in the VSOP manual.
 */
class MethodVisitor : public SemanticVisitor {
public:
    MethodVisitor();
    ~MethodVisitor() = default;

    void visit(ProgramNode& program) override ;

    void visit(ClassNode& cl) override ;

    void visit(FieldNode& field) override ;

    void visit(MethodNode& method) override ;

    void visit(FormalNode& formal) override ;
};


#endif //PART_2_PASS3VISITOR_HH
