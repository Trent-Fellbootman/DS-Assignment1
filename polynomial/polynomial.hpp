#include <memory>
#include <stdint.h>
// WARNING: ALL FUNCTIONS NOT DEBUGGED
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

    Node &&copy() {
      Node cp{coefficient, exponent, next};
      return std::move(cp);
    }
  };

  Node *head;

public:
  Polynomial(Node *head) : head(head) {}
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

  Polynomial &&copy() {
    Polynomial poly{head->copy()};

    for (Node *cur = head->next, Node *cpCur = poly.head; cur->next;
         cur = cur->next, cpCur = cpCur->next) {
      cpCur.next = cur->next->copy();
    }
  }

  Polynomial &&operator+(Polynomial &poly) {
    Polynomial cpPoly = copy();
    // while (cpPoly = )
  }

  Polynomial &&operator*(T scale) {
    Polynomial poly = copy();
    for (Node *current = poly.head->next; current; current = current->next) {
      current->coefficient *= scale;
    }
    return std::move(poly);
  }

  Polynomial &&operator/(T scale) {
    Polynomial poly = copy();
    for (Node *current = poly.head->next; current; current = current->next) {
      current->coefficient /= scale;
    }
    return std::move(poly);
  }

  Polynomial &&operator-(Polynomial &poly) {
    Polynomial cpPoly = copy();
  }
};
} // namespace poly
