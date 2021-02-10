//
// Created by jack on 02/04/2020.
//

#ifndef PART_2_SEMANTICVISITOR_HH
#define PART_2_SEMANTICVISITOR_HH

#include "ast/symbol_table.hh"
#include "visitor/visitor.hh"

/**
 * Basic virtual class for every semantic visitor.
 */
class SemanticVisitor : public Visitor {
public:
    SemanticVisitor();
    virtual ~SemanticVisitor() = default;

    virtual void visit(ProgramNode& program) = 0 ;

    virtual void visit(ClassNode& cl) = 0 ;

    virtual void visit(FieldNode& field)  = 0  ;

    virtual void visit(MethodNode& method)  = 0  ;

    virtual void visit(FormalNode& formal)  = 0  ;

    virtual void visit(IfExpr& expr) ;

    virtual void visit(WhileExpr& expr) ;

    virtual void visit(LetExpr& expr) ;

    virtual void visit(AssignExpr& expr) ;

    virtual void visit(UnOpExpr& expr) ;

    virtual void visit(BinOpExpr& expr) ;

    virtual void visit(CallExpr& expr) ;

    virtual void visit(NewExpr& expr) ;

    virtual void visit(BlockExpr& expr) ;

    virtual void visit(ValueExpr& expr) ;

    void error(const yy::location& l, const std::string m);

    bool getError();

protected:
    /**
     *
     * @param type The type of the var.
     * @return True if this type is a primitive type.
     */
    bool isPrimitive(std::string type);
    /**
     * Check if a type inherits from another one.
     * @return True if the first type inherits from the second one.
     */
    bool isA(std::string type1, std::string type2);
    /**
     * Returns the first common ancestor bewteen two types. Returns NULL if there is no common ancestor
     * (Note: should not happen in valid VSOP program, since all classes inherit from the object method)
     */
    ClassNode* commonAncestor(ClassNode* type1, ClassNode* type2);

    SymbolTable symbol_table;
    bool sem_error;
};


#endif //PART_2_SEMANTICVISITOR_HH
