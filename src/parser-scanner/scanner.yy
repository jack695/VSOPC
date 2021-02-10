%{ /* -*- C++ -*- */
# include <cerrno>
# include <climits>
# include <cstdlib>
# include <cstring> // strerror
# include <string>
# include "driver.hh"
# include "parser.tab.hh"
#include <map>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <initializer_list>
#include <stack>
using namespace std;
%}

%option noyywrap nounput noinput batch debug

%{
    /* Mapping between the symbol of the operators and their string */
    std::map<std::string, std::string> op = {{"(", "lpar"}, {")",  "rpar"}, {"{",  "lbrace"}, {"}",  "rbrace"}, {".",  "dot"}, {"=",  "equal"}, {":",  "colon"}, {";",  "semicolon"}, {",",  "comma"}, {"+",  "plus"}, {"<",  "lower"}, {"<=",  "lower-equal"}, {"-", "minus"}, {"*",  "times"}, {"+",  "plus"}, {"/",  "div"}, {"^",  "pow"}, {"<-", "assign"}};

    /* Global variables */
    int nested_lvl = 0;
    /* Temporary string for the parsed characters */
    std::string tmp_str;
    /* Stack with the positions of the opening multi line comment */
    std::stack<yy::location> loc_stack;

    /* Definitions of the function used */
    void output_token(unsigned int l, unsigned int col, string token_class, string token_value);
    void output_error(std::string file_name, unsigned int line, unsigned int column, std::string error_message, driver& drv);
    std::string convert_special_char(char c);
    yy::parser::symbol_type output_keyword(std::string yytext, yy::location& loc);
    yy::parser::symbol_type output_operator(std::string yytext, yy::location& loc);
%}

/* Exclusive start condition */
%x nested_comm
%x str
%x escape

LOWERCASE_LETTER    [a-z]
UPPERCASE_LETTER    [A-Z]
LETTER              {LOWERCASE_LETTER}|{UPPERCASE_LETTER}
BIN_DIGIT           [01]
DIGIT               {BIN_DIGIT}|[2-9]
HEX_DIGIT           {DIGIT}|[a-f]|[A-F]
TAB                 \11
LF                  \12
FF                  \14
CR                  \15
WHITESPACE          [ \11\14\15]

decimal-integer     {DIGIT}+
hex-integer         "0x"{HEX_DIGIT}+
integer-literal     {decimal-integer}|{hex-integer}

