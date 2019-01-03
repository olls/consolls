#include "compolls.h"

#include "ast-visitor.h"
#include "ast-string.h"
#include "ast.h"
#include "parse-tree-visitor.h"
#include "parser.h"
#include "parse-tree.h"
#include "string-array.h"


namespace Compolls
{

bool
compile(String::String text, Machine::Machine& machine, Basolls::MemoryAddress& addr, Basolls::Subroutine<void>& result)
{
  result = {};

  printf("Compiling:  \"%.*s\"\n\n", text.length, text.start);

  Array::Array<Tokeniser::Token> tokens = Tokeniser::tokenise(text);

  Tokeniser::print(text, tokens);

  Parser::Parser parser = {};
  parser.tokens.elements = tokens;
  parser.text = text;

  parser.lookahead.symbols[0] = advance_terminal(parser);
  parser.lookahead.symbols[1] = advance_terminal(parser);

  bool success = true;

  Parser::Tree::Node* program_node = NULL;

  success &= Parser::program(parser, &program_node);
  printf("\n\n");

  StringArray::StringArray parse_tree_text = {};
  Parser::Tree::string(text, program_node, parse_tree_text);
  parse_tree_text += S("\n");
  StringArray::print(parse_tree_text);

  if (!success)
  {
    printf("\nParsing error.\n");
  }
  else
  {
    AST::AST ast = {};
    success &= AST::make_ast(text, ast, program_node);

    if (!success)
    {
      printf("\nAST error.\n");
    }
    else
    {
      StringArray::StringArray ast_text = {};
      AST::string(text, ast, ast_text);
      ast_text += S("\n");
      StringArray::print(ast_text);
    }
  }

  return success;
}

} // namespace Compolls
