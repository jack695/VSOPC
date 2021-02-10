# ifndef AST_HH
# define AST_HH

#include <iostream>
#include <list>
#include <map>
#include <vector>
#include "parser-scanner/location.hh"
#include "visitor/visitor.hh"

/***************** VIRTUAL CLASSES ***********************/

/**
 * Basic virtual class for a node in the ast.
 */
class Node {
public:
    /**
     *
     * @param loc location in the source file of the node
     */
     Node(yy::location loc);
     /**
      * Accept a visitor.
      * @param v Visitor to accept.
      */
     virtual void accept(Visitor& v) = 0;
     virtual ~Node()= default;
     /**
      * Location of the node in the source file.
      */
     const yy::location location;
     /**
      *
      * @return The value associated to the node.
      */
     std::string getVal();
     /**
      * Set the value of this node.
      * @param val Value to set.
      */
     void setVal(std::string val);

private:
    std::string value;
};

/**
 * Basic virtual class for a expression node in the ast.
 */
class ExprNode : public Node {
public:
     ExprNode(yy::location loc);
     void accept(Visitor& v) override = 0;
     /**
      *
      * @return The type of the expression.
      */
     std::string getType();
     /**
      * Set the type of this expression.
      * @param type Type to set.
      */
     void setType(std::string type);

protected:
    std::string type; // Each expression is typed.
};

/***************** EXPR CLASSES ***********************/

class IfExpr : public ExprNode {
public:
    /**
     *
     * @param if_expr Expression of the condition.
     * @param then_expr Expression of the then branch.
     * @param else_expr Expression of the else branch.
     * @param loc Location in the source file of this expression.
     */
    IfExpr(ExprNode* if_expr, ExprNode* then_expr, ExprNode* else_expr, yy::location loc);
    ~IfExpr() override;
    /**
     *
     * @return The expression of the condition.
     */
    ExprNode* getIfExpr();
    /**
     *
     * @return The expression of the then branch.
     */
    ExprNode* getThenExpr();
    /**
     *
     * @return The expression of the else branch.
     */
    ExprNode* getElseExpr();
    void accept(Visitor& v) override {
        v.visit(*this);
    }
    
private:
    ExprNode* if_expr;
    ExprNode* then_expr;
    ExprNode* else_expr;
};

class WhileExpr : public ExprNode {
public:
    /**
     *
     * @param while_expr Expression of the loop condition
     * @param do_expr Expression of the loop body.
     * @param loc Location of the expression in the source file.
     */
    WhileExpr(ExprNode* while_expr, ExprNode* do_expr, yy::location loc);
    ~WhileExpr() override;
    /**
     *
     * @return The expression of the loop condition.
     */
    ExprNode* getWhileExpr();
    /**
     *
     * @return The expression of the loop body.
     */
    ExprNode* getDoExpr();
    void accept(Visitor& v) override {
        v.visit(*this);
    }
    
private:
    ExprNode* while_expr;
    ExprNode* do_expr;
        
};

class LetExpr : public ExprNode {
public:
    /**
     *
     * @param name Name of the new variable.
     * @param type Type of the new variable.
     * @param init_expr Expression to initialize the variable.
     * @param in_expr Body of the let expression.
     * @param loc Location in the source file of this expression.
     */
    LetExpr(std::string name, std::string type, ExprNode* init_expr, ExprNode* in_expr, yy::location loc);
    ~LetExpr() override;
    /**
     *
     * @return The name of the new variable.
     */
    std::string getName();
    /**
     *
     * @return The type of the variable.
     */
    std::string getVarType();
    /**
     *
     * @return The expression that initializes the new variable.
     */
    ExprNode* getInitExpr();
    /**
     *
     * @return The body of the let expression.
     */
    ExprNode* getInExpr();
    void accept(Visitor& v) override {
        v.visit(*this);
    }

private:
    std::string name;
    std::string varType;
    ExprNode* init_expr;
    ExprNode* in_expr;
};

