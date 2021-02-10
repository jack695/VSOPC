#include <utility>
#include "ast.hh"

Node::Node(yy::location loc): location(loc){

}

std::string Node::getVal(){
    return value;
}

void Node::setVal(std::string val) {
    this->value = val;
}

ExprNode::ExprNode(yy::location loc) : Node(loc) {
    type = ValueExpr::UNKNOWN;
}

std::string ExprNode::getType() {
    return this->type;
}

void ExprNode::setType(std::string type){
    this->type = type;
}

/*************** IfExpr Implementation ***************************/

IfExpr::IfExpr(ExprNode* if_expr, ExprNode* then_expr, ExprNode* else_expr, yy::location loc) : ExprNode(loc){
    this->if_expr = if_expr;
    this->then_expr = then_expr;
    this->else_expr = else_expr;
}   

IfExpr::~IfExpr(){
    delete if_expr;
    delete then_expr;
    delete else_expr;
}

ExprNode* IfExpr::getIfExpr(){
    return this->if_expr;
}

ExprNode* IfExpr::getThenExpr(){
    return this->then_expr;
}

ExprNode* IfExpr::getElseExpr(){
    return this->else_expr;
}


/*************** WhileExpr Implementation ***************************/

WhileExpr::WhileExpr(ExprNode* while_expr, ExprNode* do_expr, yy::location loc) : ExprNode(loc){
    this->while_expr = while_expr;
    this->do_expr = do_expr;
}   

WhileExpr::~WhileExpr(){
    delete while_expr;
    delete do_expr;
}

ExprNode* WhileExpr::getWhileExpr(){
    return this->while_expr;
}

ExprNode* WhileExpr::getDoExpr(){
    return this->do_expr;
}

/*************** LetExpr Implementation ***************************/

LetExpr::LetExpr(std::string name, std::string type, ExprNode* init_expr, ExprNode* in_expr, yy::location loc) : ExprNode(loc){
    this->name = std::move(name);
    this->varType = std::move(type);
    this->init_expr = init_expr;
    this->in_expr = in_expr;
}   

LetExpr::~LetExpr(){
    delete init_expr;
    delete in_expr;
}

std::string LetExpr::getName(){
    return this->name;  
}

std::string LetExpr::getVarType(){
    return this->varType;
}

ExprNode* LetExpr::getInitExpr(){
    return this->init_expr;  
}

ExprNode* LetExpr::getInExpr(){
    return this->in_expr;  
}

/*************** AssignExpr Implementation ***************************/

AssignExpr::AssignExpr(std::string name, ExprNode* init_expr, yy::location loc) : ExprNode(loc){
    this->name = std::move(name);
    this->init_expr = init_expr;
}   

AssignExpr::~AssignExpr(){
    delete init_expr;
}

std::string AssignExpr::getName(){
    return this->name;  
}

ExprNode* AssignExpr::getInitExpr(){
    return this->init_expr;  
}

/*************** UnOpExpr Implementation ***************************/

UnOpExpr::UnOpExpr(std::string op, ExprNode* expr, yy::location loc) : ExprNode(loc){
    this->op = std::move(op);
    this->expr = expr;
}   

UnOpExpr::~UnOpExpr(){
    delete expr;
}

std::string UnOpExpr::getOp(){
    return this->op;  
}

ExprNode* UnOpExpr::getExpr(){
    return this->expr;  
}

/*************** BinOpExpr Implementation ***************************/

BinOpExpr::BinOpExpr(std::string op, ExprNode* left_expr, ExprNode* right_expr, yy::location loc) : ExprNode(loc){
    this->op = std::move(op);
    this->left_expr = left_expr;
    this->right_expr = right_expr;
}   

BinOpExpr::~BinOpExpr(){
    delete left_expr;
    delete right_expr;
}

std::string BinOpExpr::getOp(){
    return this->op;  
}

ExprNode* BinOpExpr::getLeftExpr(){
    return this->left_expr;  
}

ExprNode* BinOpExpr::getRightExpr(){
    return this->right_expr;  
}

/*************** CallExpr Implementation ***************************/

CallExpr::CallExpr(ExprNode *object, const std::string &method, const std::list<ExprNode *> &args, yy::location loc) :
                                                                                                     ExprNode(loc),
                                                                                                     object(object),
                                                                                                     method(method),
                                                                                                     args(args) {}

CallExpr::~CallExpr(){
    delete object;
    for (const auto& args_el: args){
        delete args_el;
    }
}



ExprNode* CallExpr::getObject(){
    return this->object;  
}

const std::string &CallExpr::getMethod() const {
    return method;
}

std::list<ExprNode *> &CallExpr::getArgs() {
    return args;
}



/*************** NewExpr Implementation ***************************/

NewExpr::NewExpr(std::string type, yy::location loc) : ExprNode(loc){
    this->type = std::move(type);
}   

NewExpr::~NewExpr()= default;


/*************** BlockExpr Implementation ***************************/

