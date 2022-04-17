#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

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
  std::string a = "ccutsadguafdccue";

  for (std::string tmp : separate(a, {'c', 'u'})) {
    std::cout << tmp << std::endl;
  }

  std::map<std::string, int> m;
  m["cute"] = 1;
  m["koala"] = 2;
  for (auto &pair : m) {
    std::cout << pair.first << pair.second << std::endl;
  }
}