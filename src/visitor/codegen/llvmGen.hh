//
// Created by jack on 22/04/2020.
//

#ifndef PART_2_LLVMGEN_HH
#define PART_2_LLVMGEN_HH

#include <string>
#include <ast/ast.hh>
#include <fstream>
#include <ast/symbol_table.hh>
#include <visitor/semantic/semanticVisitor.hh>
#include "Builder.hh"

/**
 * Class in charge to convert the annotated ast after the semantic step into LLVM IR or code.
 */
class LLVMCodeGen : public Visitor {
public:

    LLVMCodeGen(bool compile, std::string src_file);

    virtual ~LLVMCodeGen() = default;

    virtual void visit(ProgramNode& program) ;

    virtual void visit(ClassNode& cl) ;

    virtual  void visit(FieldNode &field);

    virtual void visit(MethodNode& method) ;

    virtual void visit(FormalNode& formal) ;

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

private:
    // Rename the methods to avoid collisions. All methods will be prefixed by className_
    static void renameMethods(ProgramNode *program);
    // Index the fields of the class and its children. Should be called on the Object class to have correct
    // alignment of memory between classes.
    static void indexFields(ClassNode &node);
    // Index the methods of the class and its children. Should be called on the Object class to have correct
    // alignment of memory between classes.
    static void indexMethods(ClassNode &node);
    // Define the method @_className_new that is in charge to allocate memory to the heap for the class
    void defineNewMethod(ClassNode &node);
    // Define the method @_className_init that is in charge to initialize the fields of this class
    void defineInitMethod(ClassNode &node);
    // Convert the internal string representation to a LLVM accepted format. e.g. "\x0a" to "\\OA"
    static unsigned convertEscapeChar(std::string str, std::string* formatted);

    bool compile;
    Builder builder;
    SymbolTable symbol_table;
    std::string srcFile;
};


#endif //PART_2_LLVMPASS3_HH
