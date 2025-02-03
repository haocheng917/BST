#include <bits/stdc++.h>
using namespace std;
mt19937 rnd;
int read() {
	int x=0, f=1; char ch = getchar();
	while (ch > '9' || ch < '0') { 
        if (ch == '-') f = -1;
        ch = getchar();
    }
	while (ch <= '9' && ch >= '0') {
        x = (x << 3) + (x << 1) + (ch ^ 48);
        ch = getchar();
    }
	return x * f;
}
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
int getMax(Node *rt) {
    while (rt->rightSon) rt = rt->rightSon;
    return rt->val;
}
int getMin(Node *rt) {
    while (rt->leftSon) rt = rt->leftSon;
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
char s[20];
int main() {
    n = read(), m = read();
    for (int i = 1; i <= n; i++) {
        a[i] = read();
        Map[i] = a[i];
        insert(rt1, a[i]);
        if (i - 1) insert(rt0, abs(a[i] - a[i - 1]));
        min_sort_gap = min(min_sort_gap, min(a[i] - pre(a[i]), suc(a[i]) - a[i]));
    }
    while (m--) {
        scanf("%s", s);
        if (s[0] == 'I') {
            int i, k;
            i = read();
            k = read();
            if (i != n) {
                erase(rt0, abs(Map[i] - a[i + 1]));
                insert(rt0, abs(a[i + 1] - k));
            }
            insert(rt0, abs(Map[i] - k));
            insert(rt1, Map[i] = k);
            min_sort_gap = min(min_sort_gap, min(k - pre(k), suc(k) - k));
        } else if (s[4] == 'G') {
            cout << getMin(rt0) << '\n';
        } else {
            cout << min_sort_gap << '\n';
        }
    }
    return 0;
}