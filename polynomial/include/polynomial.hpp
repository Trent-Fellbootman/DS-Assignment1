#include <stdint.h>

#pragma once
namespace poly {
template <typename T>

class Polynomial {
  struct Node {
    T coefficient;
    uint32_t exponent;
    Node *next;
  };

  Node *head;

public:
  uint32_t getLength() { return head->exponent; }
  T evaluate(T x);

  Polynomial(const Polynomial&) = delete;
  Polynomial& operator=(const Polynomial&) = delete;
  Polynomial&& operator=(const Polynomial&&) = default;

  Polynomial& copy() {
    
  }

  Polynomial&& operator+(Polynomial& poly);
  Polynomial&& operator*(T scale);
  Polynomial&& operator-(Polynomial& poly);
};
} // namespace poly