class AssignExpr : public ExprNode {
public:
    /**
     *
     * @param name Name of the assigned variable.
     * @param init_expr The expression that initializes the variable.
     * @param loc Location of this expression in the source file.
     */
    AssignExpr(std::string name, ExprNode* init_expr, yy::location loc);
    ~AssignExpr() override;
    /**
     *
     * @return Name of the assigned variable.
     */
    std::string getName();
    /**
     *
     * @return The expression that initializes the variable.
     */
    ExprNode* getInitExpr();
    void accept(Visitor& v) override {
        v.visit(*this);
    }

private:
    std::string name;
    ExprNode* init_expr;
};

class UnOpExpr : public ExprNode {
public:
    /**
     *
     * @param op The unary operator.
     * @param expr The expression of the operand.
     * @param loc The location of this expression in the source file.
     */
    UnOpExpr(std::string op, ExprNode* expr, yy::location loc);
    ~UnOpExpr() override;
    /**
     *
     * @return The unary operator.
     */
    std::string getOp();
    /**
     *
     * @return The expression of the operand.
     */
    ExprNode* getExpr();
    void accept(Visitor& v) override {
        v.visit(*this);
    }

private:
    std::string op;
    ExprNode* expr;
};

class BinOpExpr : public ExprNode {
public:
    /**
     *
     * @param op The binary operator
     * @param left_expr The left operand expression.
     * @param right_expr The right operand expression.
     * @param loc The location of this expression in the source file.
     */
    BinOpExpr(std::string op, ExprNode* left_expr, ExprNode* right_expr, yy::location loc);
    ~BinOpExpr() override;
    /**
     *
     * @return The binary operator
     */
    std::string getOp();
    /**
     *
     * @return The left operand expression.
     */
    ExprNode* getLeftExpr();
    /**
     *
     * @return The right operand expression.
     */
    ExprNode* getRightExpr();
    void accept(Visitor& v) override {
        v.visit(*this);
    }

private:
    std::string op;
    ExprNode* left_expr;
    ExprNode* right_expr;
};

class CallExpr : public ExprNode {
public:
    /**
     *
     * @param object Expression of the caller.
     * @param method Name of the called method.
     * @param args List of the arguments expressions.
     * @param loc Location of the call in the source file.
     */
    CallExpr(ExprNode *object, const std::string &method, const std::list<ExprNode *> &args, yy::location loc);
    ~CallExpr() override;
    /**
     *
     * @return The expression of the caller.
     */
    ExprNode* getObject();
    /**
     *
     * @return The name of the called method.
     */
    const std::string &getMethod() const;
    /**
     *
     * @return The list of the arguments expressions.
     */
    std::list<ExprNode *>& getArgs();
    void accept(Visitor& v) override {
        v.visit(*this);
    }

private:
    ExprNode* object;
    std::string method;
    std::list<ExprNode *> args;
};

class NewExpr : public ExprNode {
public:
    /**
     *
     * @param type The type of the object to create.
     * @param loc Location of this new in the source file.
     */
    NewExpr(std::string type, yy::location loc);
    ~NewExpr() override;
    void accept(Visitor& v) override {
        v.visit(*this);
    }
};

class BlockExpr : public ExprNode {
public:
    ~BlockExpr() override;
    /**
     *
     * @param expr The list of expressions that composes this block.
     * @param loc The position of the block in the source file.
     */
    BlockExpr(std::list<ExprNode*>& expr, yy::location loc);
    /**
     *
     * @return The list of expressions that composes this block.
     */
    std::list<ExprNode*>& getExpr();
    void accept(Visitor& v) override {
        v.visit(*this);
    }

private:
    std::list<ExprNode*> expr;
};

class ValueExpr : public ExprNode {
public:
    /**
     *
     * @param value Value of the expression. Either a constant or an object identifier.
     * @param type Type of the value.
     * @param loc Location of this value in the source file.
     * @param cst Set to true if the expression is a constant. False if it is an object identifier.
     */
    ValueExpr(std::string value, std::string type, yy::location loc, bool cst = true);
    ~ValueExpr() override;
    /**
     *
     * @return The value of the expression
     */
    std::string getValue();
    void accept(Visitor& v) override {
        v.visit(*this);
    }

