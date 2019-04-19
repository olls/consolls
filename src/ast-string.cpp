#include "ast-string.h"

#include "ast-visitor.h"
#include "type-system.h"
#include "string-array.h"
#include "string.h"


namespace Compolls
{

namespace AST
{

struct StringBuilder
{
  AST const* ast;
  String::String const* text;
  StringArray::StringArray* result;
  u32 depth;
};


void
indent(StringBuilder& state, u32 plus = 0)
{
  for (u32 i = 0; i < (state.depth + plus); ++i)
  {
    *state.result += "  ";
  }
}


bool
node_func(StringBuilder& state, VisitorEvent event)
{
  bool success = true;

  switch (event)
  {
    case (VisitorEvent::Enter):
    {
      indent(state);
      state.depth += 1;
    } break;
    case (VisitorEvent::Leave):
    {
      state.depth -= 1;
      indent(state);
      *state.result += "}\n";
    } break;
  }

  return success;
}


void
string_string(StringArray::StringArray& result, Strings::Table const & strings, Strings::ID string)
{
  result += "string: ";
  result += String::string_f("%d, \"", string);
  result += Strings::get(strings, string);
  result += "\"";
}


bool
string(StringBuilder& state, VisitorEvent event, Program*)
{
  bool success = true;

  if (event == VisitorEvent::Enter)
  {
    *state.result += "Program {\n";
  }

  return success;
}

bool
string(StringBuilder& state, VisitorEvent event, Body* body)
{
  bool success = true;

  if (event == VisitorEvent::Enter)
  {
    *state.result += "Body {\n";
    indent(state);  *state.result += "types: {  ";
    for (u32 type_index = 0;
         type_index < body->scope.types.n_elements;
         ++type_index)
    {
      TypeSystem::Type type = body->scope.types[type_index];
      *state.result += TypeSystem::string(type);
      *state.result += "  ";
    }
    *state.result += "}\n";
  }

  return success;
}

bool
string(StringBuilder& state, VisitorEvent event, Statement*)
{
  bool success = true;

  if (event == VisitorEvent::Enter)
  {
    *state.result += "Statement {\n";
  }

  return success;
}

bool
string(StringBuilder& state, VisitorEvent event, Assignment*)
{
  bool success = true;

  if (event == VisitorEvent::Enter)
  {
    *state.result += "Assignment {\n";
  }

  return success;
}

bool
string(StringBuilder& state, VisitorEvent event, Declaration*)
{
  bool success = true;

  if (event == VisitorEvent::Enter)
  {
    *state.result += "Declaration {\n";
  }

  return success;
}

bool
string(StringBuilder& state, VisitorEvent event, Expression*)
{
  bool success = true;

  if (event == VisitorEvent::Enter)
  {
    *state.result += "Expression {\n";
  }

  return success;
}

bool
string(StringBuilder& state, VisitorEvent event, FunctionCall*)
{
  bool success = true;

  if (event == VisitorEvent::Enter)
  {
    *state.result += "FunctionCall {\n";
  }

  return success;
}

bool
string(StringBuilder& state, VisitorEvent event, Literal*)
{
  bool success = true;

  if (event == VisitorEvent::Enter)
  {
    *state.result += "Literal {\n";
  }

  return success;
}

bool
string(StringBuilder& state, VisitorEvent event, Function*)
{
  bool success = true;

  if (event == VisitorEvent::Enter)
  {
    *state.result += "Function {\n";
  }

  return success;
}

bool
string(StringBuilder& state, VisitorEvent event, Number* number)
{
  bool success = true;

  if (event == VisitorEvent::Enter)
  {
    *state.result += "Number {\n";
    indent(state);
    *state.result += "text: \"";
    *state.result += number->text;
    *state.result += "\"\n";
    indent(state);
    *state.result += "number: ";
    *state.result += String::string_f("%u\n", number->number);
  }

  return success;
}

bool
string(StringBuilder& state, VisitorEvent event, Declarations*)
{
  bool success = true;

  if (event == VisitorEvent::Enter)
  {
    *state.result += "Declarations {\n";
  }

  return success;
}

bool
string(StringBuilder& state, VisitorEvent event, Expressions*)
{
  bool success = true;

  if (event == VisitorEvent::Enter)
  {
    *state.result += "Expressions {\n";
  }

  return success;
}

bool
string(StringBuilder& state, VisitorEvent event, Type* type)
{
  bool success = true;

  if (event == VisitorEvent::Enter)
  {
    *state.result += "Type {\n";

    indent(state);
    string_string(*state.result, state.ast->strings, type->label);
    *state.result += "\n";

    indent(state);
    *state.result += String::string_f("type: %u\n", type->type);
  }

  return success;
}

bool
string(StringBuilder& state, VisitorEvent event, Identifier* identifier)
{
  bool success = true;

  if (event == VisitorEvent::Enter)
  {
    *state.result += "Identifier {\n";

    indent(state);
    string_string(*state.result, state.ast->strings, identifier->label);
    *state.result += "\n";

    indent(state);
    *state.result += String::string_f("identifier_id: %u\n", identifier->identifier);
  }

  return success;
}


bool
string(String::String const & text, AST& ast, StringArray::StringArray& result)
{
  Visitor<StringBuilder> visitor = {};
  visitor.node_func = node_func;
  visitor.program_func = string;
  visitor.body_func = string;
  visitor.statement_func = string;
  visitor.assignment_func = string;
  visitor.declaration_func = string;
  visitor.expression_func = string;
  visitor.function_call_func = string;
  visitor.literal_func = string;
  visitor.function_func = string;
  visitor.number_func = string;
  visitor.declarations_func = string;
  visitor.expressions_func = string;
  visitor.type_func = string;
  visitor.identifier_func = string;

  StringBuilder string_builder = {};
  string_builder.ast = &ast;
  string_builder.text = &text;
  string_builder.result = &result;
  string_builder.depth = 0;

  return visit<StringBuilder>(string_builder, visitor, &ast);
}

} // namespace AST

} // namespace Compolls