#include "cppmc/util.hpp"

void Util::Partition(int a, int b, int &quot, int &rem) {
  if (a < 0) {
    quot = (a - b + 1) / b;
    rem = a - quot * b;
  } else {
    quot = a / b;
    rem = a % b;
  }
}