BlockExpr::BlockExpr(std::list<ExprNode*>& expr, yy::location loc) : ExprNode(loc){
    this->expr = expr;
}   

BlockExpr::~BlockExpr(){
    for (auto e: expr)
        delete e;
}

std::list<ExprNode*>& BlockExpr::getExpr(){
    return this->expr;  
}

/*************** ValueExpr Implementation ***************************/

const std::string ValueExpr::UNKNOWN = "unknown";
const std::string ValueExpr::BOOL = "bool";
const std::string ValueExpr::INT32 = "int32";
const std::string ValueExpr::STRING = "string";
const std::string ValueExpr::UNIT = "unit";

ValueExpr::ValueExpr(std::string value, std::string type, yy::location loc, bool cst) : ExprNode(loc), CONSTANT(cst){
    this->value = std::move(value);
    this->type = type;
}   

ValueExpr::~ValueExpr()= default;

std::string ValueExpr::getValue(){
    return this->value;
}

/*************** FormalNode Implementation ***************************/

FormalNode::FormalNode(std::string name, std::string type, yy::location loc) : Node(loc){
    this->name = std::move(name);
    this->type = std::move(type);
}

FormalNode::~FormalNode()= default;

std::string FormalNode::getName(){
    return this->name;  
}

std::string FormalNode::getType(){
    return this->type;
}

/*************** FieldNode Implementation ***************************/

FieldNode::FieldNode(std::string name, std::string type, yy::location loc) : Node(loc){
    this->name = std::move(name);
    this->type = std::move(type);
    this->expr = NULL;
}

FieldNode::FieldNode(std::string name, std::string type, ExprNode* expr, yy::location loc) : FieldNode(std::move(name), std::move(type), loc){
    this->expr = expr;
}

FieldNode::~FieldNode(){
    delete expr;
}

std::string FieldNode::getName(){
    return this->name;  
}

std::string FieldNode::getType(){
    return this->type;  
}

ExprNode* FieldNode::getExpr(){
    return this->expr;  
}

void FieldNode::setIndex(unsigned int i){
    this->index = i;
}

unsigned int FieldNode::getIndex(){
    return this->index;
}

/*************** MethodNode Implementation ***************************/

MethodNode::MethodNode(std::string name, std::string type, BlockExpr* block, std::list<FormalNode*>& formals, yy::location loc) : Node(loc) {
    this->name = std::move(name);
    this->type = std::move(type);
    this->block = block;
    this->formals = formals;
}

MethodNode::~MethodNode(){
    for (auto& formal: formals)
        delete formal;
    delete block;
}

std::list<FormalNode*>& MethodNode::getFormals(){
    return this->formals;
}

std::string MethodNode::getName(){
    return this->name;  
}

std::string MethodNode::getType(){
    return this->type;  
}

BlockExpr* MethodNode::getBlock(){
    return this->block;  
}

bool MethodNode::samePrototype(MethodNode &method) {
    bool same = (method.name == name && method.type == type);
    same &= formals.size() == method.formals.size();

    if (!same)
        return same;

    std::list<FormalNode*>::iterator it = formals.begin();
    std::list<FormalNode*>::iterator it2 = method.formals.begin();
    while (it != formals.end()){
        // We only check that types are consistent, variable name can be renamed
        same &= (*it)->getType() == (*it2)->getType();

        ++it;
        ++it2;
    }
    return same;
}

ClassNode *MethodNode::getParent() {
    return this->parent;
}

void MethodNode::setParent(ClassNode* cl) {
    this->parent = cl;
}

void MethodNode::setName(std::string name) {
    this->name = name;
}

void MethodNode::setIndex(unsigned int index){
    this->index = index;
}

unsigned int MethodNode::getIndex(){
    return this->index;
}


/*************** ClassNode Implementation ***************************/

ClassNode::ClassNode(std::string name, std::list<FieldNode*>& fields, std::list<MethodNode*>& methods, yy::location loc, std::string parentName = "Object") : Node(loc){
    this->name = std::move(name);
    this->parentName = std::move(parentName);
    this->fields = fields;
    this->methods = methods;
    parent = NULL;
    children = std::list<ClassNode*>();
}
 
ClassNode::~ClassNode(){
    for (auto& field: fields)
        delete field;
    for (auto& method: methods)
        delete method;
}

std::string ClassNode::getName(){
    return this->name;  
}

std::string ClassNode::getParentName(){
    return this->parentName;  
}

std::list<FieldNode*>& ClassNode::getFields(){
    return this->fields;  
}

std::list<MethodNode*>& ClassNode::getMethods(){
    return this->methods;  
}

ClassNode *ClassNode::getParent() const {
    return parent;
}

void ClassNode::setParent(ClassNode *parent) {
    this->parent = parent;
}

const std::list<ClassNode *> &ClassNode::getChildren() const {
    return children;
}

void ClassNode::addChild(ClassNode* child) {
    children.push_back(child);
}

void ClassNode::visitChildren(std::map<std::string, bool>& visitedClasses){
    if (visitedClasses[name])
        return;

    visitedClasses[name] = true;
    for (auto& child  : this->children){
        child->visitChildren(visitedClasses);
    }
}