    /**
     * Type for an object identifier
     */
    static const std::string UNKNOWN;
    static const std::string BOOL;
    static const std::string INT32;
    static const std::string STRING;
    static const std::string UNIT;

    /**
     * Attribute set if the value is a constant. False if it is an object identifier.
     */
    const bool CONSTANT;

private:
    std::string value;
};

/***************** NODE CLASSES ***********************/

class FormalNode : public Node {
public:
    void accept(Visitor& v) override {
        v.visit(*this);
    }
    ~FormalNode() override;
    /**
     *
     * @param name Name of the argument.
     * @param type Type of the argument.
     * @param loc Location of this argument in the source file.
     */
    FormalNode(std::string name, std::string type, yy::location loc);
    /**
     *
     * @return The name of the argument.
     */
    std::string getName();
    /**
     *
     * @return The type of the argument.
     */
    std::string getType();

private:
    std::string name;
    std::string type;

};

class FieldNode : public Node {
public:
    void accept(Visitor& v) override {
        v.visit(*this);
    }
    ~FieldNode() override;
    FieldNode(std::string name, std::string type, yy::location loc);
    /**
     *
     * @param name Name of the field.
     * @param type Type of the field.
     * @param expr Expression to initialize the field.
     * @param loc Location of the field in the source file.
     */
    FieldNode(std::string name, std::string type, ExprNode* expr, yy::location loc);
    /**
     *
     * @return The type of the name.
     */
    std::string getName();
    /**
     *
     * @return The type of the field.
     */
    std::string getType();
    /**
     *
     * @return The expression of the initializer.
     */
    ExprNode* getExpr();
    /**
     *
     * @param i Set an index for this field. This is used for the alignment and the inheritance of classes.
     */
    void setIndex(unsigned int i);
    /**
     *
     * @return The index associated to this field.
     */
    unsigned int getIndex();

private:
    std::string name;
    std::string type;
    ExprNode* expr;
    unsigned int index;
};

class MethodNode : public Node {
public:
    void accept(Visitor& v) override {
        v.visit(*this);
    }
    ~MethodNode() override;
    /**
     *
     * @param name The name of the method.
     * @param type The type of the method.
     * @param block The body of the method.
     * @param formals The arguments of the method.
     * @param loc Location of this method in the source file.
     */
    MethodNode(std::string name, std::string type, BlockExpr* block, std::list<FormalNode*>& formals, yy::location loc);
    /**
     *
     * @return The name of the method.
     */
    std::string getName();
    /**
     *
     * @return The arguments of the method.
     */
    std::list<FormalNode*>& getFormals();
    /**
     *
     * @return The type of the method.
     */
    std::string getType();
    /**
     *
     * @return The body of the method.
     */
    BlockExpr* getBlock();
    /**
     *
     * @return The class in which this method is defined.
     */
    ClassNode* getParent();
    /**
     * Set the class in which this method is defined.
     */
    void setParent(ClassNode* );
    /**
     * Change the name of the method.
     * @param name The new name of the method.
     */
    void setName(std::string name);
    /**
     * Set an index for this method. This is used for the alignment and the inheritance of classes.
     * @param index
     */
    void setIndex(unsigned int index);
    /**
     *
     * @return The index set to this method.
     */
    unsigned int getIndex();
    /**
     * Check if two methods have the same prototype.
     * They do not need to be defined in the same class or to have the same arguments name.
     * @param method The other method to compare.
     * @return True if the methods have the same prototype.
     */
    bool samePrototype(MethodNode &method);

private:
    std::string name;
    std::list<FormalNode*> formals;
    std::string type;
    BlockExpr* block;
    ClassNode* parent;
    unsigned int index;
};

