#include <algorithm>
#include <cstdint>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#pragma once
namespace poly {

class polynomial_multiplication_not_implimented : public std::exception {
public:
  const char *what() const noexcept override final {
    return "Polynomial multiplication is not yet implemented";
  }
};

namespace helper {
template <typename T> T pow(T x, uint32_t exp) {
  T ans = 1;
  T ck = x;
  while (exp != 0) {
    if ((exp & 1) == 1) {
      ans = ans * ck;
    }
    exp = exp >> 1;
    ck = ck * ck;
  }
  return ans;
}
} // namespace helper

template <typename T>

class Polynomial {
  struct Node {
    T coefficient;
    uint32_t exponent;
    std::unique_ptr<Node> next;

    Node(T coefficient, uint32_t exponent, std::unique_ptr<Node> next)
        : coefficient(coefficient), exponent(exponent), next(std::move(next)) {}

    Node() : coefficient(0), exponent(0), next(nullptr) {}

    std::unique_ptr<Node> copy() {
      // copy does not copy the next
      return std::make_unique<Node>(coefficient, exponent, nullptr);
    }
  };

  std::unique_ptr<Node> head;

public:
  Polynomial() : Polynomial(std::vector<std::pair<T, uint32_t>>{(1, 1)}){};

  Polynomial(std::vector<std::pair<T, uint32_t>> item) {
    std::sort(item.begin(), item.end(),
              [](std::pair<T, uint32_t> a, std::pair<T, uint32_t> b) {
                return (a.second > b.second);
              });
    head = std::unique_ptr<Node>(new Node);
    head->exponent = item.size();
    Node *cur = head.get();
    for (int i = 0; i < item.size(); i++) {
      cur->next = std::unique_ptr<Node>(new Node);
      cur->next->coefficient = item[i].first;
      cur->next->exponent = item[i].second;
      cur = cur->next.get();
    }
  }

  static Polynomial zero() {
    Polynomial poly;
    poly.head = std::make_unique<Node>(0, 1, nullptr);
    poly.head->next = std::make_unique<Node>(0, 0, nullptr);
    return std::move(poly);
  }

  Polynomial(const Polynomial &original)
      : head(std::move(original.copy()->head)) {}

  Polynomial &operator=(const Polynomial &poly) {
    this->head = std::move(poly.head->copy());
    Node *cpCur = poly.head.get();
    for (Node *cur = head.get(); cpCur->next;
         cur = cur->next.get(), cpCur = cpCur->next.get()) {
      cur->next = cpCur->next->copy();
    }
    return *this;
  }

  Polynomial &operator=(Polynomial &&) = default;
  Polynomial(std::unique_ptr<Node> head) : head(std::move(head)) {}

  uint32_t getLength() const { return head->exponent; }

  std::vector<std::pair<T, uint32_t>> dump() const {
    std::vector<std::pair<T, uint32_t>> ret;
    for (Node *current = head.get(); current->next;
         current = current->next.get()) {
      ret.push_back(std::pair<T, uint32_t>(current->next->coefficient,
                                           current->next->exponent));
    }
    return ret;
  }

  T evaluate(T x) const {
    T sum = 0;
    Node *current = head->next.get();
    uint32_t lastExponent = current->exponent;
    for (; current;
         lastExponent = current->exponent, current = current->next.get()) {
      sum *= helper::pow(x, lastExponent - current->exponent);
      sum += current->coefficient;
      if (current->next == nullptr) {
        lastExponent = current->exponent;
        break;
      }
    }

    sum *= helper::pow(x, lastExponent);
    return sum;
  }

  std::unique_ptr<Polynomial> copy() const {
    Polynomial poly{std::move(head->copy())};
    Node *cpCur = poly.head.get();
    for (Node *cur = head.get(); cur->next;
         cur = cur->next.get(), cpCur = cpCur->next.get()) {
      cpCur->next = cur->next->copy();
    }
    return std::make_unique<Polynomial>(std::move(poly.head));
  }

