#include "tlib.h"
#include <iostream>

using namespace std;

bool isAnElementOf(int left, bool l_inclusive, int right, bool r_inclusive, int val)
{
  if (left < right)
  {
    //(10, 100) OR (100, 250)
    if (!l_inclusive && !r_inclusive)
    {
      return (val > left && val < right);
    }
    else if (l_inclusive && !r_inclusive)
    {
      return (val >= left && val < right);
    }
    else if (!l_inclusive && r_inclusive)
    {
      return (val > left && val <= right);
    }
    else
    {
      return (val >= left && val <= right);
    }
  }
  else if (left > right)
  {
    //(100, 10)
    int range = (256 - left) + right + (r_inclusive ? 1 : 0);
    int x = l_inclusive ? -1 : 0;

    for (int i = 0; i < range; i++)
    {
      x = (left + i) % 256;

      if (x == val)
      {
        return true;
      }
    }
  }
  else if (left == right)
  {
    if (l_inclusive == false || r_inclusive == false)
      return false;
    if (val == left)
      return true;
  }

  return false;
}