keyword             "and"|"bool"|"class"|"do"|"else"|"extends"|"false"|"if"|"in"|"int32"|"isnull"|"let"|"new"|"not"|"string"|"then"|"true"|"unit"|"while"
operator            "("|")"|"{"|"}"|"."|"="|":"|";"|","|"+"|"<"|"<="|"-"|"*"|"+"|"/"|"^"|"<-"
type-identifier     {UPPERCASE_LETTER}({LETTER}|{DIGIT}|[_])*
object-identifier   {LOWERCASE_LETTER}({LETTER}|{DIGIT}|[_])*
identifier          ({type-identifier}|{object-identifier})
escape-sequence     "b"|"t"|"n"|"r"|"""|"\"|"x"{HEX_DIGIT}{HEX_DIGIT}
escaped-char        \\{escape-sequence}
/* '"' removed for simplicity of the scanner */
regular-char        [^\0\12\"\\]
char                {regular-char}|{escaped-char}

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}

%%

%{
  // A handy shortcut to the location held by the driver.
  yy::location& loc = drv.location;
  // Code run each time yylex is called.
  loc.step ();
%}

    /* Special scanner for nested comments*/

"(*"                    {
                            loc_stack.push(loc);
                            loc.step ();
                            BEGIN(nested_comm);
                        }
<nested_comm>{
    "(*"                {
                            loc_stack.push(loc);
                            loc.step ();
                            nested_lvl++;
                        }
    "*"+")"             {
                            loc_stack.pop();

                            if(! nested_lvl){
                                BEGIN(INITIAL); // Return to the normal scanner
                            } else {
                                nested_lvl--;
                            }

                            loc.step ();
                        }
    .                   loc.step (); /* eat anything */ /* TODO inneficient */
    {LF}                {
                            loc.lines (1); 
                            loc.step ();
                        }
    <<EOF>>             {
                            yy::location comm_loc = loc_stack.top();
                            loc_stack.pop();
                            loc_stack = std::stack<yy::location>();

                            output_error(*comm_loc.begin.filename, comm_loc.begin.line, comm_loc.begin.column, "The multi-line comment is not explicitly terminated. EOF encountered.", drv);
                            return yy::parser::make_END (loc);
                        }
}
    /* Special scanner for string literals */
\"                                               {
                                                     tmp_str = "\"";
                                                     BEGIN(str);
                                                 }
<str>{
    \"                                           {
                                                     tmp_str.append(yytext);
                                                     BEGIN(INITIAL); // Return to the normal scanner                                              
                                                     if (drv.mode == LEXER) {
                                                         output_token(loc.begin.line, loc.begin.column, "string-literal", tmp_str);
                                                     }
                                                     return yy::parser::make_STRING_LITERAL (tmp_str, loc);
                                                 }
    \0                                           {
                                                     unsigned int end_col = 0 < loc.end.column ? loc.end.column - 1 : 0;
                                                     output_error(*loc.end.filename, loc.end.line, end_col, "NUL character found in string literal", drv); 
                                                 }
    \12                                          {
                                                     unsigned int end_col = 0 < loc.end.column ? loc.end.column - 1 : 0;
                                                     
                                                     output_error(*loc.end.filename, loc.end.line, end_col, "Raw line feed found in string literal", drv); 
                                                     loc.lines (1);
                                                 } 
    {regular-char}*                              tmp_str.append(yytext);
    \\                                           BEGIN(escape);
    <<EOF>>                                      {
                                                     output_error(*loc.begin.filename, loc.begin.line, loc.begin.column, "The string literal is not explicitly terminated. EOF encountered.", drv);
                                                     return yy::parser::make_END (loc);
                                                 }
}

<escape>{
    [btnr\"\\]                                   {
                                                    /* Convert the escape sequence in a \xhh format */
                                                    tmp_str.append(convert_special_char(yytext[0]));
                                                    BEGIN(str);
                                                 }
    x{HEX_DIGIT}{HEX_DIGIT}                      {
                                                    tmp_str.push_back('\\');
                                                    tmp_str.append(yytext);
                                                    BEGIN(str);
                                                 }
    \12(\11|" ")*                                {
                                                    BEGIN(str);
                                                    loc.lines (1);
                                                    loc.end.column = yyleng; /* column is reset to 1 + nbOfChar - LF = yyleng*/
                                                 }
    (.)                                          {
                                                    std::string msg = "Invalid escape sequence in string literal: '\\";
                                                    msg.append( yytext ); msg.append("'");
                                                    output_error(*loc.end.filename, loc.end.line, loc.end.column-2, msg, drv); /* The error has to point to the '/' */
                                                    BEGIN(str);
                                                 }
}

    /* Basic scanner */

"//".*                                           loc.step ();
{decimal-integer}                                {
                                                     std::string s(yytext);
                                                     //  Remove the leading 0's
                                                     while(s[0] == '0' && s.size() > 1){
                                                        s.erase(0, 1);
                                                     }
                                                     if (drv.mode == LEXER) {
                                                         output_token(loc.begin.line, loc.begin.column, "integer-literal", s);
                                                     }
                                                     return yy::parser::make_INTEGER_LITERAL (std::stoi(s), loc);
                                                 }
{hex-integer}                                    {
                                                     int n;
                                                     std::istringstream(yytext) >> std::hex >> n;
                                                     if (drv.mode == LEXER) {
                                                         output_token(loc.begin.line, loc.begin.column, "integer-literal", std::to_string(n));
                                                     }
                                                     return yy::parser::make_INTEGER_LITERAL (n, loc);
                                                 }
{keyword}                                        {
                                                     if (drv.mode == LEXER) {
                                                         output_token(loc.begin.line, loc.begin.column, yytext, "");
                                                     }
                                                     return output_keyword(yytext, loc);
                                                 }
{type-identifier}                                {
                                                    if (drv.mode == LEXER) {
                                                        output_token(loc.begin.line, loc.begin.column, "type-identifier", yytext);
                                                    }
                                                    return yy::parser::make_TYPE_ID (yytext, loc);
                                                 }
{object-identifier}                              {
                                                     if (drv.mode == LEXER) {
                                                         output_token(loc.begin.line, loc.begin.column, "object-identifier", yytext);
                                                     }
                                                     return yy::parser::make_OBJECT_ID (yytext, loc);
                                                 }
{operator}                                       {
                                                     if (drv.mode == LEXER) {
                                                          output_token(loc.begin.line, loc.begin.column, op[yytext], "");
                                                     }
                                                     return output_operator(yytext, loc);
                                                 }
{LF}+                                            loc.lines (yyleng); loc.step ();
{WHITESPACE}+                                    loc.step (); 

    /* Errors handling */

{integer-literal}{identifier}                    output_error (*loc.begin.filename, loc.begin.line, loc.begin.column, "Invalid integer literal", drv);
.                                                {
                                                     std::string msg = "Invalid VSOP character: '";
                                                     msg.append(yytext); msg.append("'");
                                                     output_error (*loc.begin.filename, loc.begin.line, loc.begin.column, msg, drv); /* TODO Is that enough? */
                                                 }

<<EOF>>    return yy::parser::make_END (loc);

%%



/* Implementation of the functions used */

void output_token(unsigned int l, unsigned int col, string token_class, string token_value){
    assert(! token_class.empty());

    std::cout << l << "," << col << "," << token_class;
    if(! token_value.empty())
        std::cout << "," << token_value;
    std::cout << std::endl;
}

/**
 * Output an error indicating the file, the line and the column of the problem's origin.
 */
void output_error(std::string file_name, unsigned int line, unsigned int column, std::string error_message, driver& drv){
        assert(! error_message.empty());
        drv.lexical_error = true;

        std::cerr << file_name << ":" << line << ":" << column << ": lexical error: " << error_message << std::endl;
    }

// Escape char as described in the VSOP manual
std::string convert_special_char(char c){
    switch(c){
        case 'n': return "\\x0a";
        case 'b': return "\\x08";
        case 't': return "\\x09";
        case 'r': return "\\x0d";
        case '\\': return "\\x5c";
        case '\"': return "\\x22";
        default: return "";
    }
}

// Call the right token maker according to the text
yy::parser::symbol_type output_operator(std::string yytext, yy::location& loc){
    if (yytext == "("){
        return yy::parser::make_LPAR (loc);
    }
    else if (yytext == ")") {
             return yy::parser::make_RPAR (loc);
    }
    else if (yytext == "{") {
             return yy::parser::make_LBRACE (loc);
    }
    else if (yytext == "}") {
             return yy::parser::make_RBRACE (loc);
    }
    else if (yytext == ".") {
             return yy::parser::make_DOT (loc);
    }
    else if (yytext == "=") {
             return yy::parser::make_EQUAL (loc);
    }
    else if (yytext == ":") {
             return yy::parser::make_COLON (loc);
    }
    else if (yytext == ";") {
             return yy::parser::make_SEMICOLON (loc);
    }
    else if (yytext == ",") {
             return yy::parser::make_COMMA (loc);
    }
    else if (yytext == "+") {
             return yy::parser::make_PLUS (loc);
    }
    else if (yytext == "<") {
             return yy::parser::make_LOWER (loc);
    }
    else if (yytext == "<=") {
             return yy::parser::make_LOWER_EQUAL (loc);
    }
    else if (yytext == "-") {
             return yy::parser::make_MINUS (loc);
    }
    else if (yytext == "*") {
             return yy::parser::make_TIMES (loc);
    }
    else if (yytext == "/") {
             return yy::parser::make_DIV (loc);
    }
    else if (yytext == "^") {
             return yy::parser::make_POW (loc);
    }
    else if (yytext == "<-") {
             return yy::parser::make_ASSIGN (loc);
    } else {
        std::cerr << loc.begin.filename << ":" << loc.begin.line << ":" << loc.begin.column << ": lexical error: Internal Error: The string " << yytext << " is not an operator. The function ouput_operator should not be called." << std::endl;
        return yy::parser::make_END (loc);
    }
}

yy::parser::symbol_type output_keyword(std::string yytext, yy::location& loc){
    if (yytext == "and") {
        return yy::parser::make_AND (loc);
    }
    else if (yytext == "bool") {
        return yy::parser::make_BOOL (loc);
    }
    else if (yytext == "class") {
        return yy::parser::make_CLASS (loc);
    }
    else if (yytext == "do") {
        return yy::parser::make_DO (loc);
    }
    else if (yytext == "else") {
        return yy::parser::make_ELSE (loc);
    }
    else if (yytext == "extends") {
        return yy::parser::make_EXTENDS (loc);
    }
    else if (yytext == "false") {
        return yy::parser::make_FALSE (loc);
    }
    else if (yytext == "if") {
        return yy::parser::make_IF (loc);
    }
    else if (yytext == "in") {
        return yy::parser::make_IN (loc);
    }
    else if (yytext == "int32") {
        return yy::parser::make_INT32 (loc);
    }
    else if (yytext == "isnull") {
        return yy::parser::make_ISNULL (loc);
    }
    else if (yytext == "let") {
        return yy::parser::make_LET (loc);
    }
    else if (yytext == "new") {
        return yy::parser::make_NEW (loc);
    }
    else if (yytext == "not") {
        return yy::parser::make_NOT (loc);
    }
    else if (yytext == "string") {
        return yy::parser::make_STRING (loc);
    }
    else if (yytext == "then") {
        return yy::parser::make_THEN (loc);
    }
    else if (yytext == "true") {
        return yy::parser::make_TRUE (loc);
    }
    else if (yytext == "unit") {
        return yy::parser::make_UNIT (loc);
    }
    else if (yytext == "while") {
        return yy::parser::make_WHILE (loc);
    }
    else {
        std::cerr << loc.begin.filename << ":" << loc.begin.line << ":" << loc.begin.column << ": lexical error: Internal Error: The string " << yytext << " is not a keyword. The function ouput_operator should not be called." << std::endl;
        return yy::parser::make_END (loc);
    }
}


