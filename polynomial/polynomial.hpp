#include <algorithm>
#include <cstdint>
#include <map>
#include <memory>
#include <vector>
// WARNING: ALL FUNCTIONS NOT DEBUGGED
#pragma once
namespace poly {
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

    std::unique_ptr<Node> copy() {
      return std::make_unique<Node>(coefficient, exponent, next);
    }
  };

  std::unique_ptr<Node> head;

public:
  Polynomial() = delete;

  Polynomial(std::vector<std::pair<T, T>> item) {
    std::sort(item.begin(), item.end(),
              [](std::pair<T, T> a, std::pair<T, T> b) {
                return (a.first < b.first);
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

  Polynomial(const Polynomial &) = delete;
  Polynomial &operator=(const Polynomial &) = delete;
  Polynomial &operator=(Polynomial &&) = default;

  uint32_t getLength() { return head->exponent; }

  T evaluate(T x) const {
    T sum = 0;
<<<<<<< HEAD
    Node* current = head->next.get();
    uint32_t lastExponent = current->exponent;
    for (; current; current = current->next.get(), lastExponent = current->exponent) {
=======
    Node *current = head->next.get();
    uint32_t lastExponent = 0;
    while (true) {
>>>>>>> 2c62a1f6cab288203e4710949192d5a1a68e581c
      sum *= helper::pow(x, lastExponent - current->exponent);
      sum += current->coefficient * x;
      if (current->next) {
        current = current->next.get();
        lastExponent = current->exponent;
      } else {
        break;
      }
    }

    sum *= helper::pow(x, lastExponent);
    return sum;
  }

  Polynomial &&copy() const {
    Polynomial poly{head->copy()};
    Node *cpCur = poly.head;
    for (Node *cur = head->next; cur->next;
         cur = cur->next, cpCur = cpCur->next) {
      cpCur->next = cur->next->copy();
    }
  }

  Polynomial &&operator+(Polynomial &poly) const {
    Polynomial res{head->copy()};

    res.head->exponent = 0;
    Node *cur = res.head, *cur1 = head->next, *cur2 = poly.head->next;
    for (; cur1 && cur2; res.head->exponent++) {
      if (cur1->exponent < cur2->exponent) {
        cur->next = cur2->copy();
        cur2 = cur2->next;
        cur = cur->next;
      } else if (cur1->exponent > cur2->exponent) {
        cur->next = cur1->copy();
        cur1 = cur1->next;
        cur = cur->next;
      } else {
        cur->next = Node{cur1->coefficient + cur2->coefficient, cur1->exponent,
                         nullptr};
        cur1 = cur1->next;
        cur2 = cur2->next;
        cur = cur->next;
      }
    }

    if (cur1 != nullptr || cur2 != nullptr) {
      for (Node *remaining = cur1 ? cur1 : cur2; remaining;
           remaining = remaining->next, res.head->exponent++) {
        cur->next = remaining->copy();
        cur = cur->next;
      }
    }

    return std::move(res);
  }

  Polynomial &&operator*(T scale) const {
    Polynomial poly = copy();
    for (Node *current = poly.head->next; current; current = current->next) {
      current->coefficient *= scale;
    }
    return std::move(poly);
  }

  Polynomial &&operator/(T scale) const {
    Polynomial poly = copy();
    for (Node *current = poly.head->next; current; current = current->next) {
      current->coefficient /= scale;
    }
    return std::move(poly);
  }

  Polynomial &&operator-(Polynomial &poly) const { return operator+(poly * -1); }
};
} // namespace poly
