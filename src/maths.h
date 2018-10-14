#include "types.h"


namespace Maths
{

template <typename T>
T
min(T const & a, T const & b)
{
  return (a < b) ? a : b;
}


template <typename T>
T
max(T const & a, T const & b)
{
  return (a > b) ? a : b;
}

} // namespace Maths