bool ClassNode::visitChildren(std::map<std::string, bool> &visitedClasses, std::list<ClassNode*>& names) {
    if (visitedClasses[name])
        return true;

    bool cycle_detected = false;
    visitedClasses[name] = true;

    for (auto& child  : this->children){
        cycle_detected |= child->visitChildren(visitedClasses, names);
    }

    // Create a list containing the cycle
    if (cycle_detected)
        names.push_back(this);
    return cycle_detected;
}

std::list<ClassNode*> ClassNode::getAncestors(){
    std::list<ClassNode*> ancestors;
    if (parent == NULL){
        ancestors.push_front(this);
        return ancestors;
    }
    ancestors = parent->getAncestors();
    ancestors.push_front(this);
    return ancestors;
}

MethodNode* ClassNode::hasMethod(std::string method) {
    MethodNode* callee = NULL;
    for(auto& m: methods){
        callee = m->getName() == method ? m: callee;
    }

    // If method not found, check in the parent class
    if(callee == NULL && parent != NULL)
        callee = parent->hasMethod(method);

    return callee;
}

void ClassNode::inheritMethods(){
    std::list<ClassNode*> ancestors = this->getAncestors();
    std::vector<MethodNode*> methods;

    for(auto it = ancestors.rbegin(); it != ancestors.rend(); ++it){
        for (auto &parent_method: (*it)->getMethods()) {
            bool parent_method_exists = false;
            // If a parent method has already been declared, replace it by the child method
            for(unsigned int i = 0; i < methods.size(); ++i){
                if(methods[i]->getName() == parent_method->getName()){
                    parent_method_exists = true;
                    methods[i] = parent_method;
                }

            }
            // If no parent method has the same prototype, add it to the methods of the child
            if(!parent_method_exists)
                methods.push_back(parent_method);
        }
    }

    this->allMethods = methods;
}

void ClassNode::inheritFields(){
    std::list<ClassNode*> ancestors = this->getAncestors();
    std::vector<FieldNode*> fields;

    for(auto it = ancestors.rbegin(); it != ancestors.rend(); ++it){
        for (auto &parent_field: (*it)->getFields()) {
            fields.push_back(parent_field);
        }
    }

    this->allFields = fields;
}

std::vector<MethodNode*> ClassNode::getAllMethods(){
    if (this->allMethods.empty()){
        inheritMethods();
    }
    return this->allMethods;
}

std::vector<FieldNode *> ClassNode::getAllFields() {
    if(allFields.empty()){
        inheritFields();
    }
    return this->allFields;
}

/*************** ProgramNode Implementation ***************************/

ProgramNode::ProgramNode(std::list<ClassNode*>& classes, yy::location loc) : Node(loc){
    this->classes = classes;
}

ProgramNode::~ProgramNode(){
    for (auto & cl : classes){
        delete cl;
    }
}

std::list<ClassNode*>& ProgramNode::getClasses(){
    return this->classes;  
}

void ProgramNode::initMaps() {
    for (const auto& cl: classes){
        nameToClasses[cl->getName()].push_back(cl);
        visitedClasses[cl->getName()] = false;
    }
}

void ProgramNode::initObjectClass() {
    std::list<FieldNode*> fields;
    std::list<MethodNode*> methods;
    yy::location loc;
    ClassNode* obj = new ClassNode("Object", fields, methods, loc, "");

    std::list<ExprNode*> block;
    std::list<FormalNode*> emptyformals;
    std::list<FormalNode*> stringformals;
    stringformals.push_back(new FormalNode("s", ValueExpr::STRING, loc));
    std::list<FormalNode*> boolformals;
    boolformals.push_back(new FormalNode("b", ValueExpr::BOOL, loc));
    std::list<FormalNode*> int32formals;
    int32formals.push_back(new FormalNode("i", ValueExpr::INT32, loc));
    obj->getMethods().push_back(new MethodNode("print", "Object", new BlockExpr(block, loc), stringformals, loc));
    obj->getMethods().push_back(new MethodNode("printBool", "Object", new BlockExpr(block, loc), boolformals, loc));
    obj->getMethods().push_back(new MethodNode("printInt32", "Object", new BlockExpr(block, loc), int32formals, loc));
    obj->getMethods().push_back(new MethodNode("inputLine", ValueExpr::STRING, new BlockExpr(block, loc), emptyformals, loc));
    obj->getMethods().push_back(new MethodNode("inputBool", ValueExpr::BOOL, new BlockExpr(block, loc), emptyformals, loc));
    obj->getMethods().push_back(new MethodNode("inputInt32", ValueExpr::INT32, new BlockExpr(block, loc), emptyformals, loc));

    classes.push_front(obj);
}

const std::map<std::string, std::list<ClassNode *>>& ProgramNode::getNameToClasses() const {
    return nameToClasses;
}

std::map<std::string, bool> &ProgramNode::getVisitedClasses(){
    return visitedClasses;
}
