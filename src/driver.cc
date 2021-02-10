/* 
    File originally from https://www.gnu.org/software/bison/manual/html_node/Calc_002b_002b-Parsing-Driver.html 
    Some modifications may have been done by me for the project.
*/

#include <visitor/codegen/llvmGen.hh>
#include "driver.hh"
#include "visitor/visitor.hh"
#include "visitor/semantic/semanticVisitor.hh"
#include "visitor/semantic/pass1Visitor.hh"


driver::driver (Mode mode): trace_parsing (false), trace_scanning (false) {
    lexical_error = false;
    syntax_error = false;

    this->mode = mode;
}

int driver::parse (const std::string &f){
    file = f;
    location.initialize (&file);
    scan_begin ();

    // The parser wil parse the code and initialize the ast
    ProgramNode* ast;
    yy::parser parse (*this, &ast);
    parse.set_debug_level (trace_parsing);
    result = parse.parse ();
    scan_end ();

    Visitor* v;
    if (!result && mode == PARSER){
        v = new PrintVisitor;
        ast->accept(* v);

        delete v;
        delete ast;
    } else if (!result && mode >= CHECK){
        // A semantic analysis must be performed
        v = new ClassVisitor;
        ast->accept(* v);
        result = !((ClassVisitor *) v)->getError() ? result : 3;

        if(!result && mode == CHECK){
            // Only print the annotated ast
            delete v;
            v = new ExtendedPrintVisitor;
            ast->accept(*v);
        } else if (!result && mode >= LLVM) {
            // generate the code
            delete v;
            v = new LLVMCodeGen(mode == CODE, f);
            ast->accept(*v);
        }

        delete v;
        delete ast;
    }


    return result;
}

int driver::lex (const std::string& f){
  file = f;
  location.initialize (&file);
  scan_begin ();
  while (yylex(*this).type != yy::parser::token::TOK_END) ;

  scan_end ();

  return lexical_error;
}

