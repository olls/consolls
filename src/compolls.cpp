#include "compolls.h"

#include "parser.h"
#include "parse-tree.h"
#include "string-array.h"


namespace Compolls
{

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

  parser.lookahead.symbols[0] = advance_terminal(parser);
  parser.lookahead.symbols[1] = advance_terminal(parser);

  bool success = true;

  Parser::Tree::Node* program_node = NULL;

  success &= Parser::program(parser, &program_node);

  assert(success);

  StringArray::StringArray parse_tree_text = {};
  Parser::Tree::string(text, parse_tree_text, program_node);
  parse_tree_text += S("\n");
  StringArray::print(parse_tree_text);

  return result;
}

} // namespace Compolls
