//
// Created by jack on 16/03/2020.
//

#ifndef PART_2_SYMBOL_TABLE_HH
#define PART_2_SYMBOL_TABLE_HH


#include <string>
#include <map>
#include <vector>
#include <list>

typedef std::tuple<unsigned int, std::string, std::string, void*> Variable;
typedef std::tuple<unsigned int, std::string, void*> Method;
typedef std::tuple<std::string, void*, void*> Type;

class SymbolTable {
public:
    SymbolTable ();
    ~SymbolTable ();
    /**
     * Add a new variable to the current scope.
     * @param key Key to retrieve this variable.
     * @param name Name of the variable.
     * @param type Type of the variable
     * @param value Generic pointer to the value.
     * @return false if a variable with the same key is already declared in the current scope.
     */
    bool addVariable(std::string key, std::string name, std::string type, void* value);
    /**
     * Add a new method to the current scope.
     * @param key Key of the method.
     * @param name Name of the method.
     * @param value Generic pointer to the value associated to the method.
     * @return false if a method with the same key is already declared in the current scope.
     */
    bool addMethod(std::string key, std::string name, void* value);
    /**
     * Add a new type.
     * @param key Key of the type.
     * @param name Name of the type.
     * @param value Any value to associate to the type.
     * @param cl A pointer to the class associated to the type.
     * @return false if a type with the same key has already been declared.
     */
    bool addType(std::string key, std::string name, void* value, void* cl = NULL);
    /**
     * Get the list of variables associated to a key.
     * @param key Key of the variable.
     * @param this_scope If true, search only in the current scope.
     * @return The list of variables with this key.
     */
    std::list<Variable> getVariable(const std::string& key, bool this_scope = false) const ;
    /**
     * Get the list of methods associated to a key.
     * @param key Key of the method.
     * @param this_scope If true, search only in the current scope.
     * @return The list of methods with this key.
     */
    std::list<Method> getMethod(const std::string& key, bool this_scope = false) const ;
    /**
     * Return a type.
     * @param key The key of the type.
     * @return The type associated to the key.
     */
    Type* getType(const std::string& key) const ;

    /**
     * Push a new empty scope for the variables and methods.
     */
    void pushScope();
    /**
     * Remove the last scope and the variables and methods declared in it.
     */
    void exitScope();

private:
    std::vector<std::map<std::string, Variable>> variables;
    std::vector<std::map<std::string, Method>> methods;
    std::map<std::string, Type> types;
};


#endif //PART_2_SYMBOL_TABLE_HH
