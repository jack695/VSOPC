//
// Created by jack on 16/03/2020.
//

#include <tuple>
#include "symbol_table.hh"

SymbolTable::SymbolTable() {
    variables = std::vector<std::map<std::string, Variable>>();
    methods = std::vector<std::map<std::string, Method>>();
    types = std::map<std::string, std::tuple<std::string, void*, void*>>();
}

SymbolTable::~SymbolTable() {

}

void SymbolTable::pushScope() {
    std::map<std::string, Variable> scoped_var;
    this->variables.push_back(scoped_var);

    std::map<std::string, Method> scoped_methods;
    this->methods.push_back(scoped_methods);
}

void SymbolTable::exitScope() {
    if (!variables.empty()) {
        this->variables.pop_back();
    }
    if (!methods.empty()) {
        this->methods.pop_back();
    }
}

bool SymbolTable::addVariable(std::string key, std::string name, std::string type, void* value) {
    std::map<std::string, Variable> &scope = variables.back();
    if(scope.find(key) != scope.end())
        return false;

    Variable v = Variable(variables.size()-1, name, type, value);
    scope[key] = v;

    return true;
}

bool SymbolTable::addMethod(std::string key, std::string name, void *value) {
    std::map<std::string, Method> &scope = methods.back();
    if(scope.find(key) != scope.end())
        return false;

    Method v = Method(methods.size()-1, name, value);
    scope[key] = v;

    return true;
}

bool SymbolTable::addType(std::string key, std::string name, void *value, void* initializers) {
    if(types.find(key) != types.end())
        return false;

    Type v = Type(name, value, initializers);
    types[key] = v;

    return true;
}

std::list<Variable> SymbolTable::getVariable(const std::string &key, bool this_scope) const {
    std::list<Variable> matches;

    // If the user wants only the symbol in the last scope
    if (this_scope){
        auto& scope = variables.back();
        if (scope.find(key) != scope.end())
            matches.push_front(scope.find(key)->second);
    } else {
        for (auto& scope: variables){
            if (scope.find(key) != scope.end())
                matches.push_front(scope.find(key)->second);
        }
    }

    return matches;
}

std::list<Method> SymbolTable::getMethod(const std::string &key, bool this_scope) const {
    std::list<Method> matches;

    // If the user wants only the symbol in the last scope
    if (this_scope){
        auto& scope = methods.back();
        if (scope.find(key) != scope.end())
            matches.push_front(scope.find(key)->second);
    } else {
        for (auto& scope: methods){
            if (scope.find(key) != scope.end())
                matches.push_front(scope.find(key)->second);
        }
    }

    return matches;
}

Type* SymbolTable::getType(const std::string &key) const {
    if (types.find(key) != types.end())
        return (Type *) (&(types.find(key)->second));
    else
        return NULL;
}