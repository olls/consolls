#include "parser.h"

#include "pair.h"
#include "allocate.h"

#define LENGTH(array) sizeof((array))/sizeof((array)[0])


namespace Compolls
{

namespace Parser
{

#define PARSE_DEBUG_TRACE (0)


// TODO: We need a way to return from a production with an error instead of a not-matching


Symbol
token_to_symbol(Strings::Table& strings, Tokeniser::Token token)
{
  // TODO: This should really just be done in the tokeniser, then we can do away with the lookahead
  Symbol result = {
    .type = SymbolType::Error,
    .token = token
  };

  if (Strings::add(strings, "(") == token.string)
  {
    result.type = SymbolType::L_Parenthesis;
  }
  else if (Strings::add(strings, ")") == token.string)
  {
    result.type = SymbolType::R_Parenthesis;
  }
  else if (Strings::add(strings, "{") == token.string)
  {
    result.type = SymbolType::L_Brace;
  }
  else if (Strings::add(strings, "}") == token.string)
  {
    result.type = SymbolType::R_Brace;
  }
  else if (Strings::add(strings, "[") == token.string)
  {
    result.type = SymbolType::L_Bracket;
  }
  else if (Strings::add(strings, "]") == token.string)
  {
    result.type = SymbolType::R_Bracket;
  }
  else if (Strings::add(strings, "=") == token.string)
  {
    result.type = SymbolType::Equals;
  }
  else if (Strings::add(strings, ",") == token.string)
  {
    result.type = SymbolType::Comma;
  }
  else if (Strings::add(strings, "Func") == token.string)
  {
    result.type = SymbolType::Func;
  }

  if (result.type == SymbolType::Error)
  {
    String::String token_text = Strings::get(strings, token.string);

    if (String::all(token_text, String::is_num))
    {
      result.type = SymbolType::Number;
    }
    else if (String::all(token_text, String::is_alpha_num_underscore))
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

  if (!Fifo::empty(parser.tokens))
  {
    Tokeniser::Token token = Fifo::pop(parser.tokens);
    result = token_to_symbol(*parser.strings, token);
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
    case (SymbolType::Func):
    {
      result.start = "Func";
    } break;
    case (SymbolType::Number):
    {
      result.start = "Number";
    } break;
  }

  result.length = strlen(result.start);

  return result;
}


template <SymbolType... t_symbols>
bool
terminal(Parser& parser, Tree::Node** o_token_nodes[sizeof...(t_symbols)])
{
  static u32 const n_symbols = sizeof...(t_symbols);
  static SymbolType const symbols[n_symbols] = {t_symbols...};

  static_assert(n_symbols <= Parser::SymbolLookaheadType::lookahead_n);

  // Test all symbols match the lookahead

  bool matches = true;

  for (u32 i = 0; i < n_symbols; ++i)
  {
    SymbolType symbol = symbols[i];
    // printf("Matching terminal %.*s\n", print_s(symbol_string(symbol)));
    if (parser.lookahead.symbols[i].type != symbol)
    {
      matches = false;
      break;
    }
  }

  if (matches)
  {
    // Assign symbols to output

    for (u32 i = 0; i < n_symbols; ++i)
    {
      Tree::Node** o_token_node = o_token_nodes[i];

      if (o_token_node)
      {
        *o_token_node = Allocate::allocate<Tree::Node>();
        (*o_token_node)->type = Tree::Node::Terminal;
        (*o_token_node)->terminal.token = parser.lookahead.symbols[i].token;
        (*o_token_node)->text_start = parser.lookahead.symbols[i].token.start;
        (*o_token_node)->text_end = parser.lookahead.symbols[i].token.end;
      }
    }

    if (n_symbols > 0 && parser.lookahead.symbols[0].type != SymbolType::Error)
    {
      // Record the end text position of the last token
      // This is so we can easily get the end position of the last token we read at the end of a production
      parser.last_symbol_end_position = parser.lookahead.symbols[0].token.end;
    }

    // Advance lookahead

    for (u32 i = 0; i < parser.lookahead.lookahead_n; ++i)
    {
      if (i+n_symbols < parser.lookahead.lookahead_n)
      {
        parser.lookahead.symbols[i] = parser.lookahead.symbols[i+n_symbols];
      }
      else
      {
        parser.lookahead.symbols[i] = advance_terminal(parser);
      }
    }
  }

  assert(parser.depth < 50);
  return matches;
}


template <SymbolType terminal_symbol>
bool
terminal(Parser& parser, Tree::Node** o_token_node = NULL)
{
  return terminal<terminal_symbol>(parser, &o_token_node);
}


using Production = bool (*)(Parser&, Tree::Node**);

struct ProductionLambda
{
  Production production;
  Tree::Node** result = NULL;
};


bool
accept_sequence(Parser& parser, u32 length, ProductionLambda productions[])
{
  bool success = true;

  for (u32 production_index = 0;
       production_index < length;
       ++production_index)
  {
    ProductionLambda production = productions[production_index];

    success &= production.production(parser, production.result);

    if (!success)
    {
      break;
    }
  }

  return success;
}


bool
type_definition(Parser& parser, Tree::Node** result);

bool
function_signature(Parser& parser, Tree::Node** result);

bool
function(Parser& parser, Tree::Node** result);

bool
literal(Parser& parser, Tree::Node** result);

bool
function_call(Parser& parser, Tree::Node** result);

bool
expression(Parser& parser, Tree::Node** result);

bool
expressions(Parser& parser, Tree::Node** result);

bool
declaration(Parser& parser, Tree::Node** result);

bool
declarations(Parser& parser, Tree::Node** result);

bool
assignment(Parser& parser, Tree::Node** result);

bool
statement(Parser& parser, Tree::Node** result);

bool
body(Parser& parser, Tree::Node** result);

bool
program(Parser& parser, Tree::Node** result);


#define start_production_debug(parser) _start_production_debug(__FUNCTION__, (parser))

void
_start_production_debug(const char * production_name, Parser& parser)
{
#if PARSE_DEBUG_TRACE
  printf("%*s%s {\n", parser.depth*2, "", production_name);

  parser.depth += 1;

  printf("%*slkhd: ", parser.depth*2, "");
  print_lookahead(*parser.strings, parser.lookahead);
  printf("\n");
#endif
}


#define end_production_debug(parser, matches, node) _end_production_debug(__FUNCTION__, (parser), (matches), (node))
void
_end_production_debug(const char* production_name, Parser& parser, bool matches, Tree::Node const * node)
{
#if PARSE_DEBUG_TRACE
  printf("%*slkhd: ", parser.depth*2, "");
  print_lookahead(*parser.strings, parser.lookahead);
  printf("\n");

  parser.depth -= 1;

  printf("%*s} %s", parser.depth*2, "", production_name);
  if (node != NULL)
  {
    String::String string = Tokeniser::string(parser.text, {node->text_start, node->text_end});
    printf("  \"%.*s\"", print_s(string));
  }
  printf("  %s\n", matches ? "true" : "false");
#endif
}


u32
current_text_position(Parser const & parser)
{
  u32 result;

  if (parser.lookahead.symbols[0].type == SymbolType::Error)
  {
    result = parser.text.length;
  }
  else
  {
    result = parser.lookahead.symbols[0].token.start;
  }

  return result;
}

bool
type_name(Parser& parser, Tree::Node** result)
{
  start_production_debug(parser);

  // type_name <- `Func` function_signature
  //            | IDENTIFIER

  Tree::Node node = { .type = Tree::Node::TypeName, .text_start = current_text_position(parser) };

  bool matches = true;

  if (terminal<SymbolType::Func>(parser))
  {
    node.type_name.type = Tree::TypeNameNode::Type::FunctionSignature;

    matches &= function_signature(parser, &node.type_name.function_signature);

    assert(matches);
  }
  else
  {
    if (terminal<SymbolType::Identifier>(parser, &node.type_name.identifier))
    {
      node.type_name.type = Tree::TypeNameNode::Type::Identifier;
    }
    else
    {
      matches = false;
    }
  }

  node.text_end = parser.last_symbol_end_position;

  if (matches)
  {
    *result = Allocate::copy(node);
  }

  end_production_debug(parser, matches, *result);
  return matches;
}

bool
function_signature(Parser& parser, Tree::Node** result)
{
  start_production_debug(parser);

  // function_signature <- '[' type_name ']' '(' declarations ')'

  Tree::Node node = { .type = Tree::Node::FunctionSignature, .text_start = current_text_position(parser) };

  bool matches = false;
  if (terminal<SymbolType::L_Bracket>(parser))
  {
    matches = true;

    matches &= type_name(parser, &node.function_signature.return_type);
    assert(matches);
    Tree::Node** nodes[] = {NULL, NULL};
    matches &= terminal<SymbolType::R_Bracket, SymbolType::L_Parenthesis>(parser, nodes);
    assert(matches);

    ProductionLambda sequence[] = {
      {declarations, &node.function_signature.declarations},
      {terminal<SymbolType::R_Parenthesis>}
    };

    if (!accept_sequence(parser, LENGTH(sequence), sequence))
    {
      // TODO: More descriptive error message
      printf("Error while parsing function signature\n");
      matches &= false;
    }
  }

  node.text_end = parser.last_symbol_end_position;

  if (matches && result)
  {
    *result = Allocate::copy(node);
  }

  end_production_debug(parser, matches, *result);
  return matches;
}


bool
function(Parser& parser, Tree::Node** result)
{
  start_production_debug(parser);

  // function <- function_signature '{' body '}'

  Tree::Node node = { .type = Tree::Node::Function, .text_start = current_text_position(parser) };

  bool matches = function_signature(parser, &node.function.function_signature);

  if (matches)
  {
    ProductionLambda sequence[] = {
      {terminal<SymbolType::L_Brace>},
      {body, &node.function.body},
      {terminal<SymbolType::R_Brace>}
    };

    if (!accept_sequence(parser, LENGTH(sequence), sequence))
    {
      // TODO: More descriptive error message
      printf("Error while parsing function definition\n");
      matches &= false;
      assert(matches);
    }
  }

  node.text_end = parser.last_symbol_end_position;

  if (matches && result)
  {
    *result = Allocate::copy(node);
  }

  end_production_debug(parser, matches, *result);
  return matches;
}


bool
literal(Parser& parser, Tree::Node** result)
{
  start_production_debug(parser);

  // literal <- NUMBER
  //          | function

  Tree::Node node = { .type = Tree::Node::Literal, .text_start = current_text_position(parser) };

  bool matches = ((terminal<SymbolType::Number>(parser, &node.literal.number) ? (node.literal.type = Tree::LiteralNode::Type::Number), true : false) ||
                  (function(parser, &node.literal.function) ? (node.literal.type = Tree::LiteralNode::Type::Function), true : false));

  node.text_end = parser.last_symbol_end_position;

  if (matches && result)
  {
    *result = Allocate::copy(node);
  }

  end_production_debug(parser, matches, *result);
  return matches;
}


bool
function_call(Parser& parser, Tree::Node** result)
{
  start_production_debug(parser);

  // function_call <- IDENTIFIER '(' expressions ')'

  Tree::Node node = { .type = Tree::Node::FunctionCall, .text_start = current_text_position(parser) };

  Tree::Node** node_addrs[2] = {&node.function_call.label, NULL};
  bool matches = terminal<SymbolType::Identifier, SymbolType::L_Parenthesis>(parser, node_addrs);

  if (matches)
  {
    ProductionLambda productions[] = {
      {expressions, &node.function_call.expressions},
      {terminal<SymbolType::R_Parenthesis>}
    };

    if (!accept_sequence(parser, LENGTH(productions), productions))
    {
      assert(0 && "error");
    }
  }

  node.text_end = parser.last_symbol_end_position;

  if (matches && result)
  {
    *result = Allocate::copy(node);
  }

  end_production_debug(parser, matches, *result);
  return matches;
}


bool
expression(Parser& parser, Tree::Node** result)
{
  start_production_debug(parser);

  // expression <- literal |
  //               function_call |
  //               IDENTIFIER

  Tree::Node node = { .type = Tree::Node::Expression, .text_start = current_text_position(parser) };

  bool matches = ((literal(parser, &node.expression.literal) ? (node.expression.type = Tree::ExpressionNode::Type::Literal), true : false) ||
                  (function_call(parser, &node.expression.function_call) ? (node.expression.type = Tree::ExpressionNode::Type::FunctionCall), true : false) ||
                  (terminal<SymbolType::Identifier>(parser, &node.expression.identifier) ? (node.expression.type = Tree::ExpressionNode::Type::Identifier), true : false));

  node.text_end = parser.last_symbol_end_position;

  if (matches && result)
  {
    *result = Allocate::copy(node);
  }

  end_production_debug(parser, matches, *result);
  return matches;
}


bool
expressions(Parser& parser, Tree::Node** result)
{
  start_production_debug(parser);

  // expressions <- expression { ',' expressions }
  //              | .

  bool matches = true;

  Tree::Node node = { .type = Tree::Node::Expressions, .text_start = current_text_position(parser) };
  Tree::Node* expression_node = NULL;

  do
  {
    if (expression(parser, &expression_node))
    {
      Array::add(node.expressions.expressions, expression_node);
      expression_node = NULL;
    }
    else if (node.expressions.expressions.n_elements == 0)
    {
      break;
    }
    else
    {
      assert(0 && "error");
    }
  }
  while (terminal<SymbolType::Comma>(parser));

  node.text_end = parser.last_symbol_end_position;

  if (matches && result)
  {
    *result = Allocate::copy(node);
  }
  else
  {
    Array::free_array(node.expressions.expressions);
  }

  end_production_debug(parser, matches, *result);
  return matches;
}


bool
declaration(Parser& parser, Tree::Node** result)
{
  start_production_debug(parser);

  // declaration <- `Func` function_signature IDENTIFIER
  //              | IDENTIFIER IDENTIFIER

  Tree::Node node = { .type = Tree::Node::Declaration, .text_start = current_text_position(parser) };

  bool matches = false;

  // TODO: This is a bit of a hacky work around to prevent decl-> IDENTIFIER(type) IDENTIFIER(label) being ambiguous with expr-> IDENTIFIER(label) if not matched at the same time.
  //       So we have to match the type inside the declaration instead of in its own production
  //
  //       Maybe change declaration syntax to `name: type = value` to resolve this in a neater way

  node.declaration.type_name = Allocate::allocate<Tree::Node>();
  node.declaration.type_name->type = Tree::Node::Type::TypeName;

  if (terminal<SymbolType::Func>(parser))
  {
    matches = true;

    node.declaration.type_name->type_name.type = Tree::TypeNameNode::Type::FunctionSignature;

    // These must both match, as we have already consumed the Func symbol
    matches &= function_signature(parser, &node.declaration.type_name->type_name.function_signature);
    assert(matches);
    matches &= terminal<SymbolType::Identifier>(parser, &node.declaration.label);
    assert(matches);
  }
  else
  {
    Tree::Node** arg_types[] = {&node.declaration.type_name->type_name.identifier, &node.declaration.label};
    if (terminal<SymbolType::Identifier, SymbolType::Identifier>(parser, arg_types))
    {
      matches = true;
      node.declaration.type_name->type_name.type = Tree::TypeNameNode::Type::Identifier;
    }
  }

  node.text_end = parser.last_symbol_end_position;

  if (matches && result)
  {
    *result = Allocate::copy(node);
  }
  else
  {
    Allocate::unallocate(node.declaration.type_name);
  }

  end_production_debug(parser, matches, *result);
  return matches;
}


bool
declarations(Parser& parser, Tree::Node** result)
{
  start_production_debug(parser);

  // declarations <- declaration { ',' declaration }
  //               | .

  bool matches = true;

  Tree::Node node = { .type = Tree::Node::Declarations, .text_start = current_text_position(parser) };
  Tree::Node* declaration_node = NULL;

  do
  {
    if (declaration(parser, &declaration_node))
    {
      Array::add(node.declarations.declarations, declaration_node);
      declaration_node = NULL;
    }
    else if (node.declarations.declarations.n_elements == 0)
    {
      break;
    }
    else
    {
      assert(0 && "error");
    }
  }
  while (terminal<SymbolType::Comma>(parser));

  node.text_end = parser.last_symbol_end_position;

  if (matches && result)
  {
    *result = Allocate::copy(node);
  }
  else
  {
    Array::free_array(node.declarations.declarations);
  }

  end_production_debug(parser, matches, *result);
  return matches;
}


bool
assignment(Parser& parser, Tree::Node** result)
{
  start_production_debug(parser);

  // assignment <- declaration '=' expression

  Tree::Node node = { .type = Tree::Node::Assignment, .text_start = current_text_position(parser) };

  bool matches = declaration(parser, &node.assignment.declaration);

  if (matches)
  {
    ProductionLambda productions[] = {
      {terminal<SymbolType::Equals>},
      {expression, &node.assignment.expression}
    };

    if (!accept_sequence(parser, LENGTH(productions), productions))
    {
      assert(0 && "error");
    }
  }

  node.text_end = parser.last_symbol_end_position;

  if (matches && result)
  {
    *result = Allocate::copy(node);
  }

  end_production_debug(parser, matches, *result);
  return matches;
}


bool
statement(Parser& parser, Tree::Node** result)
{
  start_production_debug(parser);

  // statement <- assignment |
  //              expression

  Tree::Node node = { .type = Tree::Node::Statement, .text_start = current_text_position(parser) };

  bool matches = ((assignment(parser, &node.statement.assignment) ? (node.statement.type = Tree::StatementNode::Type::Assignment), true : false) ||
                  (expression(parser, &node.statement.expression) ? (node.statement.type = Tree::StatementNode::Type::Expression), true : false));

  node.text_end = parser.last_symbol_end_position;

  if (matches && result)
  {
    *result = Allocate::copy(node);
  }

  end_production_debug(parser, matches, *result);
  return matches;
}


bool
body(Parser& parser, Tree::Node** result)
{
  start_production_debug(parser);

  // body <- { statement }

  Tree::Node node = { .type = Tree::Node::Body, .text_start = current_text_position(parser) };
  Tree::Node* statement_node = NULL;

  bool matches = true;

  while (statement(parser, &statement_node))
  {
    Array::add(node.body.statements, statement_node);
    statement_node = NULL;
  }

  node.text_end = parser.last_symbol_end_position;

  if (matches && result)
  {
    *result = Allocate::copy(node);
  }
  else
  {
    Array::free_array(node.body.statements);
  }

  end_production_debug(parser, matches, *result);
  return matches;
}


bool
program(Parser& parser, Tree::Node** result)
{
  start_production_debug(parser);

  // program <- body

  Tree::Node node = { .type = Tree::Node::Program, .text_start = current_text_position(parser) };

  bool matches = body(parser, &node.program.body);

  node.text_end = parser.last_symbol_end_position;

  if (matches && result)
  {
    *result = Allocate::copy(node);
  }

  end_production_debug(parser, matches, *result);
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
// type <- `Func` function_signature
//       | IDENTIFIER
//
// function_signature <- '[' type ']' '(' declarations ')'
//
// declaration <- `Func` function_signature IDENTIFIER
//              | IDENTIFIER IDENTIFIER
//
// expression <- literal
//             | function_call
//             | IDENTIFIER
//
// TODO: different number types (hex, ...)
//
// literal <- NUMBER
//          | function
//
// function_call <- IDENTIFIER '(' expressions ')'
//
// function <- function_signature '{' body '}'
//
// expressions <- expression { ',' expressions }
//              | .
//
// declarations <- declaration { ',' declaration }
//               | .
//

} // namespace Parser

} // namespace Compolls