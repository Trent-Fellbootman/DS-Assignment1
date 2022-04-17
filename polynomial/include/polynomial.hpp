namespace poly {
template <typename T>

class Polynomial {
  struct Node {
    T coefficient;
    uint32_t exponential;
    Node *next;
  };
};
} // namespace poly
