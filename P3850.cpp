#include <bits/stdc++.h>
using namespace std;
int n, m, q;
string s[300];
mt19937 rnd;
struct Node {
    string name;
    size_t size;
    int priority;
    Node *leftSon{nullptr}, *rightSon{nullptr};
    Node(string name, Node* leftSon, Node *rightSon)
        : name(name), leftSon(leftSon), rightSon(rightSon) {
        priority = rnd();
        update();
    }
    void update() {
        size = 1;
        if (leftSon) size += leftSon->size;
        if (rightSon) size += rightSon->size;
    }
};
Node *root;
Node *build(int l, int r) {
    if (l > r) return nullptr;
    int mid = l + r >> 1;
    return new Node(s[mid], build(l, mid - 1), build(mid + 1, r));
}
pair<Node*, Node*> split(Node *cur, int rank) {
    if (!cur) return {nullptr, nullptr};
    if (!rank) return {nullptr, cur};
    int LeftSonSize = cur->leftSon ? cur->leftSon->size : 0;
    if (rank <= LeftSonSize) {
        Node *l, *r;
        tie(l, r) = split(cur->leftSon, rank);
        cur->leftSon = r;
        cur->update();
        return {l, cur};
    } else {
        Node *l, *r;
        tie(l, r) = split(cur->rightSon, rank - LeftSonSize - 1);
        cur->rightSon = l;
        cur->update();
        return {cur, r};
    }
}
Node *merge(Node *u, Node *v) {
    if (!u) return v;
    if (!v) return u;
    if (u->priority > v->priority) {
        u->rightSon = merge(u->rightSon, v);
        u->update();
        return u;
    } else {
        v->leftSon = merge(u, v->leftSon);
        v->update();
        return v;
    }
}
void print(vector<string> *v, Node *cur) {
    if (cur) {
        print(v, cur->leftSon);
        v->push_back(cur->name);
        print(v, cur->rightSon);
    }
}
vector<string> v;
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        cin >> s[i];
    }
    root = build(1, n);
    scanf("%d", &m);
    while (m--) {
        string s;
        int rank;
        cin >> s >> rank;
        Node *l, *r;
        tie(l, r) = split(root, rank);
        root = merge(merge(l, new Node(s, nullptr, nullptr)), r);
    }
    print(&v, root);
    scanf("%d", &q);
    while (q--) {
        int id;
        scanf("%d", &id);
        cout << v[id] << '\n';
    }
    return 0;
}