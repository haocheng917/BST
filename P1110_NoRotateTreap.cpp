#include <bits/stdc++.h>
using namespace std;
mt19937 rnd;
struct Node {
    int val, priority, size = 1, cnt = 1;
    Node *leftSon = nullptr;
    Node *rightSon = nullptr;
    Node(int val): val(val) {
        priority = rnd();
    }
    void update() {
        size = 1;
        if (leftSon) size += leftSon->size;
        if (rightSon) size += rightSon->size;
    }
};
Node *rt0, *rt1;
pair<Node *, Node *> split(Node *cur, int val) {
    if (!cur) return {nullptr, nullptr};
    if (cur->val > val) {
        Node *l, *r; tie(l, r) = split(cur->leftSon, val);
        cur->leftSon = r;
        cur->update();
        return {l, cur};
    } else {
        Node *l, *r; tie(l, r) = split(cur->rightSon, val);
        cur->rightSon = l;
        cur->update();
        return {cur, r};
    }
}
Node *merge(Node *u, Node *v) {
    if (!u && !v) return nullptr;
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
#define abs(x) ((x) > 0 ? (x) : -(x))
const int N = 1e6, inf = 0x3f3f3f3f;
int n, m, a[N], min_sort_gap = inf, Map[N];
void insert(Node *&rt, int x) {
    Node *l, *m, *r;
    tie(m, r) = split(rt, x);
    tie(l, m) = split(m, x - 1);
    if (m) m->cnt++, m->size++;
    else m = new Node(x);
    rt = merge(l, merge(m, r));
}
void erase(Node *&rt, int x) {
    Node *l, *m, *r;
    tie(m, r) = split(rt, x);
    tie(l, m) = split(m, x - 1);
    if (m->cnt > 1) m->cnt--, m->size--;
    else { delete m; m = nullptr; }
    rt = merge(l, merge(m, r));
}
int getMax(Node *&rt) {
    if (rt->rightSon) return getMax(rt->rightSon);
    return rt->val;
}
int getMin(Node *&rt) {
    if (rt->leftSon) return getMin(rt->leftSon);
    return rt->val;
}
int pre(int k) {
    Node *l, *m, *r;
    tie(m, r) = split(rt1, k);
    tie(l, m) = split(m, k - 1);
    int res;
    if (m->cnt > 1) res = k;
    else res = l ? getMax(l) : -inf;
    rt1 = merge(l, merge(m, r));
    return res;
}
int suc(int k) {
    Node *l, *m, *r;
    tie(l, r) = split(rt1, k);
    tie(l, m) = split(l, k - 1);
    int res;
    if (m->cnt > 1) res = k;
    else res = r ? getMin(r) : inf;
    rt1 = merge(l, merge(m, r));
    return res;
}
int main() {
//    freopen("D:\\BST\\SomeProblems'TasksData\\P1110_2.in", "r", stdin);
    ios::sync_with_stdio(false);
    cin.tie(0);
    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        Map[i] = a[i];
        insert(rt1, a[i]);
        if (i - 1) insert(rt0, abs(a[i] - a[i - 1]));
        min_sort_gap = min(min_sort_gap, min(a[i] - pre(a[i]), suc(a[i]) - a[i]));
    }
    while (m--) {
        string op;
        cin >> op;
        if (op == "INSERT") {
            int i, k;
            cin >> i >> k;
            if (i != n) {
                erase(rt0, abs(Map[i] - a[i + 1]));
                insert(rt0, abs(a[i + 1] - k));
            }
            insert(rt0, abs(Map[i] - k));
            insert(rt1, Map[i] = k);
            min_sort_gap = min(min_sort_gap, min(k - pre(k), suc(k) - k));
        } else if (op == "MIN_GAP") {
            cout << getMin(rt0) << '\n';
        } else if (op == "MIN_SORT_GAP") {
            cout << min_sort_gap << '\n';
        }
    }
    return 0;
}