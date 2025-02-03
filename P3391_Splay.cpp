#include <bits/stdc++.h>
using namespace std;
struct Node {
    int val, size = 1;
    Node *leftSon = nullptr, *rightSon = nullptr;
    Node *father;
    bool tag = 0;
    Node(int val, Node* fa)
        : val(val), father(fa) {    
    }
    void update() {
        size = 1;
        if (leftSon) size += leftSon->size;
        if (rightSon) size += rightSon->size;
    }
    bool isLeftSon() {
        return this == father->leftSon;
    }
    void addTag() {
        tag = !tag;
        swap(leftSon, rightSon);
    }
    void pushDown() {
        if (!tag) return;
        if (leftSon) leftSon->addTag();
        if (rightSon) rightSon->addTag();
        tag = 0;
    }
};
Node *root;
void rotate(Node *&cur) {
    Node *father = cur->father;
    Node *grandfather = father->father;
    if (grandfather) {
        grandfather->pushDown();
        if (father->isLeftSon()) {
            grandfather->leftSon = cur;
        } else {
            grandfather->rightSon = cur;
        }
    }
    father->pushDown();
    if (cur->isLeftSon()) {
        father->leftSon = cur->rightSon;
        if (father->leftSon) {
            father->leftSon->father = father;
        }
        cur->rightSon = father;
    } else {
        father->rightSon = cur->leftSon;
        if (father->rightSon) {
            father->rightSon->father = father;
        }
        cur->leftSon = father;
    }
    cur->father = grandfather;
    father->father = cur;
    father->update();
    cur->update();
}
void splay(Node *cur) {
    for (Node *fa; fa = cur->father;) {
        if (fa->father) {
            rotate(fa->isLeftSon() == cur->isLeftSon() ? fa : cur);
        }
        rotate(cur);
    }
    root = cur;
}
void splay2(Node *cur) {
    for (Node *fa; fa = cur->father, cur->father != root;) {
        if (fa->father != root) {
            rotate(fa->isLeftSon() == cur->isLeftSon() ? fa : cur);
        }
        rotate(cur);
    }
}
Node *build(int l, int r, Node *fa = nullptr) {
    if (l > r) return nullptr;
    int mid = l + r >> 1;
    Node* t = new Node(mid, fa);
    t->leftSon = build(l, mid - 1, t);
    t->rightSon = build(mid + 1, r, t);
    t->update();
    return t;
}
Node *Kth(int k, Node *cur = root) {
    cur->pushDown();
    int leftSonSize = cur->leftSon ? cur->leftSon->size : 0;
    if (k <= leftSonSize) {
        return Kth(k, cur->leftSon);
    } else if (k <= leftSonSize + 1) {
        return cur;
    } else {
        return Kth(k - leftSonSize - 1, cur->rightSon);
    }
}
int n, m, p, q;
void print(Node *cur = root) {
    if (!cur) return;
    cur->pushDown();
    print(cur->leftSon);
    if (cur->val > 0 && cur->val <= n)
        cout << cur->val << ' ';
    print(cur->rightSon);
}
int main() {
    scanf("%d%d", &n, &m);
    root = build(0, n + 1);
    while (m--) {
        scanf("%d%d", &p, &q);
        splay(Kth(p));
        splay2(Kth(q + 2));
        root->rightSon->leftSon->addTag();
    }
    print();
    return 0;
}