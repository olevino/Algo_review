#include<cstdio>
class Node {
public:
    int value;
    Node* next;
    Node(int x):value(x),next(nullptr){}
};

Node* merge(Node* begin, int n) {
    if(n == 1)
        return begin;
    int mid = n/2;
    Node* cur = begin;
    int term = 1;
    while(term != mid){
        cur = cur->next;
        term++;
    }
    Node* cur2 = cur->next;
    cur->next = nullptr;
    cur = merge(begin, mid);
    cur2 = merge(cur2, n - mid);
    Node* ans;
    if (cur->value <= cur2->value) {
        ans = cur;
        cur=cur->next;
    } else {
        ans = cur2;
        cur2 = cur2->next;
    }
    Node* cur3 = ans;
    while (cur != nullptr && cur2 != nullptr) {
        if (cur->value <= cur2->value) {
            cur3->next = cur;
            cur3 = cur;
            cur=cur->next;
        } else {
            cur3->next = cur2;
            cur3 = cur2;
            cur2=cur2->next;
        }
    }
    while (cur != nullptr) {
        cur3->next = cur;
        cur3 = cur;
        cur=cur->next;
    }
    while (cur2 != nullptr) {
        cur3->next = cur2;
        cur3 = cur2;
        cur2=cur2->next;
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