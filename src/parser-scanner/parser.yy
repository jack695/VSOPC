/*
    File originally from https://www.gnu.org/software/bison/manual/html_node/Calc_002b_002b-Parser.html
    Some modifications may have been done by me for the project.
*/

%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0.4"
%defines

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
  #include <string>
  #include "ast/ast.hh"
  #include <tuple>
  class driver;
}

// The parsing context.
%param { driver& drv }
%parse-param { ProgramNode** ast }

%locations

%define parse.trace
%define parse.error verbose

%code {
# include "driver.hh"
#include <iostream>
#include <typeinfo>
#include <stack>
}

%define api.token.prefix {TOK_}
%token
    END  0  "end of file"
;
%nterm <std::list<ClassNode*>> classes
%nterm <std::tuple<std::list<FieldNode*>, std::list<MethodNode*>>> class-body
%nterm <std::tuple<std::list<FieldNode*>, std::list<MethodNode*>>> class-body-part
%nterm <std::list<FormalNode*>> formals
%nterm <std::list<FormalNode*>> formals-list

%nterm <ClassNode*> class
%nterm <FieldNode*> field
%nterm <MethodNode*> method
%nterm <FormalNode*> formal

%nterm <ExprNode*> init.opt
%nterm <BlockExpr*> block
%nterm <std::list<ExprNode*>> expr-list-semicolon
%nterm <std::list<ExprNode*>> args
%nterm <std::list<ExprNode*>> expr-list-comma

%nterm <ExprNode*> expr
%nterm <ExprNode*> if-stmt
%nterm <ValueExpr*> literal
%nterm <ValueExpr*> boolean-literal

%token <int> INTEGER_LITERAL
%nterm <std::string> extension.opt
%nterm <std::string> type
%token <std::string> STRING_LITERAL
%token <std::string> TYPE_ID
%token <std::string> OBJECT_ID

%token BOOL
%token CLASS
%token DO
%token EXTENDS
%token FALSE
%token IF
%token IN
%token INT32
%token NEW
%token STRING
%token TRUE
%token UNIT

%token LBRACE
%token RBRACE
%token LET WHILE
%token LPAR
%token RPAR
%token COLON
%token SEMICOLON
%token COMMA

%precedence IN DO
%precedence THEN
%precedence ELSE
%precedence SEMICOLON

%right ASSIGN
%left AND
%right NOT
%nonassoc LOWER LOWER_EQUAL EQUAL
%left PLUS MINUS
%left TIMES DIV
%right ISNULL NEG /* handle unary with higher priority */
%right POW
%left DOT

/*%printer { yyo << $$; } <*>;*/

%%
%start program;

program: classes class                                       {
                                                                std::list<ClassNode*> classes;
                                                                classes = $1;
                                                                classes.push_back($2);

                                                                @$.begin = @1.begin;
                                                                @$.end = @2.end;
                                                                ProgramNode* program = new ProgramNode(classes, @$);
                                                                int result;
 
                                                                if (drv.lexical_error){
                                                                    result = 1;
                                                                } else {
                                                                    if (! drv.syntax_error) {
                                                                        result = 0;
                                                                    }
                                                                    else{
                                                                        result = 2;
                                                                    }
                                                                }

                                                                (*ast) = program;
                                                                return result;
                                                             }
    ;

classes:
      %empty                                                 {
                                                                std::list<ClassNode*> classes;
                                                                $$ = classes;
                                                             }
    | classes class                                          {
                                                                $1.push_back($2);
                                                                $$ = $1;
 
                                                             }
    ;

class:
      CLASS TYPE_ID extension.opt class-body                 {
                                                                @$.begin = @1.begin;
                                                                @$.end = @4.end;
                                                                $$ = new ClassNode($2, std::get<0>($4), std::get<1>($4), @$, $3);
                                                             }
    ;

extension.opt:
      %empty                                                 {
                                                                $$ = "Object";
                                                             }
    | EXTENDS TYPE_ID                                        {
                                                                $$ = $2;
                                                             }
    ;

class-body:
      LBRACE class-body-part RBRACE                          {
                                                                $$ = $2;
                                                             }
      ;

class-body-part:
      %empty                                                 {
                                                                std::tuple<std::list<FieldNode>, std::list<MethodNode>> classBody;
                                                             }
    | class-body-part field                                  {
                                                                std::get<0>($1).push_back($2);
                                                                $$ = $1;
                                                             }
    | class-body-part method                                 {
                                                                std::get<1>($1).push_back($2);
                                                                $$ = $1;
                                                             }
    ;

field:
      OBJECT_ID COLON type init.opt SEMICOLON                {
                                                                @$.begin = @1.begin;
                                                                @$.end = @5.end;
                                                                $$ = new FieldNode($1, $3, $4, @$);
                                                             }
      ;

init.opt:
      %empty                                                 {
                                                                $$ = NULL;
                                                             }
    | ASSIGN expr                                            {
                                                                $$ = $2;
                                                             }
    ;

