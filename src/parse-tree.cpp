#include "parse-tree.h"

#include "parse-tree-visitor.h"


namespace Compolls
{

namespace Parser
{

namespace Tree
{

static const String::String NODE_NAMES[] = {
  S("Program"),
  S("Statement"),
  S("Body"),
  S("Assignment"),
  S("Declaration"),
  S("Expression"),
  S("Literal"),
  S("FunctionCall"),
  S("Function"),
  S("Expressions"),
  S("Declarations"),
  S("Terminal")
};

String::String
node_name(Node::Type node_type)
{
  return NODE_NAMES[node_type];
}


#define INDENT(sb) { for (u32 i = 0; i < (sb).depth; ++i) { *(sb).result += S("  "); } }


struct StringBuilder
{
  String::String const* text;
  StringArray::StringArray* result;
  u32 depth;
};


bool
string(StringBuilder& string_builder, Visitor<StringBuilder>::Event event, Node* node)
{
  bool success = true;

  switch (event)
  {
    case (Visitor<StringBuilder>::Event::Begin):
    {
      assert(node);

      INDENT(string_builder);
      string_builder.depth += 1;

      *string_builder.result += node_name(node->type);
      *string_builder.result += S(" {\n");
    } break;

    case (Visitor<StringBuilder>::Event::End):
    {
      string_builder.depth -= 1;
      INDENT(string_builder);
      *string_builder.result += S("}\n");
    } break;
  }

  return success;
}


bool
terminal_string(StringBuilder& string_builder, Visitor<StringBuilder>::Event event, TerminalNode* terminal_node)
{
  bool success = true;

  INDENT(string_builder);
  *string_builder.result += S("\"");
  *string_builder.result += Tokeniser::string(*string_builder.text, terminal_node->token);
  *string_builder.result += S("\"\n");

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