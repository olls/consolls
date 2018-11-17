#include "compolls.h"

#include "tokeniser.h"
#include "array.h"
#include "stack.h"
#include "pair.h"


#define LENGTH(array) sizeof((array))/sizeof((array)[0])


namespace Compolls
{

namespace Parser
{

struct Symbol
{
  SymbolType type;
  Tokeniser::Token token;
};


struct Parser
{
  String::String text;
  Stack::Stack<Tokeniser::Token> tokens;

  Symbol symbol_a;
  Symbol symbol_b;

  u32 depth;
};


Symbol
token_to_symbol(String::String const & text, Tokeniser::Token token)
{
  Symbol result = {
    .type = SymbolType::Error,
    .token = token
  };

  String::String token_text = Tokeniser::string(text, token);

  static Pair<String::String, SymbolType> const string_to_symbol[] = {
    {S(" "), SymbolType::Whitespace},
    {S("\t"), SymbolType::Whitespace},
    {S("\n"), SymbolType::Whitespace},
    {S("("), SymbolType::L_Parenthesis},
    {S(")"), SymbolType::R_Parenthesis},
    {S("{"), SymbolType::L_Brace},
    {S("}"), SymbolType::R_Brace},
    {S("["), SymbolType::L_Bracket},
    {S("]"), SymbolType::R_Bracket},
    {S("="), SymbolType::Equals},
    {S(","), SymbolType::Comma}
  };

  for (u32 i = 0; i < LENGTH(string_to_symbol); ++i)
  {
    if (String::equals(token_text, string_to_symbol[i].first))
    {
      result.type = string_to_symbol[i].second;

      break;
    }
  }

  if (result.type == SymbolType::Error)
  {
    if (String::all(token_text, String::is_num))
    {
      result.type = SymbolType::Number;
    }
    else if (String::all(token_text, String::is_alpha_num))
    {
      result.type = SymbolType::Identifier;
    }
  }

  if (result.type != SymbolType::Error)
  {
    result.token = token;
  }

  return result;
}


Symbol
advance_terminal(Parser& parser)
{
  Symbol result = {
    .type = SymbolType::Error,
    .token = {}
  };

  if (!Stack::empty(parser.tokens))
  {
    Tokeniser::Token token = Stack::pop(parser.tokens);
    result = token_to_symbol(parser.text, token);
  }

  return result;
}


String::String
symbol_string(SymbolType symbol)
{
  String::String result = {};

  switch (symbol)
  {
    case (SymbolType::Error):
    {
      result.start = "Error";
    } break;
    case (SymbolType::Whitespace):
    {
      result.start = "Whitespace";
    } break;
    case (SymbolType::L_Parenthesis):
    {
      result.start = "L_Parenthesis";
    } break;
    case (SymbolType::R_Parenthesis):
    {
      result.start = "R_Parenthesis";
    } break;
    case (SymbolType::L_Brace):
    {
      result.start = "L_Brace";
    } break;
    case (SymbolType::R_Brace):
    {
      result.start = "R_Braces";
    } break;
    case (SymbolType::L_Bracket):
    {
      result.start = "L_Bracket";
    } break;
    case (SymbolType::R_Bracket):
    {
      result.start = "R_Bracket";
    } break;
    case (SymbolType::Equals):
    {
      result.start = "Equals";
    } break;
    case (SymbolType::Comma):
    {
      result.start = "Comma";
    } break;
    case (SymbolType::Identifier):
    {
      result.start = "Identifier";
    } break;
    case (SymbolType::Number):
    {
      result.start = "Number";
    } break;
  }

  result.length = strlen(result.start);

  return result;
}


template <SymbolType terminal_symbol>
bool
terminal(Parser& parser)
{
  bool result = false;

  printf("%*sWant %.*s got %.*s(%.*s)\n", parser.depth*2, "",
         print_s(symbol_string(terminal_symbol)),
         print_s(symbol_string(parser.symbol_a.type)), print_s(Tokeniser::string(parser.text, parser.symbol_a.token)));
  if (parser.symbol_a.type == terminal_symbol)
  {
    parser.symbol_a = parser.symbol_b;
    parser.symbol_b = advance_terminal(parser);
    result = true;
  }

  assert(parser.depth < 20);
  return result;
}


template <SymbolType terminal_symbol_a, SymbolType terminal_symbol_b>
bool
terminal(Parser& parser)
{
  bool result = false;

  printf("%*sWant %.*s %.*s got %.*s(%.*s) %.*s(%.*s)\n", parser.depth*2, "",
         print_s(symbol_string(terminal_symbol_a)),
         print_s(symbol_string(terminal_symbol_b)),
         print_s(symbol_string(parser.symbol_a.type)), print_s(Tokeniser::string(parser.text, parser.symbol_a.token)),
         print_s(symbol_string(parser.symbol_b.type)), print_s(Tokeniser::string(parser.text, parser.symbol_b.token)));
  if (parser.symbol_a.type == terminal_symbol_a &&
      parser.symbol_b.type == terminal_symbol_b)
  {
    parser.symbol_a = advance_terminal(parser);
    parser.symbol_b = advance_terminal(parser);
    result = true;
  }

  assert(parser.depth < 20);
  return result;
}


using Production = bool (*)(Parser&);

bool
accept_sequence(Parser& parser, u32 length, Production productions[])
{
  bool success = true;

  for (u32 production_index = 0;
       production_index < length;
       ++production_index)
  {
    Production production = productions[production_index];

    success &= production(parser);

    if (!success)
    {
      break;
    }
  }

  return success;
}


bool
declarations(Parser& parser);

bool
body(Parser& parser);


bool
function(Parser& parser)
{
  printf("%*sfunction{\n", parser.depth*2, "");
  parser.depth += 1;

  // function <- '[' identifier ']' '(' declarations ')' '{' body '}'

  bool matches = terminal<SymbolType::L_Bracket>(parser);

  if (matches)
  {
    Production sequence[] = {
      terminal<SymbolType::Identifier>,
      terminal<SymbolType::R_Bracket>,
      terminal<SymbolType::L_Parenthesis>,
      declarations,
      terminal<SymbolType::R_Parenthesis>,
      terminal<SymbolType::L_Brace>,
      body,
      terminal<SymbolType::R_Brace>
    };

    if (!accept_sequence(parser, LENGTH(sequence), sequence))
    {
      assert(0 && "error");
    }
  }

  parser.depth -= 1;
  printf("%*s} %s\n", parser.depth*2, "", matches ? "true" : "false");

  return matches;
}


bool
literal(Parser& parser)
{
  printf("%*sliteral{\n", parser.depth*2, "");
  parser.depth += 1;

  // literal <- NUMBER
  //          | function

  bool matches = (terminal<SymbolType::Number>(parser) ||
                  function(parser));

  parser.depth -= 1;
  printf("%*s} %s\n", parser.depth*2, "", matches ? "true" : "false");

  return matches;
}


bool
expressions(Parser& parser);


bool
function_call(Parser& parser)
{
  printf("%*sfunction_call{\n", parser.depth*2, "");
  parser.depth += 1;

  // function_call <- IDENTIFIER '(' expressions ')'

  bool matches = terminal<SymbolType::Identifier, SymbolType::L_Parenthesis>(parser);

  if (matches)
  {
    Production productions[] = {
      expressions,
      terminal<SymbolType::R_Parenthesis>
    };

    if (!accept_sequence(parser, LENGTH(productions), productions))
    {
      assert(0 && "error");
    }
  }

  parser.depth -= 1;
  printf("%*s} %s\n", parser.depth*2, "", matches ? "true" : "false");
  return matches;
}


bool
expression(Parser& parser)
{
  printf("%*sexpression{\n", parser.depth*2, "");
  parser.depth += 1;

  // expression <- literal |
  //               function_call |
  //               IDENTIFIER

  bool matches = (literal(parser) ||
                  function_call(parser) ||
                  terminal<SymbolType::Identifier>(parser));

  parser.depth -= 1;
  printf("%*s} %s\n", parser.depth*2, "", matches ? "true" : "false");

  return matches;
}


bool
expressions(Parser& parser)
{
  printf("%*sexpressions{\n", parser.depth*2, "");
  parser.depth += 1;

  // expressions <- expression { ',' expressions }
  //              | .

  bool matches = true;

  if (expression(parser))
  {
    while (terminal<SymbolType::Comma>(parser))
    {
      if (!expression(parser))
      {
        assert(0 && "error");
      }
    }
  }

  parser.depth -= 1;
  printf("%*s} %s\n", parser.depth*2, "", matches ? "true" : "false");

  return matches;
}


bool
declaration(Parser& parser)
{
  printf("%*sdeclaration{\n", parser.depth*2, "");
  parser.depth += 1;

  // declaration <- IDENTIFIER IDENTIFIER

  bool matches = terminal<SymbolType::Identifier, SymbolType::Identifier>(parser);

  parser.depth -= 1;
  printf("%*s} %s\n", parser.depth*2, "", matches ? "true" : "false");

  return matches;
}


bool
declarations(Parser& parser)
{
  printf("%*sdeclarations{\n", parser.depth*2, "");
  parser.depth += 1;

  // declarations <- declaration { ',' declaration }
  //               | .

  bool matches = true;

  if (declaration(parser))
  {
    while (terminal<SymbolType::Comma>(parser))
    {
      if (!declaration(parser))
      {
        assert(0 && "error");
      }
    }
  }

  parser.depth -= 1;
  printf("%*s} %s\n", parser.depth*2, "", matches ? "true" : "false");

  return matches;
}


bool
assignment(Parser& parser)
{
  printf("%*sassignment{\n", parser.depth*2, "");
  parser.depth += 1;

  // assignment <- declaration '=' expression

  bool matches = declaration(parser);

  if (matches)
  {
    Production productions[] = {
      terminal<SymbolType::Equals>,
      expression
    };

    if (!accept_sequence(parser, LENGTH(productions), productions))
    {
      assert(0 && "error");
    }
  }

  parser.depth -= 1;
  printf("%*s} %s\n", parser.depth*2, "", matches ? "true" : "false");

  return matches;
}


bool
statement(Parser& parser)
{
  printf("%*sstatement{\n", parser.depth*2, "");
  parser.depth += 1;

  // statement <- assignment |
  //              expression

  bool matches = (assignment(parser) ||
                  expression(parser));

  parser.depth -= 1;
  printf("%*s} %s\n", parser.depth*2, "", matches ? "true" : "false");

  return matches;
}


bool
body(Parser& parser)
{
  printf("%*sbody{\n", parser.depth*2, "");
  parser.depth += 1;

  // body <- { statement }

  bool matches = true;

  while (statement(parser))
  {}

  parser.depth -= 1;
  printf("%*s} %s\n", parser.depth*2, "", matches ? "true" : "false");

  return matches;
}


bool
program(Parser& parser)
{
  printf("%*sprogram{\n", parser.depth*2, "");
  parser.depth += 1;

  // program <- body

  bool matches = body(parser);

  parser.depth -= 1;
  printf("%*s} %s\n", parser.depth*2, "", matches ? "true" : "false");

  return matches;
}


//
// # Context Free Grammar: EBNF
//
// - UPPER CASE and 'things in quotes' are terminals, which is what the input token array is composed of.
// - `|` = alternative match
// - `.` = the production matches against nothing
// - `{}` = matched zero or more times
//
//
// program <- body
//
// body <- { statement }
//
// statement <- assignment
//            | expression
//
// assignment <- declaration '=' expression
//
// declaration <- IDENTIFIER IDENTIFIER
//
// expression <- literal
//             | function_call
//             | IDENTIFIER
//
// literal <- NUMBER
//          | function
//
// function_call <- IDENTIFIER '(' expressions ')'
//
// function <- '[' IDENTIFIER ']' '(' declarations ')' '{' body '}'
//
// expressions <- expression { ',' expressions }
//              | .
//
// declarations <- declaration { ',' declaration }
//               | .
//

} // namespace Parser


Basolls::Subroutine<void>
compile(String::String text)
{
  Basolls::Subroutine<void> result = {};

  printf("Compiling:  \"%.*s\"\n\n", text.length, text.start);

  Array::Array<Tokeniser::Token> tokens = Tokeniser::tokenise(text);

  for (u32 token_index = 0;
       token_index < tokens.n_elements;
       ++token_index)
  {
    printf("\"%.*s\", ", print_s(Tokeniser::string(text, tokens[token_index])));
  }
  printf("\n");

  Parser::Parser parser = {};
  parser.tokens.elements = tokens;
  parser.text = text;

  parser.symbol_a = advance_terminal(parser);
  parser.symbol_b = advance_terminal(parser);

  bool success = true;

  success &= Parser::program(parser);

  assert(success);

  return result;
}

} // namespace Compolls
