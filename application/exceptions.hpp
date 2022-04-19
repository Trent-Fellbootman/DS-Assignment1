#pragma once

#include <exception>
#include <string>

namespace app {
class variable_not_found_exception : public std::exception {
private:
  std::string err;

public:
  variable_not_found_exception(std::string var) {
    char buffer[512];
    std::snprintf(buffer, sizeof(buffer), POLYNOMIAL_NOT_FOUND_MESSAGE,
                  var.c_str());
    this->err = buffer;
  }
  const char *what() const noexcept override final { return err.c_str(); }
};
class invalid_expression_exception : public std::exception {
public:
  const char *what() const noexcept override final {
    return MESSAGE_FAILED_TO_PARSE_EXPRESSIONS;
  }
};
class invalid_polynomial_exception : public std::exception {
public:
  const char *what() const noexcept override final {
    return MESSAGE_FAILED_TO_PARSE_POLYNOMIALS;
  }
};

}; // namespace app
