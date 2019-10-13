#include "types.h"


namespace Maths
{

template <typename T>
inline T
min(T const & a, T const & b)
{
  return (a < b) ? a : b;
}


template <typename T>
inline T
max(T const & a, T const & b)
{
  return (a > b) ? a : b;
}

} // namespace Maths