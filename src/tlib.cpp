#include "tlib.h"

bool isAnElementOf(int left, bool l_inclusive, int right, bool r_inclusive, int val)
{
  if (left < right)
  {
    //(10, 100) OR (100, 250)
  }
  else if (left > right)
  {
    //(100, 10)
  }
  else if (left == right)
  {
    if (l_inclusive == false || r_inclusive == false)
      return false;
    if (val == left)
      return true;
  }
}