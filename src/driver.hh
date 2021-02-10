/* 
    File originally from https://www.gnu.org/software/bison/manual/html_node/Calc_002b_002b-Parsing-Driver.html 
    Some modifications may have been done by me for the project.
*/

# ifndef DRIVER_HH
# define DRIVER_HH
# include <string>
# include <map>
# include "parser-scanner/parser.tab.hh"

// Give Flex the prototype of yylex we want ...
# define YY_DECL \
  yy::parser::symbol_type yylex (driver& drv)
// ... and declare it for the parser's sake.
YY_DECL;

typedef enum{LEXER, PARSER, CHECK, LLVM, CODE} Mode;

// Conducting the whole scanning and parsing of Calc++.
class driver
{
public:
  driver (Mode mode);

  int result;
  Mode mode;

  // Run the parser on file F.  Return 0 on success.
  int parse (const std::string& f);
  // Run the lexer on file F.  Return 0 on success.
  int lex (const std::string& f);
  // The name of the file being parsed.
  std::string file;
  // Whether to generate parser debug traces.
  bool trace_parsing;

  // Handling the scanner.
  void scan_begin (); // Implemented in scanner.yy
  void scan_end (); // Implemented in scanner.yy
  // Whether to generate scanner debug traces.
  bool trace_scanning;
  // The token's location used by the scanner.
  yy::location location;
  // Whether an error has occured during the lexical analysis
  bool lexical_error;
  // Whether an error has occured during the syntax analysis
  bool syntax_error;
};
#endif // ! DRIVER_HH

