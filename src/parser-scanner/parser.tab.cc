// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.


// First part of user declarations.

#line 37 "parser.tab.cc" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "parser.tab.hh"

// User implementation prologue.

#line 51 "parser.tab.cc" // lalr1.cc:412
// Unqualified %code blocks.
#line 30 "parser.yy" // lalr1.cc:413

# include "driver.hh"
#include <iostream>
#include <typeinfo>
#include <stack>

#line 60 "parser.tab.cc" // lalr1.cc:413


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace yy {
#line 146 "parser.tab.cc" // lalr1.cc:479

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              // Fall through.
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  parser::parser (driver& drv_yyarg, ProgramNode** ast_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      drv (drv_yyarg),
      ast (ast_yyarg)
  {}

  parser::~parser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  inline
  parser::by_state::by_state ()
    : state (empty_state)
  {}

  inline
  parser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  parser::by_state::clear ()
  {
    state = empty_state;
  }

  inline
  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  inline
  parser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  parser::symbol_number_type
  parser::by_state::type_get () const
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  inline
  parser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  parser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
      switch (that.type_get ())
    {
      case 55: // block
        value.move< BlockExpr* > (that.value);
        break;

      case 50: // class
        value.move< ClassNode* > (that.value);
        break;

      case 54: // init.opt
      case 59: // expr
      case 60: // if-stmt
        value.move< ExprNode* > (that.value);
        break;

      case 51: // field
        value.move< FieldNode* > (that.value);
        break;

      case 53: // formal
        value.move< FormalNode* > (that.value);
        break;

      case 52: // method
        value.move< MethodNode* > (that.value);
        break;

      case 61: // literal
      case 62: // boolean-literal
        value.move< ValueExpr* > (that.value);
        break;

      case 3: // INTEGER_LITERAL
        value.move< int > (that.value);
        break;

      case 45: // classes
        value.move< std::list<ClassNode*> > (that.value);
        break;

      case 56: // expr-list-semicolon
      case 57: // args
      case 58: // expr-list-comma
        value.move< std::list<ExprNode*> > (that.value);
        break;

      case 48: // formals
      case 49: // formals-list
        value.move< std::list<FormalNode*> > (that.value);
        break;

      case 4: // STRING_LITERAL
      case 5: // TYPE_ID
      case 6: // OBJECT_ID
      case 63: // extension.opt
      case 64: // type
        value.move< std::string > (that.value);
        break;

      case 46: // class-body
      case 47: // class-body-part
        value.move< std::tuple<std::list<FieldNode*>, std::list<MethodNode*>> > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

  inline
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
      switch (that.type_get ())
    {
      case 55: // block
        value.copy< BlockExpr* > (that.value);
        break;

      case 50: // class
        value.copy< ClassNode* > (that.value);
        break;

      case 54: // init.opt
      case 59: // expr
      case 60: // if-stmt
        value.copy< ExprNode* > (that.value);
        break;

      case 51: // field
        value.copy< FieldNode* > (that.value);
        break;

      case 53: // formal
        value.copy< FormalNode* > (that.value);
        break;

      case 52: // method
        value.copy< MethodNode* > (that.value);
        break;

      case 61: // literal
      case 62: // boolean-literal
        value.copy< ValueExpr* > (that.value);
        break;

      case 3: // INTEGER_LITERAL
        value.copy< int > (that.value);
        break;

      case 45: // classes
        value.copy< std::list<ClassNode*> > (that.value);
        break;

      case 56: // expr-list-semicolon
      case 57: // args
      case 58: // expr-list-comma
        value.copy< std::list<ExprNode*> > (that.value);
        break;

      case 48: // formals
      case 49: // formals-list
        value.copy< std::list<FormalNode*> > (that.value);
        break;

      case 4: // STRING_LITERAL
      case 5: // TYPE_ID
      case 6: // OBJECT_ID
      case 63: // extension.opt
      case 64: // type
        value.copy< std::string > (that.value);
        break;

      case 46: // class-body
      case 47: // class-body-part
        value.copy< std::tuple<std::list<FieldNode*>, std::list<MethodNode*>> > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
  parser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  parser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  parser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
  parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::parse ()
  {
    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            symbol_type yylookahead (yylex (drv));
            yyla.move (yylookahead);
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
        switch (yyr1_[yyn])
    {
      case 55: // block
        yylhs.value.build< BlockExpr* > ();
        break;

      case 50: // class
        yylhs.value.build< ClassNode* > ();
        break;

      case 54: // init.opt
      case 59: // expr
      case 60: // if-stmt
        yylhs.value.build< ExprNode* > ();
        break;

      case 51: // field
        yylhs.value.build< FieldNode* > ();
        break;

      case 53: // formal
        yylhs.value.build< FormalNode* > ();
        break;

      case 52: // method
        yylhs.value.build< MethodNode* > ();
        break;

      case 61: // literal
      case 62: // boolean-literal
        yylhs.value.build< ValueExpr* > ();
        break;

      case 3: // INTEGER_LITERAL
        yylhs.value.build< int > ();
        break;

      case 45: // classes
        yylhs.value.build< std::list<ClassNode*> > ();
        break;

      case 56: // expr-list-semicolon
      case 57: // args
      case 58: // expr-list-comma
        yylhs.value.build< std::list<ExprNode*> > ();
        break;

      case 48: // formals
      case 49: // formals-list
        yylhs.value.build< std::list<FormalNode*> > ();
        break;

      case 4: // STRING_LITERAL
      case 5: // TYPE_ID
      case 6: // OBJECT_ID
      case 63: // extension.opt
      case 64: // type
        yylhs.value.build< std::string > ();
        break;

      case 46: // class-body
      case 47: // class-body-part
        yylhs.value.build< std::tuple<std::list<FieldNode*>, std::list<MethodNode*>> > ();
        break;

      default:
        break;
    }


      // Compute the default @$.
      {
        slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, slice, yylen);
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 2:
#line 112 "parser.yy" // lalr1.cc:859
    {
                                                                std::list<ClassNode*> classes;
                                                                classes = yystack_[1].value.as< std::list<ClassNode*> > ();
                                                                classes.push_back(yystack_[0].value.as< ClassNode* > ());

                                                                yylhs.location.begin = yystack_[1].location.begin;
                                                                yylhs.location.end = yystack_[0].location.end;
                                                                ProgramNode* program = new ProgramNode(classes, yylhs.location);
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
#line 734 "parser.tab.cc" // lalr1.cc:859
    break;

  case 3:
#line 139 "parser.yy" // lalr1.cc:859
    {
                                                                std::list<ClassNode*> classes;
                                                                yylhs.value.as< std::list<ClassNode*> > () = classes;
                                                             }
#line 743 "parser.tab.cc" // lalr1.cc:859
    break;

  case 4:
#line 143 "parser.yy" // lalr1.cc:859
    {
                                                                yystack_[1].value.as< std::list<ClassNode*> > ().push_back(yystack_[0].value.as< ClassNode* > ());
                                                                yylhs.value.as< std::list<ClassNode*> > () = yystack_[1].value.as< std::list<ClassNode*> > ();
 
                                                             }
#line 753 "parser.tab.cc" // lalr1.cc:859
    break;

  case 5:
#line 151 "parser.yy" // lalr1.cc:859
    {
                                                                yylhs.location.begin = yystack_[3].location.begin;
                                                                yylhs.location.end = yystack_[0].location.end;
                                                                yylhs.value.as< ClassNode* > () = new ClassNode(yystack_[2].value.as< std::string > (), std::get<0>(yystack_[0].value.as< std::tuple<std::list<FieldNode*>, std::list<MethodNode*>> > ()), std::get<1>(yystack_[0].value.as< std::tuple<std::list<FieldNode*>, std::list<MethodNode*>> > ()), yylhs.location, yystack_[1].value.as< std::string > ());
                                                             }
#line 763 "parser.tab.cc" // lalr1.cc:859
    break;

  case 6:
#line 159 "parser.yy" // lalr1.cc:859
    {
                                                                yylhs.value.as< std::string > () = "Object";
                                                             }
#line 771 "parser.tab.cc" // lalr1.cc:859
    break;

  case 7:
#line 162 "parser.yy" // lalr1.cc:859
    {
                                                                yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
                                                             }
#line 779 "parser.tab.cc" // lalr1.cc:859
    break;

  case 8:
#line 168 "parser.yy" // lalr1.cc:859
    {
                                                                yylhs.value.as< std::tuple<std::list<FieldNode*>, std::list<MethodNode*>> > () = yystack_[1].value.as< std::tuple<std::list<FieldNode*>, std::list<MethodNode*>> > ();
                                                             }
#line 787 "parser.tab.cc" // lalr1.cc:859
    break;

  case 9:
#line 174 "parser.yy" // lalr1.cc:859
    {
                                                                std::tuple<std::list<FieldNode>, std::list<MethodNode>> classBody;
                                                             }
#line 795 "parser.tab.cc" // lalr1.cc:859
    break;

  case 10:
#line 177 "parser.yy" // lalr1.cc:859
    {
                                                                std::get<0>(yystack_[1].value.as< std::tuple<std::list<FieldNode*>, std::list<MethodNode*>> > ()).push_back(yystack_[0].value.as< FieldNode* > ());
                                                                yylhs.value.as< std::tuple<std::list<FieldNode*>, std::list<MethodNode*>> > () = yystack_[1].value.as< std::tuple<std::list<FieldNode*>, std::list<MethodNode*>> > ();
                                                             }
#line 804 "parser.tab.cc" // lalr1.cc:859
    break;

  case 11:
#line 181 "parser.yy" // lalr1.cc:859
    {
                                                                std::get<1>(yystack_[1].value.as< std::tuple<std::list<FieldNode*>, std::list<MethodNode*>> > ()).push_back(yystack_[0].value.as< MethodNode* > ());
                                                                yylhs.value.as< std::tuple<std::list<FieldNode*>, std::list<MethodNode*>> > () = yystack_[1].value.as< std::tuple<std::list<FieldNode*>, std::list<MethodNode*>> > ();
                                                             }
#line 813 "parser.tab.cc" // lalr1.cc:859
    break;

  case 12:
#line 188 "parser.yy" // lalr1.cc:859
    {
                                                                yylhs.location.begin = yystack_[4].location.begin;
                                                                yylhs.location.end = yystack_[0].location.end;
                                                                yylhs.value.as< FieldNode* > () = new FieldNode(yystack_[4].value.as< std::string > (), yystack_[2].value.as< std::string > (), yystack_[1].value.as< ExprNode* > (), yylhs.location);
                                                             }
#line 823 "parser.tab.cc" // lalr1.cc:859
    break;

  case 13:
#line 196 "parser.yy" // lalr1.cc:859
    {
                                                                yylhs.value.as< ExprNode* > () = NULL;
                                                             }
#line 831 "parser.tab.cc" // lalr1.cc:859
    break;

  case 14:
#line 199 "parser.yy" // lalr1.cc:859
    {
                                                                yylhs.value.as< ExprNode* > () = yystack_[0].value.as< ExprNode* > ();
                                                             }
#line 839 "parser.tab.cc" // lalr1.cc:859
    break;

  case 15:
#line 205 "parser.yy" // lalr1.cc:859
    {
                                                                yylhs.location.begin = yystack_[6].location.begin;
                                                                yylhs.location.end = yystack_[0].location.end;
                                                                yylhs.value.as< MethodNode* > () = new MethodNode(yystack_[6].value.as< std::string > (), yystack_[1].value.as< std::string > (), yystack_[0].value.as< BlockExpr* > (), yystack_[4].value.as< std::list<FormalNode*> > (), yylhs.location);
                                                             }
#line 849 "parser.tab.cc" // lalr1.cc:859
    break;

  case 16:
#line 212 "parser.yy" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 855 "parser.tab.cc" // lalr1.cc:859
    break;

  case 17:
#line 213 "parser.yy" // lalr1.cc:859
    { yylhs.value.as< std::string > () = "int32"; }
#line 861 "parser.tab.cc" // lalr1.cc:859
    break;

  case 18:
#line 214 "parser.yy" // lalr1.cc:859
    { yylhs.value.as< std::string > () = "bool"; }
#line 867 "parser.tab.cc" // lalr1.cc:859
    break;

  case 19:
#line 215 "parser.yy" // lalr1.cc:859
    { yylhs.value.as< std::string > () = "string"; }
#line 873 "parser.tab.cc" // lalr1.cc:859
    break;

  case 20:
#line 216 "parser.yy" // lalr1.cc:859
    { yylhs.value.as< std::string > () = "unit"; }
#line 879 "parser.tab.cc" // lalr1.cc:859
    break;

  case 21:
#line 220 "parser.yy" // lalr1.cc:859
    {
                                                                std::list<FormalNode*> formals;
                                                                yylhs.value.as< std::list<FormalNode*> > () = formals;
                                                             }
#line 888 "parser.tab.cc" // lalr1.cc:859
    break;

  case 22:
#line 224 "parser.yy" // lalr1.cc:859
    {
                                                                yylhs.value.as< std::list<FormalNode*> > () = yystack_[0].value.as< std::list<FormalNode*> > ();
                                                             }
#line 896 "parser.tab.cc" // lalr1.cc:859
    break;

  case 23:
#line 230 "parser.yy" // lalr1.cc:859
    {
                                                                std::list<FormalNode*> formals;
                                                                formals.push_back(yystack_[0].value.as< FormalNode* > ());
                                                                yylhs.value.as< std::list<FormalNode*> > () = formals;
                                                             }
#line 906 "parser.tab.cc" // lalr1.cc:859
    break;

  case 24:
#line 235 "parser.yy" // lalr1.cc:859
    {
                                                                yystack_[2].value.as< std::list<FormalNode*> > ().push_back(yystack_[0].value.as< FormalNode* > ());
                                                                yylhs.value.as< std::list<FormalNode*> > () = yystack_[2].value.as< std::list<FormalNode*> > ();
                                                             }
#line 915 "parser.tab.cc" // lalr1.cc:859
    break;

  case 25:
#line 242 "parser.yy" // lalr1.cc:859
    {
                                                                yylhs.location.begin = yystack_[2].location.begin;
                                                                yylhs.location.end = yystack_[0].location.end;
                                                                yylhs.value.as< FormalNode* > () = new FormalNode (yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > (), yylhs.location);
                                                             }
#line 925 "parser.tab.cc" // lalr1.cc:859
    break;

  case 26:
#line 250 "parser.yy" // lalr1.cc:859
    {
                                                                yylhs.location.begin = yystack_[2].location.begin;
                                                                yylhs.location.end = yystack_[0].location.end;
                                                                yylhs.value.as< BlockExpr* > () = new BlockExpr(yystack_[1].value.as< std::list<ExprNode*> > (), yylhs.location);
                                                             }
#line 935 "parser.tab.cc" // lalr1.cc:859
    break;

  case 27:
#line 257 "parser.yy" // lalr1.cc:859
    {
                                                                std::list<ExprNode*> exprList;
                                                                exprList.push_back(yystack_[0].value.as< ExprNode* > ());
                                                                yylhs.value.as< std::list<ExprNode*> > () = exprList;
                                                             }
#line 945 "parser.tab.cc" // lalr1.cc:859
    break;

  case 28:
#line 262 "parser.yy" // lalr1.cc:859
    {
                                                                yystack_[2].value.as< std::list<ExprNode*> > ().push_back(yystack_[0].value.as< ExprNode* > ());
                                                                yylhs.value.as< std::list<ExprNode*> > () = yystack_[2].value.as< std::list<ExprNode*> > ();
                                                             }
#line 954 "parser.tab.cc" // lalr1.cc:859
    break;

  case 29:
#line 270 "parser.yy" // lalr1.cc:859
    {  yylhs.value.as< ExprNode* > () = yystack_[0].value.as< ExprNode* > (); }
#line 960 "parser.tab.cc" // lalr1.cc:859
    break;

  case 30:
#line 271 "parser.yy" // lalr1.cc:859
    {
                                                                yylhs.location.begin = yystack_[3].location.begin;
                                                                yylhs.location.end = yystack_[0].location.end;
                                                                yylhs.value.as< ExprNode* > () = new WhileExpr (yystack_[2].value.as< ExprNode* > (), yystack_[0].value.as< ExprNode* > (), yylhs.location);
                                                             }
#line 970 "parser.tab.cc" // lalr1.cc:859
    break;

  case 31:
#line 276 "parser.yy" // lalr1.cc:859
    {
                                                                std::list<ExprNode*> exprList;
                                                                yylhs.location.begin = yystack_[6].location.begin;
                                                                yylhs.location.end = yystack_[0].location.end;
                                                                yylhs.value.as< ExprNode* > () = new LetExpr(yystack_[5].value.as< std::string > (), yystack_[3].value.as< std::string > (), yystack_[2].value.as< ExprNode* > (), yystack_[0].value.as< ExprNode* > (), yylhs.location);
                                                             }
#line 981 "parser.tab.cc" // lalr1.cc:859
    break;

  case 32:
#line 282 "parser.yy" // lalr1.cc:859
    {
                                                                yylhs.location.begin = yystack_[2].location.begin;
                                                                yylhs.location.end = yystack_[0].location.end;
                                                                yylhs.value.as< ExprNode* > () = new AssignExpr(yystack_[2].value.as< std::string > (), yystack_[0].value.as< ExprNode* > (), yylhs.location);
                                                             }
#line 991 "parser.tab.cc" // lalr1.cc:859
    break;

  case 33:
#line 287 "parser.yy" // lalr1.cc:859
    {
                                                                yylhs.location.begin = yystack_[1].location.begin;
                                                                yylhs.location.end = yystack_[0].location.end;
                                                                yylhs.value.as< ExprNode* > () = new UnOpExpr ("not", yystack_[0].value.as< ExprNode* > (), yylhs.location);
                                                             }
#line 1001 "parser.tab.cc" // lalr1.cc:859
    break;

  case 34:
#line 292 "parser.yy" // lalr1.cc:859
    {
                                                                std::list<ExprNode*> exprList;
                                                                exprList.push_back(yystack_[2].value.as< ExprNode* > ());
                                                                exprList.push_back(yystack_[0].value.as< ExprNode* > ());
                                                                yylhs.location.begin = yystack_[2].location.begin;
                                                                yylhs.location.end = yystack_[0].location.end;
                                                                yylhs.value.as< ExprNode* > () = new BinOpExpr ("and", yystack_[2].value.as< ExprNode* > (), yystack_[0].value.as< ExprNode* > (), yylhs.location);
                                                             }
#line 1014 "parser.tab.cc" // lalr1.cc:859
    break;

  case 35:
#line 300 "parser.yy" // lalr1.cc:859
    {
                                                                std::list<ExprNode*> exprList;
                                                                exprList.push_back(yystack_[2].value.as< ExprNode* > ());
                                                                exprList.push_back(yystack_[0].value.as< ExprNode* > ());
                                                                yylhs.location.begin = yystack_[2].location.begin;
                                                                yylhs.location.end = yystack_[0].location.end;
                                                                yylhs.value.as< ExprNode* > () = new BinOpExpr ("=", yystack_[2].value.as< ExprNode* > (), yystack_[0].value.as< ExprNode* > (), yylhs.location);
                                                             }
#line 1027 "parser.tab.cc" // lalr1.cc:859
    break;

  case 36:
#line 308 "parser.yy" // lalr1.cc:859
    {
                                                                std::list<ExprNode*> exprList;
                                                                exprList.push_back(yystack_[2].value.as< ExprNode* > ());
                                                                exprList.push_back(yystack_[0].value.as< ExprNode* > ());
                                                                yylhs.location.begin = yystack_[2].location.begin;
                                                                yylhs.location.end = yystack_[0].location.end;
                                                                yylhs.value.as< ExprNode* > () = new BinOpExpr ("<", yystack_[2].value.as< ExprNode* > (), yystack_[0].value.as< ExprNode* > (), yylhs.location);
                                                             }
#line 1040 "parser.tab.cc" // lalr1.cc:859
    break;

  case 37:
#line 316 "parser.yy" // lalr1.cc:859
    {
                                                                std::list<ExprNode*> exprList;
                                                                exprList.push_back(yystack_[2].value.as< ExprNode* > ());
                                                                exprList.push_back(yystack_[0].value.as< ExprNode* > ());
                                                                yylhs.location.begin = yystack_[2].location.begin;
                                                                yylhs.location.end = yystack_[0].location.end;
                                                                yylhs.value.as< ExprNode* > () = new BinOpExpr ("<=", yystack_[2].value.as< ExprNode* > (), yystack_[0].value.as< ExprNode* > (), yylhs.location);
                                                             }
#line 1053 "parser.tab.cc" // lalr1.cc:859
    break;

  case 38:
#line 324 "parser.yy" // lalr1.cc:859
    {
                                                                std::list<ExprNode*> exprList;
                                                                exprList.push_back(yystack_[2].value.as< ExprNode* > ());
                                                                exprList.push_back(yystack_[0].value.as< ExprNode* > ());
                                                                yylhs.location.begin = yystack_[2].location.begin;
                                                                yylhs.location.end = yystack_[0].location.end;
                                                                yylhs.value.as< ExprNode* > () = new BinOpExpr ("+", yystack_[2].value.as< ExprNode* > (), yystack_[0].value.as< ExprNode* > (), yylhs.location);
                                                             }
#line 1066 "parser.tab.cc" // lalr1.cc:859
    break;

  case 39:
#line 332 "parser.yy" // lalr1.cc:859
    {
                                                                std::list<ExprNode*> exprList;
                                                                exprList.push_back(yystack_[2].value.as< ExprNode* > ());
                                                                exprList.push_back(yystack_[0].value.as< ExprNode* > ());
                                                                yylhs.location.begin = yystack_[2].location.begin;
                                                                yylhs.location.end = yystack_[0].location.end;
                                                                yylhs.value.as< ExprNode* > () = new BinOpExpr ("-", yystack_[2].value.as< ExprNode* > (), yystack_[0].value.as< ExprNode* > (), yylhs.location);
                                                             }
#line 1079 "parser.tab.cc" // lalr1.cc:859
    break;

  case 40:
#line 340 "parser.yy" // lalr1.cc:859
    {
                                                                std::list<ExprNode*> exprList;
                                                                exprList.push_back(yystack_[2].value.as< ExprNode* > ());
                                                                exprList.push_back(yystack_[0].value.as< ExprNode* > ());
                                                                yylhs.location.begin = yystack_[2].location.begin;
                                                                yylhs.location.end = yystack_[0].location.end;
                                                                yylhs.value.as< ExprNode* > () = new BinOpExpr ("*", yystack_[2].value.as< ExprNode* > (), yystack_[0].value.as< ExprNode* > (), yylhs.location);
                                                             }
#line 1092 "parser.tab.cc" // lalr1.cc:859
    break;

  case 41:
#line 348 "parser.yy" // lalr1.cc:859
    {
                                                                std::list<ExprNode*> exprList;
                                                                exprList.push_back(yystack_[2].value.as< ExprNode* > ());
                                                                exprList.push_back(yystack_[0].value.as< ExprNode* > ());
                                                                yylhs.location.begin = yystack_[2].location.begin;
                                                                yylhs.location.end = yystack_[0].location.end;
                                                                yylhs.value.as< ExprNode* > () = new BinOpExpr ("/", yystack_[2].value.as< ExprNode* > (), yystack_[0].value.as< ExprNode* > (), yylhs.location);
                                                             }
#line 1105 "parser.tab.cc" // lalr1.cc:859
    break;

  case 42:
#line 356 "parser.yy" // lalr1.cc:859
    {
                                                                std::list<ExprNode*> exprList;
                                                                exprList.push_back(yystack_[2].value.as< ExprNode* > ());
                                                                exprList.push_back(yystack_[0].value.as< ExprNode* > ());
                                                                yylhs.location.begin = yystack_[2].location.begin;
                                                                yylhs.location.end = yystack_[0].location.end;
                                                                yylhs.value.as< ExprNode* > () = new BinOpExpr ("^", yystack_[2].value.as< ExprNode* > (), yystack_[0].value.as< ExprNode* > (), yylhs.location);
                                                             }
#line 1118 "parser.tab.cc" // lalr1.cc:859
    break;

  case 43:
#line 364 "parser.yy" // lalr1.cc:859
    {
                                                                yylhs.location.begin = yystack_[1].location.begin;
                                                                yylhs.location.end = yystack_[0].location.end;
                                                                yylhs.value.as< ExprNode* > () = new UnOpExpr ("-", yystack_[0].value.as< ExprNode* > (), yylhs.location);
                                                             }
#line 1128 "parser.tab.cc" // lalr1.cc:859
    break;

  case 44:
#line 369 "parser.yy" // lalr1.cc:859
    {
                                                                yylhs.location.begin = yystack_[1].location.begin;
                                                                yylhs.location.end = yystack_[0].location.end;
                                                                yylhs.value.as< ExprNode* > () = new UnOpExpr ("isnull", yystack_[0].value.as< ExprNode* > (), yylhs.location);
                                                             }
#line 1138 "parser.tab.cc" // lalr1.cc:859
    break;

  case 45:
#line 374 "parser.yy" // lalr1.cc:859
    {
                                                                std::list<ExprNode*> exprList;
                                                                yylhs.location.begin = yystack_[1].location.begin;
                                                                yylhs.location.end = yystack_[0].location.end;
                                                                yylhs.value.as< ExprNode* > () = new NewExpr (yystack_[0].value.as< std::string > (), yylhs.location);
                                                             }
#line 1149 "parser.tab.cc" // lalr1.cc:859
    break;

  case 46:
#line 380 "parser.yy" // lalr1.cc:859
    {
                                                                yylhs.location.begin = yystack_[0].location.begin;
                                                                yylhs.location.end = yystack_[0].location.end;
                                                                yylhs.value.as< ExprNode* > () = new ValueExpr(yystack_[0].value.as< std::string > (), ValueExpr::UNKNOWN, yylhs.location, false);
                                                             }
#line 1159 "parser.tab.cc" // lalr1.cc:859
    break;

  case 47:
#line 385 "parser.yy" // lalr1.cc:859
    {
                                                                yylhs.value.as< ExprNode* > () = yystack_[0].value.as< ValueExpr* > ();
                                                             }
#line 1167 "parser.tab.cc" // lalr1.cc:859
    break;

  case 48:
#line 388 "parser.yy" // lalr1.cc:859
    {
                                                                yylhs.location.begin = yystack_[1].location.begin;
                                                                yylhs.location.end = yystack_[0].location.end;
                                                                yylhs.value.as< ExprNode* > () = new ValueExpr("()", ValueExpr::UNIT, yylhs.location);
                                                             }
#line 1177 "parser.tab.cc" // lalr1.cc:859
    break;

  case 49:
#line 393 "parser.yy" // lalr1.cc:859
    {
                                                                yylhs.value.as< ExprNode* > () = yystack_[1].value.as< ExprNode* > ();
                                                             }
#line 1185 "parser.tab.cc" // lalr1.cc:859
    break;

  case 50:
#line 396 "parser.yy" // lalr1.cc:859
    {
                                                                yylhs.value.as< ExprNode* > () = yystack_[0].value.as< BlockExpr* > ();
                                                             }
#line 1193 "parser.tab.cc" // lalr1.cc:859
    break;

  case 51:
#line 399 "parser.yy" // lalr1.cc:859
    {
                                                                yylhs.location.begin = yystack_[3].location.begin;
                                                                yylhs.location.end = yystack_[0].location.end;
                                                                yylhs.value.as< ExprNode* > () = new CallExpr(new ValueExpr("self", ValueExpr::UNKNOWN, yystack_[3].location, false), yystack_[3].value.as< std::string > (), yystack_[1].value.as< std::list<ExprNode*> > (), yylhs.location);
                                                             }
#line 1203 "parser.tab.cc" // lalr1.cc:859
    break;

  case 52:
#line 404 "parser.yy" // lalr1.cc:859
    {
                                                                yylhs.location.begin = yystack_[5].location.begin;
                                                                yylhs.location.end = yystack_[0].location.end;
                                                                yylhs.value.as< ExprNode* > () = new CallExpr(yystack_[5].value.as< ExprNode* > (), yystack_[3].value.as< std::string > (), yystack_[1].value.as< std::list<ExprNode*> > (), yylhs.location);
                                                             }
#line 1213 "parser.tab.cc" // lalr1.cc:859
    break;

  case 53:
#line 412 "parser.yy" // lalr1.cc:859
    {
                                                                yylhs.value.as< ExprNode* > () = new IfExpr(yystack_[2].value.as< ExprNode* > (), yystack_[0].value.as< ExprNode* > (), NULL, yylhs.location);
                                                             }
#line 1221 "parser.tab.cc" // lalr1.cc:859
    break;

  case 54:
#line 415 "parser.yy" // lalr1.cc:859
    {
                                                                yylhs.value.as< ExprNode* > () = new IfExpr(yystack_[4].value.as< ExprNode* > (), yystack_[2].value.as< ExprNode* > (), yystack_[0].value.as< ExprNode* > (), yylhs.location);
                                                             }
#line 1229 "parser.tab.cc" // lalr1.cc:859
    break;

  case 55:
#line 421 "parser.yy" // lalr1.cc:859
    {
                                                                std::list<ExprNode*> exprList;
                                                                yylhs.value.as< std::list<ExprNode*> > () = exprList;
                                                             }
#line 1238 "parser.tab.cc" // lalr1.cc:859
    break;

  case 56:
#line 425 "parser.yy" // lalr1.cc:859
    {
                                                                yylhs.value.as< std::list<ExprNode*> > () = yystack_[0].value.as< std::list<ExprNode*> > ();
                                                             }
#line 1246 "parser.tab.cc" // lalr1.cc:859
    break;

  case 57:
#line 431 "parser.yy" // lalr1.cc:859
    {
                                                                std::list<ExprNode*> exprList;
                                                                exprList.push_back(yystack_[0].value.as< ExprNode* > ());
                                                                yylhs.value.as< std::list<ExprNode*> > () = exprList;
                                                             }
#line 1256 "parser.tab.cc" // lalr1.cc:859
    break;

  case 58:
#line 436 "parser.yy" // lalr1.cc:859
    {
                                                                yystack_[2].value.as< std::list<ExprNode*> > ().push_back(yystack_[0].value.as< ExprNode* > ());
                                                                yylhs.value.as< std::list<ExprNode*> > () = yystack_[2].value.as< std::list<ExprNode*> > ();
                                                             }
#line 1265 "parser.tab.cc" // lalr1.cc:859
    break;

  case 59:
#line 444 "parser.yy" // lalr1.cc:859
    {
                                                                yylhs.location.begin = yystack_[0].location.begin;
                                                                yylhs.location.end = yystack_[0].location.end;
                                                                yylhs.value.as< ValueExpr* > () = new ValueExpr(std::to_string(yystack_[0].value.as< int > ()), ValueExpr::INT32, yylhs.location);
                                                             }
#line 1275 "parser.tab.cc" // lalr1.cc:859
    break;

  case 60:
#line 449 "parser.yy" // lalr1.cc:859
    {
                                                                yylhs.location.begin = yystack_[0].location.begin;
                                                                yylhs.location.end = yystack_[0].location.end;
                                                                yylhs.value.as< ValueExpr* > () = new ValueExpr(yystack_[0].value.as< std::string > (), ValueExpr::STRING, yylhs.location);
                                                             }
#line 1285 "parser.tab.cc" // lalr1.cc:859
    break;

  case 61:
#line 454 "parser.yy" // lalr1.cc:859
    {
                                                                yylhs.value.as< ValueExpr* > () = yystack_[0].value.as< ValueExpr* > ();
                                                             }
#line 1293 "parser.tab.cc" // lalr1.cc:859
    break;

  case 62:
#line 460 "parser.yy" // lalr1.cc:859
    {
                                                                yylhs.location.begin = yystack_[0].location.begin;
                                                                yylhs.location.end = yystack_[0].location.end;
                                                                yylhs.value.as< ValueExpr* > () = new ValueExpr("true", ValueExpr::BOOL, yylhs.location);
                                                             }
#line 1303 "parser.tab.cc" // lalr1.cc:859
    break;

  case 63:
#line 465 "parser.yy" // lalr1.cc:859
    {
                                                                yylhs.location.begin = yystack_[0].location.begin;
                                                                yylhs.location.end = yystack_[0].location.end;
                                                                yylhs.value.as< ValueExpr* > () = new ValueExpr("false", ValueExpr::BOOL, yylhs.location);
                                                             }
#line 1313 "parser.tab.cc" // lalr1.cc:859
    break;


#line 1317 "parser.tab.cc" // lalr1.cc:859
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
    }
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (!yyla.empty ())
      {
        int yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char parser::yypact_ninf_ = -33;

  const signed char parser::yytable_ninf_ = -5;

  const short int
  parser::yypact_[] =
  {
     -33,    17,    50,     1,    43,   -33,    45,    52,    40,   -33,
     -33,   -33,    -1,    -2,   -33,   -33,   -33,    54,     6,    37,
      39,    42,   -33,   -33,   -33,   -33,   -33,   -33,    34,     6,
      47,    54,    86,    41,   -33,     6,   -33,   -33,   -33,   -20,
     -33,    86,    65,   -33,    86,    69,    86,    62,    86,    86,
      86,   -33,   137,   -33,   -33,   -33,   -33,    57,    86,    86,
     109,   -33,   -18,   137,    55,    -5,   -33,    96,   148,    -7,
      -7,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      81,   -33,    64,    66,   137,   137,    86,   -33,    86,     6,
      86,   -33,   148,   159,   159,   159,    10,    10,    -7,    -7,
      -7,    68,   -33,    86,   124,   137,    34,   137,    86,   137,
      86,    82,    72,   137,    86,   -33,   137
  };

  const unsigned char
  parser::yydefact_[] =
  {
       3,     0,     0,     0,     2,     1,     6,     0,     0,     7,
       9,     5,     0,     0,     8,    10,    11,    21,     0,     0,
       0,    22,    23,    16,    18,    17,    19,    20,    13,     0,
       0,     0,     0,     0,    25,     0,    24,    59,    60,    46,
      63,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,    50,    14,    29,    47,    61,    12,     0,    55,     0,
       0,    45,     0,    27,     0,     0,    48,     0,    33,    43,
      44,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    15,     0,    56,    57,    32,     0,    26,     0,     0,
       0,    49,    34,    36,    37,    35,    38,    39,    40,    41,
      42,     0,    51,     0,    53,    28,    13,    30,    55,    58,
       0,     0,     0,    54,     0,    52,    31
  };

  const signed char
  parser::yypgoto_[] =
  {
     -33,   -33,   -33,   -33,   -33,   -33,   -33,   -33,   -33,    73,
      -6,    49,   -33,     2,   -33,   -32,   -33,   -33,   -33,   -33,
     -28,   -33
  };

  const signed char
  parser::yydefgoto_[] =
  {
      -1,     1,    11,    12,    20,    21,     4,    15,    16,    22,
      33,    51,    62,    82,    83,    84,    53,    54,    55,     8,
      28,     2
  };

  const signed char
  parser::yytable_[] =
  {
      52,    34,    87,    58,    90,    13,     6,    57,    88,    60,
      59,    23,    63,    24,    65,    67,    68,    69,    70,    14,
      25,    17,    26,    18,    27,     3,    71,    85,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    79,    80,    92,
      93,    94,    95,    96,    97,    98,    99,   100,    77,    78,
       5,    -4,    79,    80,   104,     7,   105,     9,   107,    10,
      19,   106,    29,    30,    32,    37,    38,    56,    39,    31,
      61,   109,    35,    40,    41,    64,    44,    42,   113,    43,
      89,    44,   116,    45,    46,    47,    66,   101,   102,    37,
      38,   108,    39,   103,    48,   114,   115,    40,    41,    49,
     111,    42,    50,    43,    36,    44,    81,    45,    46,    47,
     112,     0,     0,     0,     0,     0,     0,     0,    48,     0,
      91,     0,     0,    49,     0,     0,    50,    71,     0,    72,
      73,    74,    75,    76,    77,    78,     0,    86,    79,    80,
      71,     0,    72,    73,    74,    75,    76,    77,    78,     0,
       0,    79,    80,   110,     0,    71,     0,    72,    73,    74,
      75,    76,    77,    78,     0,     0,    79,    80,    71,     0,
      72,    73,    74,    75,    76,    77,    78,     0,     0,    79,
      80,    72,    73,    74,    75,    76,    77,    78,     0,     0,
      79,    80,    -5,    -5,    -5,    75,    76,    77,    78,     0,
       0,    79,    80
  };

  const signed char
  parser::yycheck_[] =
  {
      32,    29,    20,    23,     9,     6,     5,    35,    26,    41,
      30,     5,    44,     7,    46,    47,    48,    49,    50,    20,
      14,    23,    16,    25,    18,     8,    31,    59,    33,    34,
      35,    36,    37,    38,    39,    42,    43,    42,    43,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    38,    39,
       0,     8,    42,    43,    86,    10,    88,     5,    90,    19,
       6,    89,    25,    24,    30,     3,     4,    26,     6,    27,
       5,   103,    25,    11,    12,     6,    19,    15,   110,    17,
      25,    19,   114,    21,    22,    23,    24,     6,    24,     3,
       4,    23,     6,    27,    32,    13,    24,    11,    12,    37,
     106,    15,    40,    17,    31,    19,    57,    21,    22,    23,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      24,    -1,    -1,    37,    -1,    -1,    40,    31,    -1,    33,
      34,    35,    36,    37,    38,    39,    -1,    28,    42,    43,
      31,    -1,    33,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    43,    29,    -1,    31,    -1,    33,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    43,    31,    -1,
      33,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      43,    33,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    43,    33,    34,    35,    36,    37,    38,    39,    -1,
      -1,    42,    43
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,    45,    65,     8,    50,     0,     5,    10,    63,     5,
      19,    46,    47,     6,    20,    51,    52,    23,    25,     6,
      48,    49,    53,     5,     7,    14,    16,    18,    64,    25,
      24,    27,    30,    54,    64,    25,    53,     3,     4,     6,
      11,    12,    15,    17,    19,    21,    22,    23,    32,    37,
      40,    55,    59,    60,    61,    62,    26,    64,    23,    30,
      59,     5,    56,    59,     6,    59,    24,    59,    59,    59,
      59,    31,    33,    34,    35,    36,    37,    38,    39,    42,
      43,    55,    57,    58,    59,    59,    28,    20,    26,    25,
       9,    24,    59,    59,    59,    59,    59,    59,    59,    59,
      59,     6,    24,    27,    59,    59,    64,    59,    23,    59,
      29,    54,    57,    59,    13,    24,    59
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    44,    65,    45,    45,    50,    63,    63,    46,    47,
      47,    47,    51,    54,    54,    52,    64,    64,    64,    64,
      64,    48,    48,    49,    49,    53,    55,    56,    56,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    60,    60,    57,    57,    58,    58,    61,
      61,    61,    62,    62
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     2,     0,     2,     4,     0,     2,     3,     0,
       2,     2,     5,     0,     2,     7,     1,     1,     1,     1,
       1,     0,     1,     1,     3,     3,     3,     1,     3,     1,
       4,     7,     3,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     2,     1,     1,     2,     3,
       1,     4,     6,     4,     6,     0,     1,     1,     3,     1,
       1,     1,     1,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "INTEGER_LITERAL",
  "STRING_LITERAL", "TYPE_ID", "OBJECT_ID", "BOOL", "CLASS", "DO",
  "EXTENDS", "FALSE", "IF", "IN", "INT32", "NEW", "STRING", "TRUE", "UNIT",
  "LBRACE", "RBRACE", "LET", "WHILE", "LPAR", "RPAR", "COLON", "SEMICOLON",
  "COMMA", "THEN", "ELSE", "ASSIGN", "AND", "NOT", "LOWER", "LOWER_EQUAL",
  "EQUAL", "PLUS", "MINUS", "TIMES", "DIV", "ISNULL", "NEG", "POW", "DOT",
  "$accept", "classes", "class-body", "class-body-part", "formals",
  "formals-list", "class", "field", "method", "formal", "init.opt",
  "block", "expr-list-semicolon", "args", "expr-list-comma", "expr",
  "if-stmt", "literal", "boolean-literal", "extension.opt", "type",
  "program", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  parser::yyrline_[] =
  {
       0,   112,   112,   139,   143,   151,   159,   162,   168,   174,
     177,   181,   188,   196,   199,   205,   212,   213,   214,   215,
     216,   220,   224,   230,   235,   242,   250,   257,   262,   270,
     271,   276,   282,   287,   292,   300,   308,   316,   324,   332,
     340,   348,   356,   364,   369,   374,   380,   385,   388,   393,
     396,   399,   404,   412,   415,   421,   425,   431,   436,   444,
     449,   454,   460,   465
  };

  // Print the state stack on the debug stream.
  void
  parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG



} // yy
#line 1784 "parser.tab.cc" // lalr1.cc:1167
#line 472 "parser.yy" // lalr1.cc:1168


void
yy::parser::error (const location_type& l, const std::string& m)
{
  drv.syntax_error = true;
  std::cerr << *l.begin.filename << ":" << l.begin.line << ":" << l.begin.column << ": " << m << '\n';

}
