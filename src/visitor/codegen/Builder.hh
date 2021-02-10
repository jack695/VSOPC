//
// Created by jack on 09/05/2020.
//

#ifndef PART_2_BUILDER_HH
#define PART_2_BUILDER_HH


#include <string>
#include <fstream>
#include <ast/ast.hh>
#include <ast/symbol_table.hh>
#include <sstream>

class Builder {
public:
    Builder();
    ~Builder();

    /**
     * Dump the LLVM IR onto output.
     * @param output The output stream where the IR is dumped.
     */
    void IR(std::ostream& output);
    /**
     * Compile an executable.
     * @param exe Name of the executable
     */
    static void compile(std::string exe);
    /**
     * Declare the type, the vtable type and the vtable for the class.
     * @param node Class to parse.
     */
    void initTypes(ClassNode &node);

    /**
     *
     * @param type LLVM type of the allocation.
     * @param name ssa name to use to store the result. If not provided, a numbered ssa will be used.
     * @return The ssa containing the result
     */
    std::string createAlloca(std::string type, std::string name = "");
    /**
     *
     * @param var ssa value to cast.
     * @param type1 Original LLVM type
     * @param type2 Casting LLVM type
     * @param name ssa name to use to store the result. If not provided, a numbered ssa will be used.
     * @return The ssa containing the result
     */
    std::string createBitCast(std::string var, std::string type1, std::string type2, std::string name = "");
    /**
     *
     * @param var ssa value or global value to call. (i.e. %12 or @function )
     * @param type LLVM return type of the method.
     * @param args List of the ssa values to pass as args to the methods.
     * @param args_types List of the llvm args types
     * @param name ssa name to use to store the result. If not provided, a numbered ssa will be used.
     * @return The ssa containing the result
     */
    std::string createCall(std::string var, std::string type, std::vector<std::string> args, std::vector<std::string> args_types, std::string name = "");
    /**
     *
     * @param var pointer to the structure to access.
     * @param type Type of the structure
     * @param ptrType Type of the pointer to the structure.
     * @param idx The list of indexes to use. All theses indexes are implicitly supposed to be of type i32.
     * @param name ssa name to use to store the result. If not provided, a numbered ssa will be used.
     * @return The ssa containing the result
     */
    std::string createGEP(std::string var, std::string type, std::string ptrType, std::vector<unsigned int> idx, std::string name = "");
    /**
     *
     * @param var pointer to dereference.
     * @param type Type of the value pointed.
     * @param ptrType Type of the pointer
     * @param name ssa name to use to store the result. If not provided, a numbered ssa will be used.
     * @return The ssa containing the result
     */
    std::string createLoad(std::string var, std::string type, std::string ptrType, std::string name = "");
    /**
     *
     * @param op VSOP binary operator.
     * @param type LLVM type of the operands.
     * @param left ssa value of the left operand.
     * @param right ssa value of the right operand.
     * @param name ssa name to use to store the result. If not provided, a numbered ssa will be used.
     * @return The ssa containing the result
     */
    std::string createBinOp(std::string op, std::string type, std::string left, std::string right, std::string name="");
    /**
     *
     * @param op VSOP unary operator.
     * @param type LLVM type of the operand.
     * @param var ssa value of the operand
     * @param name ssa name to use to store the result. If not provided, a numbered ssa will be used.
     * @return The ssa containing the result
     */
    std::string createUnOp(std::string op, std::string type, std::string var, std::string name = "");
    /**
     *
     * @param var ssa value pointer to convert to int.
     * @param type Type of the pointer
     * @param name ssa name to use to store the result. If not provided, a numbered ssa will be used.
     * @param intType Type of int used for the conversion.
     * @return The ssa containing the result
     */
    std::string createPtrToInt(std::string var, std::string type, std::string name="", std::string intType = "i64");
    /**
     *
     * @param type Type of the merged values
     * @param vars List of ssa values to merge.
     * @param branches List of the labels where the ssa values come from.
     * @param name ssa name to use to store the result. If not provided, a numbered ssa will be used.
     * @return The ssa containing the result
     */
    std::string createPhi(std::string type, std::vector<std::string> vars, std::vector<std::string> branches, std::string name="");

