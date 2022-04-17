#include "application.hpp"
#include <iostream>
#include <sstream>
#define BUFFER_SIZE 1024

enum class OpType {
  ASSIGN, DISPLAY, EVALUATE, PLOT
};

template <typename T>
void Application<T>::run() {
  char buffer[BUFFER_SIZE];
  while (true) {
    std::cin.getline(buffer, BUFFER_SIZE);

  }
}