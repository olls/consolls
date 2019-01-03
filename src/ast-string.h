#pragma once

#include "ast.h"
#include "string.h"
#include "types.h"


namespace Compolls
{

namespace AST
{

bool
string(String::String const & text, AST& ast, StringArray::StringArray& result);

} // namespace AST

} // namespace Compolls