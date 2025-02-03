#include <bits/stdc++.h>
using namespace std;
struct Node {
    int val, size = 1;
    Node *leftSon = nullptr;
    Node *rightSon = nullptr;
    Node *father = nullptr;
    Node(int val, Node *fa): val(val), father(fa) {}
    bool isLeftSon() { return this == father->leftSon; }
    void update() {
        size = 1;
        if (leftSon) size += leftSon->size;
        if (rightSon) size += rightSon->size;
    }
};
Node *root, *Map[100000];
int p[100000], n, m;
Node *build(int p[], int l, int r, Node *fa = nullptr) {
    if (l > r) return nullptr;
    int mid = l + r >> 1;
    Node *t = new Node(p[mid], fa);
    t->leftSon = build(p, l, mid - 1, t);
    t->rightSon = build(p, mid + 1, r, t);
    t->update();
    return Map[p[mid]] = t;
}
Node *pre() {
    Node *x = root->leftSon;
    while (x->rightSon) x = x->rightSon;
    return x;
}
Node *suc() {
    Node *x = root->rightSon;
    while (x->leftSon) x = x->leftSon;
    return x;
}
void rotate(Node *&cur) {
    Node *father = cur->father;
    Node *grandfather = father->father;
    if (grandfather) {
        if (father->isLeftSon()) {
            grandfather->leftSon = cur;
        } else {
            grandfather->rightSon = cur;
        }
    }
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
Node* splay(Node *cur) {
    for (Node *fa; fa = cur->father; ) {
        if (fa->father) {
            rotate(cur->isLeftSon() == fa->isLeftSon() ? fa : cur);
        } 
        rotate(cur);
    }
    return cur;
}
void top(int s) {
    root = splay(Map[s]);
    if (!root->leftSon) return;
    if (!root->rightSon) {
        root->rightSon = root->leftSon;
        root->leftSon = nullptr;
        return; 
    }
    root->rightSon->father = nullptr;
    root->rightSon = splay(suc());
    root->rightSon->father = root;
    swap(root->leftSon, root->rightSon->leftSon);
    root->rightSon->leftSon->father = root->rightSon;
    root->rightSon->update();
    root->update();
}
void bottom(int s) {
    root = splay(Map[s]);
    if (!root->rightSon) return;
    if (!root->leftSon) {
        root->leftSon = root->rightSon;
        root->rightSon = nullptr;
        return; 
    }
    root->leftSon->father = nullptr;
    root->leftSon = splay(pre());
    root->leftSon->father = root;
    swap(root->rightSon, root->leftSon->rightSon);
    root->leftSon->rightSon->father = root->leftSon;
    root->leftSon->update();
    root->update();
}  
void insert(int s, int t) {
    if (!t) return;
    root = splay(Map[s]);
    Node *x = (t == 1 ? suc() : pre());
    if (x->father) {
        if (x->isLeftSon()) {
            x->father->leftSon = root;
        } else {
            x->father->rightSon = root;
        }
    }
    swap(root->leftSon, x->leftSon);
    swap(root->rightSon, x->rightSon);
    swap(root->father, x->father);
    if (root->leftSon) root->leftSon->father = root;
    if (root->rightSon) root->rightSon->father = root;
    if (x->leftSon) x->leftSon->father = x;
    if (x->rightSon) x->rightSon->father = x;
    root = splay(root);
}
int Ask(int s) {
    root = splay(Map[s]);
    return root->leftSon ? root->leftSon->size : 0;
}
int Kth(int k, Node *cur = root) {
    int lSSize = cur->leftSon ? cur->leftSon->size : 0;
    if (k <= lSSize) {
        return Kth(k, cur->leftSon);
    } else if (k <= lSSize + 1) {
        return cur->val;
    } else {
        return Kth(k - lSSize - 1, cur->rightSon);
    }
}
int main() {
    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        cin >> p[i];
    }
    root = build(p, 1, n);
    while (m--) {
        string op; int s, t;
        cin >> op >> s;
        if (op == "Top") top(s);
        else if (op == "Bottom") bottom(s);
        else if (op == "Insert") {
            cin >> t;
            insert(s, t);
        } else if (op == "Ask") {
            printf("%d\n", Ask(s));
        } else {
            printf("%d\n", Kth(s));
        }
    }
    return 0;
}
/*
1 3 2 7 5 8 10 4 9 6
5 1 2 7 8 10 4 9 6 3
6 5 1 2 7 4 10 9 6 3
10 10
1 3 2 7 5 8 10 4 9 6
Top 5
Bottom 3
Top 6
Insert 4 -1
Query 5
Query 2
Ask 2
*/