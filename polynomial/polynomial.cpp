#include "polynomial.hpp"

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

template <typename T> T Polynomial<T>::evaluate(T x) {
  T sum = 0;
  Node *current = head->next;
  uint32_t lastExponent = 0;
  for (; current; current = current->next, lastExponent = current->exponent) {
    sum *= pow(x, lastExponent - current->exponent);
    sum += x;
  }

  sum *= pow(x, lastExponent);
  return sum;
}

template <typename T> Polynomial<T> &&operator+(Polynomial<T> &poly) {}

template <typename T> Polynomial<T> &&operator*(T scale) {}

template <typename T> Polynomial<T> &&operator-(Polynomial<T> &poly) {}
} // namespace poly