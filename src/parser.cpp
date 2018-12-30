#include "parser.h"

#include "pair.h"
#include "allocate.h"
#include "meta-list.h"

#define LENGTH(array) sizeof((array))/sizeof((array)[0])


namespace Compolls
{

namespace Parser
{

#define PARSE_DEBUG_TRACE (0)


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

  if (!Fifo::empty(parser.tokens))
  {
    Tokeniser::Token token = Fifo::pop(parser.tokens);
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


template <u32 length, typename TerminalSymbols>
bool
terminal(Parser& parser, Tree::Node** o_token_nodes[])
{
  assert(length <= parser.lookahead.lookahead_n);

  bool matches = true;

  for (u32 i = 0; i < length; ++i)
  {
    if (parser.lookahead.symbols[i].type != TerminalSymbols::Get(i))
    {
      matches = false;
      break;
    }
  }

  if (matches)
  {
    for (u32 i = 0; i < length; ++i)
    {
      Tree::Node** o_token_node = o_token_nodes[i];

      if (o_token_node)
      {
        *o_token_node = Allocate::allocate<Tree::Node>();
        (**o_token_node).type = Tree::Node::Terminal;
        (**o_token_node).terminal.token = parser.lookahead.symbols[i].token;
        (**o_token_node).text_start = parser.lookahead.symbols[i].token.start;
        (**o_token_node).text_end = parser.lookahead.symbols[i].token.end;
      }
    }

    for (u32 i = 0; i < parser.lookahead.lookahead_n; ++i)
    {
      if (i+length < parser.lookahead.lookahead_n)
      {
        parser.lookahead.symbols[i] = parser.lookahead.symbols[i+length];
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
  return terminal<1, Meta::List1<SymbolType, terminal_symbol>>(parser, &o_token_node);
}


template <SymbolType terminal_symbol_a, SymbolType terminal_symbol_b>
bool
terminal(Parser& parser, Tree::Node** o_token_node_a = NULL, Tree::Node** o_token_node_b = NULL)
{
  Tree::Node** o_token_nodes[2] = {o_token_node_a, o_token_node_b};
  return terminal<2, Meta::List2<SymbolType, terminal_symbol_a, terminal_symbol_b>>(parser, o_token_nodes);
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
#endif
}


void
end_production_debug(Parser& parser, bool matches, Tree::Node const * node)
{
#if PARSE_DEBUG_TRACE
  parser.depth -= 1;

  printf("%*s}", parser.depth*2, "");
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
function(Parser& parser, Tree::Node** result)
{
  start_production_debug(parser);

  // function <- '[' identifier ']' '(' declarations ')' '{' body '}'

  Tree::Node node = { .type = Tree::Node::Function, .text_start = current_text_position(parser) };

  bool matches = terminal<SymbolType::L_Bracket>(parser);

  if (matches)
  {
    ProductionLambda sequence[] = {
      {terminal<SymbolType::Identifier>, &node.function.return_type},
      {terminal<SymbolType::R_Bracket>},
      {terminal<SymbolType::L_Parenthesis>},
      {declarations, &node.function.declarations},
      {terminal<SymbolType::R_Parenthesis>},
      {terminal<SymbolType::L_Brace>},
      {body, &node.function.body},
      {terminal<SymbolType::R_Brace>}
    };

    if (!accept_sequence(parser, LENGTH(sequence), sequence))
    {
      // TODO: More descriptive error message
      assert(0 && "Error while parsing function definition");
    }
  }

  node.text_end = current_text_position(parser);

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

  node.text_end = current_text_position(parser);

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

  bool matches = terminal<SymbolType::Identifier, SymbolType::L_Parenthesis>(parser, &node.function_call.label);

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

  node.text_end = current_text_position(parser);

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

  node.text_end = current_text_position(parser);

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

  node.text_end = current_text_position(parser);

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

  // declaration <- IDENTIFIER IDENTIFIER

  Tree::Node node = { .type = Tree::Node::Declaration, .text_start = current_text_position(parser) };

  bool matches = terminal<SymbolType::Identifier, SymbolType::Identifier>(parser, &node.declaration.type, &node.declaration.label);

  node.text_end = current_text_position(parser);

  if (matches && result)
  {
    *result = Allocate::copy(node);
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

  node.text_end = current_text_position(parser);

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

  node.text_end = current_text_position(parser);

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

  node.text_end = current_text_position(parser);

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

  node.text_end = current_text_position(parser);

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

  node.text_end = current_text_position(parser);

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

} // namespace Compolls