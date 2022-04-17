#include <stdint.h>

#pragma once
namespace poly {

namespace helper {
template <typename T> T pow(T x, uint32_t exp) {
  if (!exp) {
    return 1;
  }

  while (exp) {
    if (exp % 2 == 0) {
      T itm = pow(x, exp / 2);
      return itm * itm;
    } else {
      T itm = pow(x, (exp - 1) / 2);
      return itm * itm * x;
    }
  }
}
} // namespace helper

template <typename T>

class Polynomial {
  struct Node {
    T coefficient;
    uint32_t exponent;
    Node *next;
  };

  Node *head;

public:
  Polynomial(const Polynomial &) = delete;
  Polynomial &operator=(const Polynomial &) = delete;
  Polynomial &&operator=(const Polynomial &&) = default;
  uint32_t getLength() { return head->exponent; }

  T evaluate(T x) {
    T sum = 0;
    Node *current = head->next;
    uint32_t lastExponent = 0;
    for (; current; current = current->next, lastExponent = current->exponent) {
      sum *= helper::pow(x, lastExponent - current->exponent);
      sum += x;
    }

    sum *= helper::pow(x, lastExponent);
    return sum;
  }

  Polynomial &&copy() {}

  void operator+=(Polynomial &poly) {
    Node* cur1 = 
  }

  void operator*=(T scale) {
    for (Node* current = head->next; current; current = current->next) {
      current->coefficient *= scale;
    }
  }

  void operator-=(Polynomial &poly) {

  }
};
} // namespace poly
