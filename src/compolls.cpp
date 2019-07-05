#include "compolls.h"

#include "strings.h"
#include "ast-visitor.h"
#include "ast-string.h"
#include "ast.h"
#include "code-generator.h"
#include "parse-tree-visitor.h"
#include "parser.h"
#include "parse-tree.h"
#include "string-array.h"
#include "file.h"


namespace Compolls
{

bool
compile_file(char const* filename, Machine::Machine& machine, Machine::MemoryAddress& addr, Machine::MemoryAddress& start_result, String::String* error)
{
  bool success = true;

  String::String error_msg_void;
  String::String* error_msg;
  if (error)
  {
    error_msg = error;
  }
  else
  {
    error_msg = &error_msg_void;
  }

  File::File file = {};
  if (!File::open(filename, &file))
  {
    success &= false;
    *error_msg = String::string_f("Couldn't open file: %s\n", filename);
  }
  else
  {
    String::String file_string = { file.read_ptr, (u32)file.size };

    Machine::MemoryAddress result;
    if (!Compolls::compile(file_string, machine, addr, start_result))
    {
      success &= false;
    }
  }
  return success;
}


bool
compile(String::String text, Machine::Machine& machine, Machine::MemoryAddress& addr, Machine::MemoryAddress& start_result)
{
  bool success = true;

  printf("Compiling:  \"%.*s\"\n", text.length, text.start);

  Strings::Table strings = {};
  Array::Array<Tokeniser::Token> tokens = Tokeniser::tokenise(text, strings);

  printf("Tokens: ");
  Tokeniser::print(strings, tokens);

  Parser::Tree::Node* program_node = NULL;
  {
    Parser::Parser parser = {};
    parser.tokens.elements = tokens;
    parser.text = text;
    parser.strings = &strings;

    Parser::prime_lookahead(parser);

    success &= Parser::program(parser, &program_node);
    printf("\n\n");

    success &= Parser::validate_end_state(parser);
  }
  Array::free_array(tokens);

  {
    StringArray::StringArray parse_tree_text = {};
    Parser::Tree::string(text, program_node, parse_tree_text);
    parse_tree_text += "\n";
    StringArray::print(parse_tree_text);
    Array::free_array(parse_tree_text);
  }

  if (!success)
  {
    printf("\nParsing error.\n");
  }
  else
  {
    {
      StringArray::StringArray strings_text = {};
      Strings::string(strings, strings_text);
      strings_text += "\n";
      StringArray::print(strings_text);
      Array::free_array(strings_text);
    }

    AST::AST ast = {};
    success &= AST::make_ast(text, strings, ast, program_node);

    // TODO: free parse tree

    if (!success)
    {
      printf("\nAST error.\n");
    }
    else
    {
      {
        StringArray::StringArray ast_text = {};
        AST::string(text, ast, ast_text);
        ast_text += "\n";
        StringArray::print(ast_text);
        Array::free_array(ast_text);
      }

      success &= CodeGenerator::generate_code(&machine, addr, ast, start_result);

      if (!success)
      {
        printf("\nCode generation error.\n");
      }
    }
  }

  // TODO: free AST
  Array::free_array(strings.table);

  return success;
}

} // namespace Compolls
