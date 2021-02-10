//
// Created by jack on 02/04/2020.
//

#ifndef PART_2_PASS2VISITOR_HH
#define PART_2_PASS2VISITOR_HH


#include "visitor/visitor.hh"
#include "semanticVisitor.hh"
#include "pass3Visitor.hh"

/***************** CHECK VISITOR CLASS ***********************/
/**
 * This class is in charge to check the classes and the inheritance constraints ( as seen in the VSOP manual)
 */

class FieldVisitor : public SemanticVisitor {
public:
    FieldVisitor();
    ~FieldVisitor() = default;

    void visit(ProgramNode& program) override ;

    void visit(ClassNode& cl) override ;

    void visit(FieldNode& field) override ;

    void visit(MethodNode& method) override ;

    void visit(FormalNode& formal) override ;

private:
    MethodVisitor pass3;
};


#endif //PART_2_PASS2VISITOR_HH