  Polynomial operator+(Polynomial &poly) const {
    Polynomial res{std::move(head->copy())};

    res.head->exponent = 0;
    // assume that all polynomials will not be null
    Node *cur = res.head.get(), *cur1 = head->next.get(),
         *cur2 = poly.head->next.get();
    for (; cur1 && cur2;) {
      if (cur1->exponent < cur2->exponent) {
        cur->next = cur2->copy();
        cur2 = cur2->next.get();
        cur = cur->next.get();
        res.head->exponent++;
      } else if (cur1->exponent > cur2->exponent) {
        cur->next = cur1->copy();
        cur1 = cur1->next.get();
        cur = cur->next.get();
        res.head->exponent++;
      } else {
        if (cur1->coefficient + cur2->coefficient != 0) {
          cur->next = std::make_unique<Node>(
              cur1->coefficient + cur2->coefficient, cur1->exponent, nullptr);
          cur = cur->next.get();
          res.head->exponent++;
        }
        cur1 = cur1->next.get();
        cur2 = cur2->next.get();
      }
    }

    if (cur1 != nullptr || cur2 != nullptr) {
      for (Node *remaining = cur1 ? cur1 : cur2; remaining;
           remaining = remaining->next.get(), res.head->exponent++) {
        cur->next = remaining->copy();
        cur = cur->next.get();
      }
    }

    if (res.head->exponent == 0) {
      res.head->next = std::make_unique<Node>(0, 0, nullptr);
      res.head->exponent++;
    }

    return res;
  }

  Polynomial operator*(T scale) const {
    if (scale == 0) {
      return zero();
    }

    Polynomial poly{std::move(head->copy())};
    Node *cpCur = poly.head.get();
    for (Node *cur = head.get(); cur->next;
         cur = cur->next.get(), cpCur = cpCur->next.get()) {
      cpCur->next = cur->next->copy();
      cpCur->next->coefficient *= scale;
    }

    if (poly.head->exponent == 0) {
      return zero();
    }

    return poly;
  }

  Polynomial operator/(T scale) const {
    Polynomial poly{std::move(head->copy())};
    Node *cpCur = poly.head.get();
    for (Node *cur = head.get(); cur->next;
         cur = cur->next.get(), cpCur = cpCur->next.get()) {
      cpCur->next = cur->next->copy();
      cpCur->next->coefficient /= scale;
    }

    if (poly.head->exponent == 0) {
      return zero();
    }

    return poly;
  }

  Polynomial operator*(const Polynomial &poly) const noexcept(false) {
    if (this->head->exponent == 1 && this->head->next->exponent == 0) {
      return poly * (this->head->next->coefficient);
    } else if (poly.head->exponent == 1 && poly.head->next->exponent == 0) {
      return (*this) * poly.head->next->coefficient;
    } else {
      throw polynomial_multiplication_not_implimented();
    }

    if (poly.head->exponent == 0) {
      return zero();
    }

    return poly;
  }

  Polynomial operator-(Polynomial &poly) const {
    Polynomial t = poly * -1;
    Polynomial p = operator+(t);

    if (p.head->next == 0) {
      return zero();
    }

    return p;
  }

  static std::string formatItem(T coefficient, uint32_t exponent) {
    std::stringstream stream;
    if ((coefficient != 1 && coefficient != -1) || exponent == 0) {
      stream << coefficient;
    } else if (coefficient == -1) {
      stream << "-";
    }
    if (exponent == 1) {
      stream << "x";
    } else if (exponent > 1) {
      stream << "x^" << exponent;
    }
    return stream.str();
  }

  std::string format() const {
    Node *current = head->next.get();
    std::stringstream stream;
    stream << formatItem(current->coefficient, current->exponent);
    while (current->next) {
      current = current->next.get();
      stream << " + " << formatItem(current->coefficient, current->exponent);
    }
    return stream.str();
  }
};
} // namespace poly
