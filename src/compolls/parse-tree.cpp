#include "parse-tree.h"

#include "parse-tree-visitor.h"

#include "utils/c-array.h"
#include "utils/assert.h"


namespace Compolls
{

namespace Parser
{

namespace Tree
{
static const String::String NODE_NAMES[] = {
  "Program",
  "Statement",
  "Body",
  "Scope",
  "Assignment",
  "TypeName",
  "Declaration",
  "Expression",
  "Literal",
  "FunctionCall",
  "FunctionSignature",
  "Function",
  "Expressions",
  "Declarations",
  "Terminal"
};
static_assert(array_size(NODE_NAMES) == Node::TypeCount);

String::String
node_name(Node::Type node_type)
{
  return NODE_NAMES[node_type];
}


struct StringBuilder
{
  String::String const* text;
  StringArray::StringArray* result;
  u32 depth;
};


void
indent(StringBuilder& string_builder)
{
  for (u32 i = 0; i < string_builder.depth; ++i)
  {
    *string_builder.result += "  ";
  }
}


bool
string(StringBuilder& string_builder, VisitorEvent event, Node* node)
{
  bool success = true;

  switch (event)
  {
    case (VisitorEvent::Enter):
    {
      assert(node);

      indent(string_builder);
      string_builder.depth += 1;

      *string_builder.result += node_name(node->type);
      *string_builder.result += " {\n";
    } break;

    case (VisitorEvent::Leave):
    {
      string_builder.depth -= 1;
      indent(string_builder);
      *string_builder.result += "}\n";
    } break;
  }

  return success;
}


bool
terminal_string(StringBuilder& string_builder, VisitorEvent event, TerminalNode* terminal_node)
{
  bool success = true;

  if (event == VisitorEvent::Enter)
  {
    indent(string_builder);
    *string_builder.result += "\"";
    *string_builder.result += Tokeniser::string(*string_builder.text, terminal_node->token);
    *string_builder.result += "\"\n";
  }

  return success;
}


bool
string(String::String const & text, Node *node, StringArray::StringArray& result)
{
  Visitor<StringBuilder> visitor = {};
  visitor.generic_func = string;
  visitor.terminal_func = terminal_string;

  StringBuilder string_builder = {};
  string_builder.text = &text;
  string_builder.result = &result;
  string_builder.depth = 0;

  return visit<StringBuilder>(string_builder, visitor, node);
}

} // namespace Tree

} // namespace Parser

} // namespace Compolls