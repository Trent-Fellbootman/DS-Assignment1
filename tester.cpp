#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include <stdio.h>
#include <iomanip>
#include <numeric>
#define POLYNOMIAL_NOT_FOUND_MESSAGE "Error: Polynomial named %s does not exist"

std::vector<std::string> separate(std::string original,
                                  std::set<char> delimiters) {
  std::vector<std::string> res;

  int last = 0;

  while (true) {
    bool found = false;
    for (char delimiter : delimiters) {
      int tmp = original.find_first_of(delimiter, last);
      if (tmp != std::string::npos) {
        if (last < tmp) {
          res.push_back(
              std::string(original.begin() + last, original.begin() + tmp));
        }
        res.push_back(
            std::string(original.begin() + tmp, original.begin() + tmp + 1));
        last = tmp + 1;
        found = true;
      }
    }

    if (!found) {
      if (last < original.size()) {
        res.push_back(std::string(original.begin() + last, original.end()));
      }
      break;
    }
  }

  return std::move(res);
}

int main() {
  // std::string a = "ccutsadguafdccue";

  // for (std::string tmp : separate(a, {'c', 'u'})) {
  //   std::cout << tmp << std::endl;
  // }

  // std::map<std::string, int> m;
  // m["cute"] = 1;
  // m["koala"] = 2;
  // for (auto &pair : m) {
  //   std::cout << pair.first << pair.second << std::endl;
  // }
  // std::vector<int> v{0, 1, 2, 3, 4};
  // std::cout << (m.find("kangaroo") == m.end()) << std::endl;
  // printf(POLYNOMIAL_NOT_FOUND_MESSAGE, "koalas");
  // std::cout << std::accumulate(v.begin(), v.end(), 0);

  // std::cout << std::setw(20) << 2135210000.0 << "cute" << std::endl;(outputWidth - contentWidth) / 2
  // std::cout << std::unitbuf;
  std::cout << "koala" << std::endl;
  std::cout << "koala";
  std::cout << "\x1b[1Akangaroo";
}