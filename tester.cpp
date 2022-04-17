#include <string>
#include <iostream>

int main() {
  std::string a = "ccute";
  std::cout << std::count(a.begin(), a.end(), 'c') << std::endl;
}