    /**
     *
     * @param val ssa value to use for the test.
     * @param label1 Label to jump if condition evaluated to 1.
     * @param label2 Label to jump if condition evaluated to 0.
     */
    void createCondBr(std::string val, std::string label1, std::string label2);
    /**
     *
     * @param label Label to jump.
     */
    void createBr(std::string label);
    /**
     *
     * @param src ssa value to store
     * @param dst ssa value in which to store
     * @param type type of the ssa value to store
     */
    void createStore(std::string src, std::string dst, std::string type);
    /**
     *
     * @param var ssa value to return
     * @param type type of the ssa value
     */
    void createRet(std::string var, std::string type);

    /**
     * Define a method in the llvm format and open the body. Use it before creating the body of a function
     * @param method Method to declare
     */
    void beginFunction(MethodNode& method);
    /**
     *
     * @param methodName Name of the method to define
     * @param returnType LLVM return type of the method
     * @param args List of arguments of the method.
     */
    void beginFunction(std::string methodName, std::string returnType, std::list<FormalNode*> args);
    /**
     * Ends the definition of a function. Basically, just closes the body of the function just defined.
     * Should be called exactly once after each beginFunction call.
     * It also resets the ssa values counters, the labels names, ...
     */
    void endFunction();
    /**
     * Declare a new block. Collisions names can be avoided by using the getLabel function.
     * It was not possible to handle the collisions in this method like in the other LLVM instructions,
     * because the 'if' needs to know the labels for the branches before to create these blocks.
     * @param block Name of the label to create ( in the form %label).
     */
    void newBlock(std::string block);
    /**
     * Push the block on the stack.
     * @param block label of the block
     */
    void pushBlock(std::string block);
    /**
     * Get the latest label pushed.
     * @return the latest label
     */
    std::string popBlock();

    /**
     *
     * @param name Original type name as described in VSOP language.
     * @param pointer If set to true, will return a pointer to the object type. Has no effect on primitive types.
     * @return The LLVM type corresponding to the VSOP type.
     */
    static std::string nameToType(std::string name, bool pointer = true);
    /**
     *
     * @param type VSOP object type
     * @return The LLVM vtable type of the object.
     */
    static std::string nameToVtableType(std::string type);
    /**
     *
     * @param method The method to build the prototype
     * @return The LLVM prototype of the method.
     */
    static std::string prototype(MethodNode &method);
    /**
     * Take a label name attempt and returns a valid LLVM label.
     * E.g. getLabel("myLabel") will return %myLabel1 if there is one label named %myLabel in the method.
     * @param attempt Name of the label to create
     * @return The LLVM format of the label such there is no collision.
     */
    std::string getLabel(std::string attempt);
    /**
     *
     * @param val The string to allocate. All the char should LLVM valid. E.g. "\n" or "\x0a" is not valid while "\\0A" is.
     * @param size Number of char composing the string. E.g. "\\0A" size is 1.
     * @param name Wish name for the global string. Collisions are handled internally by the function.
     * @return The ssa value containing the pointer to this global string.
     */
    std::string createGlobalStr(std::string val, unsigned size, std::string name="");
    /**
     * Warning: this method uses the VSOP types. It assigns 0 for int32, false for bool, void for unit, "" for string
     * and null for the object types.
     * @param type VSOP type to initialize by default
     * @return The default value for the type.
     */
    std::string defaultValue(std::string type);

private:
    std::string instrName(std::string name);
    std::stringstream file;
    std::stringstream declarations;
    unsigned int instr_cnt;
    unsigned int str_cnt;
    std::string indent_lvl;
    SymbolTable symbol_table;
    std::map<std::string, unsigned> labels;
    std::map<std::string, unsigned> varNames;
    std::map<std::string, std::string> globalStrings;
    std::vector<std::string> blocks;
};



#endif //PART_2_BUILDER_HH