class ClassNode : public Node {
public:
    void accept(Visitor& v) override {
        v.visit(*this);
    }
    ~ClassNode() override;
    /**
     *
     * @param name Name of the class.
     * @param fields Fields of the class.
     * @param methods The methods of the class.
     * @param l Location of the class in the source file.
     * @param parentName The name of the parent class.
     */
    ClassNode(std::string name, std::list<FieldNode*>& fields, std::list<MethodNode*>& methods, yy::location l, std::string parentName);
    /**
     *
     * @return  Name of the class.
     */
    std::string getName();
    /**
     *
     * @return  The name of the parent class.
     */
    std::string getParentName();
    /**
     *
     * @return  Fields of the class.
     */
    std::list<FieldNode*>& getFields();
    /**
     *
     * @return The methods of the class.
     */
    std::list<MethodNode*>& getMethods();
    /**
     *
     * @return The class of the parent.
     */
    ClassNode *getParent() const;
    /**
     * Set the parent class.
     * @param parent The parent class.
     */
    void setParent(ClassNode *parent);
    /**
     * Returns all the classes that inherits from this class.
     * @return
     */
    const std::list<ClassNode *> &getChildren() const;
    /**
     * Add a class as one of the child.
     * @param child Class that inherits from this class.
     */
    void addChild(ClassNode* child);
    /**
     * Visits the class and the children. The visited classes will be set to true.
     * A class and its children whose name is already set as visited in the map won't be visited to avoid infinite loop.
     * @param visitedClasses The map of the classes and their status (true for already visited).
     */
    void visitChildren(std::map<std::string, bool>& visitedClasses);
    /**
     * Check if there is a cycle and determines the loop if so.
     * @param visitedClasses The map of the classes and their status (true for already visited).
     * @param names Should be empty. If a cycle is detected. The names of the classes forming this loop will be added
     * in this list.
     * @return True if a cycle has been detected.
     */
    bool visitChildren(std::map<std::string, bool>& visitedClasses, std::list<ClassNode*>& names);
    /**
     * Computes the set of ancestors classes (i.e. the classes from which this class inherits). Note that this class is
     * considered as a parent of itself.
     * @return The list of the ancestors from the youngest to the oldest (i.e.: [self, parent, grandparent, ...].
     */
    std::list<ClassNode*> getAncestors();
    /**
     * Check if the class or one of its ancestors has a method defined.
     * @param method The method to search.
     * @return The method if found. NULL otherwise.
     */
    MethodNode* hasMethod(std::string method);
    /**
     *
     * @return The set of all methods that a class owns. The inherited and the explicitly defined.
     */
    std::vector<MethodNode*> getAllMethods();
    /**
     *
     * @return The set of all fields that a class owns. The inherited and the explicitly defined.
     */
    std::vector<FieldNode*> getAllFields();

private:
    /**
     * Determines the set of all methods that a class owns. The inherited and the explicitly defined.
     */
    void inheritMethods();
    /**
     * Determines the set of all fields that a class owns. The inherited and the explicitly defined.
     */
    void inheritFields();

    std::string name;
    std::string parentName;
    std::list<FieldNode*> fields;
    std::list<MethodNode*> methods;
    ClassNode* parent;
    std::list<ClassNode*> children;
    std::vector<MethodNode*> allMethods;
    std::vector<FieldNode*> allFields;
};

class ProgramNode : public Node {
public:
    /**
     *
     * @param classes The classes that compose the program.
     * @param loc The location of the programm in the source file.
     */
    ProgramNode(std::list<ClassNode*>& classes, yy::location loc);
    ~ProgramNode() override;
    void accept(Visitor& v) override {
        v.visit(*this);
    }

    // Getter
    std::list<ClassNode*>& getClasses();
    /**
     *  Initialize the mapping of the object.
     *  The first one is a mapping between the name of the classes and a pointer to them. If several classes have
     *  the same name, several pointers are returned.
     *  The second one is a mapping between the name of the class and the status ( visited or not).
     */
    void initMaps();
    /**
     * Intialize the class Object and its methods. These methods body are empty since the runtime provide the
     * implementation and the fields of the class.
     */
    void initObjectClass();
    /**
     *
     * @return A map between the names and the classes objects.
     */
    const std::map<std::string, std::list<ClassNode *>>& getNameToClasses() const;
    /**
     *
     * @return A map between the classes and their status (true: visited, false: not visited).
     */
    std::map<std::string, bool>& getVisitedClasses();

private:
    std::list<ClassNode*> classes;
    std::map<std::string, std::list<ClassNode*>> nameToClasses;
    std::map<std::string, bool> visitedClasses;
};

#endif // ! AST_HH