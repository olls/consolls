#include "array.h"
#include "types.h"


namespace Stack
{

template <typename T>
struct Stack
{
  Array::Array<T> elements;
  u32 position;
};


template <typename T>
s32
elements_remaining(Stack<T>& stack)
{
  return stack.elements.n_elements - stack.position;
}


template <typename T>
bool
empty(Stack<T>& stack)
{
  return elements_remaining(stack) <= 0;
}


template <typename T>
T&
pop(Stack<T>& stack)
{
  assert(!empty(stack));

  T& result = stack.elements[stack.position];
  stack.position += 1;
  return result;
}

} // namespace Stack
