#include "string.h"
#include "array.h"
#include "types.h"


namespace Tokeniser
{

struct Token
{
  u32 start;
  u32 end;
};


Array::Array<Token>
tokenise(String::String text);


String::String
string(String::String const & text, Token const & token);

} // namespace Tokeniser