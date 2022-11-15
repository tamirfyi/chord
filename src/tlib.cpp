#include "tlib.h"
#include <iostream>

using namespace std;

bool isAnElementOf(int left, bool l_inclusive, int right, bool r_inclusive, int val)
{
  // cout << (l_inclusive ? "[" : "(") << left << " <" << val << "> " << right << (r_inclusive ? "]" : ")") << endl;

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
    int range = (256 + right) - left;

    int newLeft = 0;
    int newRight = range;
    int newVal = (val + range) % 256;

    if (l_inclusive && r_inclusive)
    {
      if (val > right && val < left)
        return false;
      return true;
    }
    else if (!l_inclusive && !r_inclusive)
    {
      if (val >= right && val <= left)
        return false;
      return true;
    }
    else if (l_inclusive && !r_inclusive)
    {
      if (val >= right && val < left)
        return false;
      return true;
    }
    else if (!l_inclusive && r_inclusive)
    {
      if (val > right && val <= left)
        return false;
      return true;
    }
  }
  else if (left == right) //[1, 1] OR (1, 1)
  {
    if (l_inclusive == false || r_inclusive == false)
      return false;
    if (val == left)
      return true;
  }

  return false;
}