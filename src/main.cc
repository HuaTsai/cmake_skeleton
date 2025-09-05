#include <iostream>
#include <myproject/math_utils.hpp>

int main() {
  int result1 = add(3, 4);
  int result2 = square(5);
  std::cout << "3 + 4 = " << result1 << std::endl;
  std::cout << "5 squared = " << result2 << std::endl;
}