method:
      OBJECT_ID LPAR formals RPAR COLON type block           {
                                                                @$.begin = @1.begin;
                                                                @$.end = @7.end;
                                                                $$ = new MethodNode($1, $6, $7, $3, @$);
                                                             }

type:
      TYPE_ID                                                { $$ = $1; }
    | INT32                                                  { $$ = "int32"; }
    | BOOL                                                   { $$ = "bool"; }
    | STRING                                                 { $$ = "string"; }
    | UNIT                                                   { $$ = "unit"; }
    ;

formals:
      %empty                                                 {
                                                                std::list<FormalNode*> formals;
                                                                $$ = formals;
                                                             }
    | formals-list                                           {
                                                                $$ = $1;
                                                             }
    ;

formals-list:
      formal                                                 {
                                                                std::list<FormalNode*> formals;
                                                                formals.push_back($1);
                                                                $$ = formals;
                                                             }
    | formals-list COMMA formal                              {
                                                                $1.push_back($3);
                                                                $$ = $1;
                                                             }
    ;

formal:
      OBJECT_ID COLON type                                   {
                                                                @$.begin = @1.begin;
                                                                @$.end = @3.end;
                                                                $$ = new FormalNode ($1, $3, @$);
                                                             }
      ;

block:
    LBRACE expr-list-semicolon RBRACE                        {
                                                                @$.begin = @1.begin;
                                                                @$.end = @3.end;
                                                                $$ = new BlockExpr($2, @$);
                                                             }

expr-list-semicolon:
      expr                                                   {
                                                                std::list<ExprNode*> exprList;
                                                                exprList.push_back($1);
                                                                $$ = exprList;
                                                             }
    | expr-list-semicolon SEMICOLON expr                     {
                                                                $1.push_back($3);
                                                                $$ = $1;
                                                             }
    ;


expr:
      if-stmt                                                {  $$ = $1; }
    | WHILE expr DO expr                                     {
                                                                @$.begin = @1.begin;
                                                                @$.end = @4.end;
                                                                $$ = new WhileExpr ($2, $4, @$);
                                                             }
    | LET OBJECT_ID COLON type init.opt IN expr              {
                                                                std::list<ExprNode*> exprList;
                                                                @$.begin = @1.begin;
                                                                @$.end = @7.end;
                                                                $$ = new LetExpr($2, $4, $5, $7, @$);
                                                             }
    | OBJECT_ID ASSIGN expr                                  {
                                                                @$.begin = @1.begin;
                                                                @$.end = @3.end;
                                                                $$ = new AssignExpr($1, $3, @$);
                                                             }
    | NOT expr                                               {
                                                                @$.begin = @1.begin;
                                                                @$.end = @2.end;
                                                                $$ = new UnOpExpr ("not", $2, @$);
                                                             }
    | expr AND expr                                          {
                                                                std::list<ExprNode*> exprList;
                                                                exprList.push_back($1);
                                                                exprList.push_back($3);
                                                                @$.begin = @1.begin;
                                                                @$.end = @3.end;
                                                                $$ = new BinOpExpr ("and", $1, $3, @$);
                                                             }
    | expr EQUAL expr                                        {
                                                                std::list<ExprNode*> exprList;
                                                                exprList.push_back($1);
                                                                exprList.push_back($3);
                                                                @$.begin = @1.begin;
                                                                @$.end = @3.end;
                                                                $$ = new BinOpExpr ("=", $1, $3, @$);
                                                             }
    | expr LOWER expr                                        {
                                                                std::list<ExprNode*> exprList;
                                                                exprList.push_back($1);
                                                                exprList.push_back($3);
                                                                @$.begin = @1.begin;
                                                                @$.end = @3.end;
                                                                $$ = new BinOpExpr ("<", $1, $3, @$);
                                                             }
    | expr LOWER_EQUAL expr                                  {
                                                                std::list<ExprNode*> exprList;
                                                                exprList.push_back($1);
                                                                exprList.push_back($3);
                                                                @$.begin = @1.begin;
                                                                @$.end = @3.end;
                                                                $$ = new BinOpExpr ("<=", $1, $3, @$);
                                                             }
    | expr PLUS expr                                         {
                                                                std::list<ExprNode*> exprList;
                                                                exprList.push_back($1);
                                                                exprList.push_back($3);
                                                                @$.begin = @1.begin;
                                                                @$.end = @3.end;
                                                                $$ = new BinOpExpr ("+", $1, $3, @$);
                                                             }
    | expr MINUS expr                                        {
                                                                std::list<ExprNode*> exprList;
                                                                exprList.push_back($1);
                                                                exprList.push_back($3);
                                                                @$.begin = @1.begin;
                                                                @$.end = @3.end;
                                                                $$ = new BinOpExpr ("-", $1, $3, @$);
                                                             }
    | expr TIMES expr                                        {
                                                                std::list<ExprNode*> exprList;
                                                                exprList.push_back($1);
                                                                exprList.push_back($3);
                                                                @$.begin = @1.begin;
                                                                @$.end = @3.end;
                                                                $$ = new BinOpExpr ("*", $1, $3, @$);
                                                             }
    | expr DIV expr                                          {
                                                                std::list<ExprNode*> exprList;
                                                                exprList.push_back($1);
                                                                exprList.push_back($3);
                                                                @$.begin = @1.begin;
                                                                @$.end = @3.end;
                                                                $$ = new BinOpExpr ("/", $1, $3, @$);
                                                             }
    | expr POW expr                                          {
                                                                std::list<ExprNode*> exprList;
                                                                exprList.push_back($1);
                                                                exprList.push_back($3);
                                                                @$.begin = @1.begin;
                                                                @$.end = @3.end;
                                                                $$ = new BinOpExpr ("^", $1, $3, @$);
                                                             }
    | MINUS expr %prec NEG                                   {
                                                                @$.begin = @1.begin;
                                                                @$.end = @2.end;
                                                                $$ = new UnOpExpr ("-", $2, @$);
                                                             }
    | ISNULL expr                                            {
                                                                @$.begin = @1.begin;
                                                                @$.end = @2.end;
                                                                $$ = new UnOpExpr ("isnull", $2, @$);
                                                             }
    | NEW TYPE_ID                                            {
                                                                std::list<ExprNode*> exprList;
                                                                @$.begin = @1.begin;
                                                                @$.end = @2.end;
                                                                $$ = new NewExpr ($2, @$);
                                                             }
    | OBJECT_ID                                              {
                                                                @$.begin = @1.begin;
                                                                @$.end = @1.end;
                                                                $$ = new ValueExpr($1, ValueExpr::UNKNOWN, @$, false);
                                                             }
    | literal                                                {
                                                                $$ = $1;
                                                             }
    | LPAR RPAR                                              {
                                                                @$.begin = @1.begin;
                                                                @$.end = @2.end;
                                                                $$ = new ValueExpr("()", ValueExpr::UNIT, @$);
                                                             }
    | LPAR expr RPAR                                         {
                                                                $$ = $2;
                                                             }
    | block                                                  {
                                                                $$ = $1;
                                                             }
    | OBJECT_ID LPAR args RPAR                               {
                                                                @$.begin = @1.begin;
                                                                @$.end = @4.end;
                                                                $$ = new CallExpr(new ValueExpr("self", ValueExpr::UNKNOWN, @1, false), $1, $3, @$);
                                                             }
    | expr DOT OBJECT_ID LPAR args RPAR                      {
                                                                @$.begin = @1.begin;
                                                                @$.end = @6.end;
                                                                $$ = new CallExpr($1, $3, $5, @$);
                                                             }
    ;

