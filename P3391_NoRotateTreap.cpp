#include <bits/stdc++.h>
using namespace std;
struct Node
{
    int val, prioitry, size;
    Node *leftSon = nullptr;
    Node *rightSon = nullptr;
    int tag;
    Node(int _val): val(_val)
    {
        size = 1; tag = 0;
        prioitry = rand();
    }
    void updateSize()
    {
        size = 1;
        if (leftSon) size += leftSon->size;
        if (rightSon) size += rightSon->size;
    }
    void addTag()
    {
        tag = !tag;
        swap(leftSon, rightSon);
    }
    void pushDown()
    {
        if (!tag) return;
        tag = 0;
        if (leftSon) leftSon->addTag();
        if (rightSon) rightSon->addTag();
    }
};
Node *root;
pair<Node*, Node*> split(Node *cur, int rank)
{
    if (!cur) return {nullptr, nullptr};
    if (!rank) return {nullptr, cur};
    cur->pushDown();
    int LeftSonSize = cur->leftSon ? cur->leftSon->size : 0;
    if (rank <= LeftSonSize)
    {
        Node *l, *r;
        tie(l, r) = split(cur->leftSon, rank);
        cur->leftSon = r;
        cur->updateSize();
        return {l, cur};
    }
    else
    {
        Node *l, *r;
        tie(l, r) = split(cur->rightSon, rank - 1 - LeftSonSize);
        cur->rightSon = l;
        cur->updateSize();
        return {cur, r};
    }
}
Node *merge(Node *u, Node *v)
{
    if (!u && !v) return nullptr;
    if (!u) return v;
    if (!v) return u;
    if (u->prioitry <= v->prioitry)
    {
        v->pushDown();
        v->leftSon = merge(u, v->leftSon);
        v->updateSize();
        return v;
    }
    else
    {
        u->pushDown();
        u->rightSon = merge(u->rightSon, v);
        u->updateSize();
        return u;
    }
}
void reverse(int l, int r)
{
    Node *a, *b, *c;
    tie(b, c) = split(root, r);
    tie(a, b) = split(b, l - 1);
    if (b) b->addTag();
    root = merge(a, merge(b, c));
}
void build(Node *&cur, int l, int r)
{
    if (l > r) return;
    int mid = l + r >> 1;
    cur = new Node(mid);
    build(cur->leftSon, l, mid - 1);
    build(cur->rightSon, mid + 1, r);
    cur->updateSize();
}
void print(Node *cur)
{
    if (!cur) return;
    cur->pushDown();
    print(cur->leftSon);
    printf("%d ", cur->val);
    print(cur->rightSon);
}
int n, m, l, r;
int main()
{
    scanf("%d%d", &n, &m);
    build(root, 1, n);
    while (m--)
    {
        scanf("%d%d", &l, &r);
        reverse(l, r);
    }
    print(root);
    return 0;
}
/*
5 5
1 1
2 4
2 3
1 2
4 5
1 2 3 4 5
1 4 3 2 5
1 3 4 2 5
3 1 4 2 5
3 1 4 5 2*/