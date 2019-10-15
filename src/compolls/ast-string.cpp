#include "ast-string.h"

#include "ast-visitor.h"
#include "type-system.h"
#include "strings.h"


namespace Compolls
{

namespace AST
{

struct StringBuilder
{
  AST const* ast;
  ScopeInfo const* curr_scope;
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
string_string(StringArray::StringArray& result, Strings::Table const& strings, Strings::ID string)
{
  result += "string: ";
  result += String::format("%d, \"", string);
  result += Strings::get(strings, string);
  result += "\"";
}


bool
string(StringBuilder& state, VisitorEvent event, void*& user, Program*)
{
  bool success = true;

  if (event == VisitorEvent::Enter)
  {
    *state.result += "Program {\n";
  }

  return success;
}

bool
string(StringBuilder& state, VisitorEvent event, void*& user, Body* body)
{
  bool success = true;

  if (event == VisitorEvent::Enter)
  {
    // Save the state.curr_scope in the user void* so we can revert the new scope on exit of this body node.
    user = (void*)state.curr_scope;
    state.curr_scope = &body->scope;

    *state.result += "Body {\n";

    // indent(state);
    // *state.result += "types: {\n";
    // TypeSystem::string(state.curr_scope->types, *state.result);
    // *state.result += "\n}\n";
  }

  if (event == VisitorEvent::Leave)
  {
    state.curr_scope = (ScopeInfo const*)user;
  }

  return success;
}

bool
string(StringBuilder& state, VisitorEvent event, void*& user, Statement*)
{
  bool success = true;

  if (event == VisitorEvent::Enter)
  {
    *state.result += "Statement {\n";
  }

  return success;
}

bool
string(StringBuilder& state, VisitorEvent event, void*& user, Assignment*)
{
  bool success = true;

  if (event == VisitorEvent::Enter)
  {
    *state.result += "Assignment {\n";
  }

  return success;
}

bool
string(StringBuilder& state, VisitorEvent event, void*& user, Declaration* declaration)
{
  bool success = true;

  if (event == VisitorEvent::Enter)
  {
    *state.result += "Declaration {\n";
    indent(state);
    TypeSystem::string(state.curr_scope->types, declaration->type, *state.result);
    *state.result += "\n";
  }

  return success;
}

bool
string(StringBuilder& state, VisitorEvent event, void*& user, Expression*)
{
  bool success = true;

  if (event == VisitorEvent::Enter)
  {
    *state.result += "Expression {\n";
  }

  return success;
}

bool
string(StringBuilder& state, VisitorEvent event, void*& user, FunctionCall*)
{
  bool success = true;

  if (event == VisitorEvent::Enter)
  {
    *state.result += "FunctionCall {\n";
  }

  return success;
}

bool
string(StringBuilder& state, VisitorEvent event, void*& user, Literal*)
{
  bool success = true;

  if (event == VisitorEvent::Enter)
  {
    *state.result += "Literal {\n";
  }

  return success;
}

bool
string(StringBuilder& state, VisitorEvent event, void*& user, Function* function)
{
  bool success = true;

  if (event == VisitorEvent::Enter)
  {
    *state.result += "Function {\n";
    indent(state);
    TypeSystem::string(state.curr_scope->types, function->type, *state.result);
    *state.result += "\n";
  }

  return success;
}

bool
string(StringBuilder& state, VisitorEvent event, void*& user, Number* number)
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
    *state.result += String::format("%u\n", number->number);
  }

  return success;
}

bool
string(StringBuilder& state, VisitorEvent event, void*& user, Declarations*)
{
  bool success = true;

  if (event == VisitorEvent::Enter)
  {
    *state.result += "Declarations {\n";
  }

  return success;
}

bool
string(StringBuilder& state, VisitorEvent event, void*& user, Expressions*)
{
  bool success = true;

  if (event == VisitorEvent::Enter)
  {
    *state.result += "Expressions {\n";
  }

  return success;
}

bool
string(StringBuilder& state, VisitorEvent event, void*& user, Identifier* identifier)
{
  bool success = true;

  if (event == VisitorEvent::Enter)
  {
    *state.result += "Identifier {\n";

    indent(state);
    string_string(*state.result, *state.ast->strings, identifier->label);
    *state.result += "\n";

    indent(state);
    *state.result += String::format("identifier_id: %u\n", identifier->identifier);
  }

  return success;
}


bool
string(String::String const& text, AST& ast, StringArray::StringArray& result)
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
  visitor.identifier_func = string;

  StringBuilder string_builder = {};
  string_builder.ast = &ast;
  string_builder.result = &result;
  string_builder.depth = 0;

  return visit<StringBuilder>(string_builder, visitor, &ast);
}

} // namespace AST

} // namespace Compolls