if-stmt:
      IF expr THEN expr                                      {
                                                                $$ = new IfExpr($2, $4, NULL, @$);
                                                             }
    | IF expr THEN expr ELSE expr                            {
                                                                $$ = new IfExpr($2, $4, $6, @$);
                                                             }
    ;

args:
      %empty                                                 {
                                                                std::list<ExprNode*> exprList;
                                                                $$ = exprList;
                                                             }
    | expr-list-comma                                        {
                                                                $$ = $1;
                                                             }
    ;

expr-list-comma:
      expr                                                   {
                                                                std::list<ExprNode*> exprList;
                                                                exprList.push_back($1);
                                                                $$ = exprList;
                                                             }
    | expr-list-comma COMMA expr                             {
                                                                $1.push_back($3);
                                                                $$ = $1;
                                                             }
    ;


literal:
      INTEGER_LITERAL                                        {
                                                                @$.begin = @1.begin;
                                                                @$.end = @1.end;
                                                                $$ = new ValueExpr(std::to_string($1), ValueExpr::INT32, @$);
                                                             }
    | STRING_LITERAL                                         {
                                                                @$.begin = @1.begin;
                                                                @$.end = @1.end;
                                                                $$ = new ValueExpr($1, ValueExpr::STRING, @$);
                                                             }
    | boolean-literal                                        {
                                                                $$ = $1;
                                                             }
    ;

boolean-literal:
      TRUE                                                   {
                                                                @$.begin = @1.begin;
                                                                @$.end = @1.end;
                                                                $$ = new ValueExpr("true", ValueExpr::BOOL, @$);
                                                             }
    | FALSE                                                  {
                                                                @$.begin = @1.begin;
                                                                @$.end = @1.end;
                                                                $$ = new ValueExpr("false", ValueExpr::BOOL, @$);
                                                             }
    ;

%%

void
yy::parser::error (const location_type& l, const std::string& m)
{
  drv.syntax_error = true;
  std::cerr << *l.begin.filename << ":" << l.begin.line << ":" << l.begin.column << ": " << m << '\n';

}
