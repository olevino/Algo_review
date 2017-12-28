#include <cstdio>
#include <iostream>

template <typename T>
    class Node{
public:
    T value;
    Node* next;
    Node(T x) : value(x), next(nullptr) {}
};

template <typename T>
Node<T>* split(Node<T>* begin, size_t n) {
    size_t mid = n / 2;
    Node<T>* left = begin;
    size_t term = 1;
    while (term != mid){
        left = left->next;
        term++;
    }
    Node<T>* right = left->next;
    left->next = nullptr;
    return right;
}

template <typename T>
Node<T>* merge(Node<T>* left, Node<T>* right, size_t s1, size_t s2) {
    Node<T>* ans;
    if (left->value <= right->value) {
        ans = left;
        left=left->next;
    } else {
        ans = right;
        right = right->next;
    }
    Node<T>* last = ans;
    while (left != nullptr && right != nullptr) {
        if (left->value <= right->value) {
            last->next = left;
            last = left;
            left=left->next;
        } else {
            last->next = right;
            last = right;
            right=right->next;
        }
    }
    while (left != nullptr) {
        last->next = left;
        last = left;
        left=left->next;
    }
    while (right != nullptr) {
        last->next = right;
        last = right;
        right=right->next;
    }
    return ans;
}

template <typename T>
Node<T>* sort(Node<T>* begin, size_t n) {
    if (n == 1)
        return begin;
    Node<T>* right = split(begin, n);
    Node<T>* left = begin;
    left = sort(begin, n / 2);
    right = sort(right, n - n / 2);
    return merge(left, right, n / 2, n - n / 2);
}

template <typename T>
void printList(Node<T>* begin) {
    while (begin != nullptr){
        std::cout << begin->value << ' ';
        begin = begin->next;
    }
    std::cout << std::endl;
}

template <typename T>
void deleteList(Node<T>* begin) {
    while (begin != nullptr){
        Node<T>* tmp = begin;
        begin = begin->next;
        delete tmp;
    }
}

int main() {
    size_t n;
    int v;
    std::cin >> n >> v;
    Node<int>* begin = new Node<int>(v);
    Node<int>* cur = begin;
    for (int i = 0; i != n-1; i++) {
        std::cin >> v;
        Node<int>* temp = new Node<int>(v);
        cur->next = temp;
        cur = temp;
    }
    begin = sort(begin, n);
    printList(begin);
    deleteList(begin);
}
