#include "application/application.hpp"
#include <iostream>

using namespace std;

int main() {
  app::Application<double> myApp;
  myApp.run();
  return 0;
}
