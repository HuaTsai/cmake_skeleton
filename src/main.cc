#include <iostream>
#include <myproject/math_utils.hpp>

int main() {
  const int result1 = add(3, 4);
  const int result2 = square(5);
  std::cout << "3 + 4 = " << result1 << "\n";
  std::cout << "5 squared = " << result2 << "\n";
}
