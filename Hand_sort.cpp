#include <cstdio>
class Node {
public:
    int value;
    Node* next;
    Node(int x) : value(x), next(nullptr) {}
};

Node* merge(Node* begin, int n) {
    if (n == 1)
        return begin;
    int mid = n / 2;
    Node* left = begin;
    int term = 1;
    while (term != mid){
        left = left->next;
        term++;
    }
    Node* right = left->next;
    left->next = nullptr;
    left = merge(begin, mid);
    right = merge(right, n - mid);
    Node* ans;
    if (left->value <= right->value) {
        ans = left;
        left=left->next;
    } else {
        ans = right;
        right = right->next;
    }
    Node* last = ans;
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



int main() {
    int n, v;
    scanf("%d %d", &n, &v);
    Node* begin = new Node(v);
    Node* cur = begin;
    for (int i = 0; i != n-1; i++) {
        scanf("%d", &v);
        Node* temp = new Node(v);
        cur->next = temp;
        cur = temp;
    }
    begin = merge(begin, n);
    while (begin != nullptr) {
        printf("%d ", begin->value);
        Node* temp = begin->next;
        delete begin;
        begin = temp;
    }
    printf("\n");
}
