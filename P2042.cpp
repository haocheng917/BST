#include <bits/stdc++.h>
using namespace std;
struct Node
{
    int val, priority, size = 1;
    Node *leftSon = nullptr;
    Node *rightSon = nullptr;
    bool reverseTag = 0, modifyTag = 0;
    int sum, leftMax, rightMax, maxSum, modifyVal;
    Node(int _val) : val(_val)
    {
        priority = rand();
        sum = leftMax = rightMax = maxSum = _val;
        update();
    }
    void update()
    {
        if (!leftSon && !rightSon)
        {
            size = 1;
            sum = leftMax = rightMax = maxSum = val;
            return;
        }
        if (!leftSon)
        {
            size = 1 + rightSon->size;
            sum = val + rightSon->sum;
            leftMax = val + max(0, rightSon->leftMax);
            rightMax = max(rightSon->rightMax, rightSon->sum + val);
            maxSum = max(leftMax, rightSon->maxSum);
            return;
        }
        if (!rightSon)
        {
            size = 1 + leftSon->size;
            sum = val + leftSon->sum;
            leftMax = max(leftSon->leftMax, leftSon->sum + val);
            rightMax = val + max(0, leftSon->rightMax);
            maxSum = max(rightMax, leftSon->maxSum);
            return;
        }
        size = 1 + leftSon->size + rightSon->size;
        sum = leftSon->sum + val + rightSon->sum;
        leftMax = max(leftSon->leftMax, max(leftSon->sum + val,
                                            leftSon->sum + val + rightSon->leftMax));
        rightMax = max(rightSon->rightMax, max(rightSon->sum + val,
                                               rightSon->sum + val + leftSon->rightMax));
        maxSum = max(max(leftSon->maxSum, rightSon->maxSum),
                     max(0, leftSon->rightMax) + val + max(0, rightSon->leftMax));
    }
    void reverse()
    {
        reverseTag = !reverseTag;
        swap(leftSon, rightSon);
        swap(leftMax, rightMax);
    }
    void modify(int _val)
    {
        modifyTag = true;
        val = modifyVal = _val;
        sum = size * _val;
        if (_val > 0)
        {
            leftMax = rightMax = maxSum = _val * size;
        }
        else
        {
            leftMax = rightMax = maxSum = _val;
        }
    }
    void pushDown()
    {
        if (modifyTag)
        {
            modifyTag = 0;
            if (leftSon)
                leftSon->modify(modifyVal);
            if (rightSon)
                rightSon->modify(modifyVal);
        }
        if (reverseTag)
        {
            reverseTag = 0;
            if (leftSon)
                leftSon->reverse();
            if (rightSon)
                rightSon->reverse();
        }
    }
};
Node *root;
pair<Node *, Node *> split(Node *cur, int rank)
{
    if (!cur)
        return {nullptr, nullptr};
    if (!rank)
        return {nullptr, cur};
    int leftSonSize = cur->leftSon ? cur->leftSon->size : 0;
    cur->pushDown();
    if (rank <= leftSonSize)
    {
        Node *l, *r;
        tie(l, r) = split(cur->leftSon, rank);
        cur->leftSon = r;
        cur->update();
        return {l, cur};
    }
    else
    {
        Node *l, *r;
        tie(l, r) = split(cur->rightSon, rank - leftSonSize - 1);
        cur->rightSon = l;
        cur->update();
        return {cur, r};
    }
}
Node *merge(Node *u, Node *v)
{
    if (!u && !v)
        return nullptr;
    if (!u)
        return v;
    if (!v)
        return u;
    if (u->priority <= v->priority)
    {
        v->pushDown();
        v->leftSon = merge(u, v->leftSon);
        v->update();
        return v;
    }
    else
    {
        u->pushDown();
        u->rightSon = merge(u->rightSon, v);
        u->update();
        return u;
    }
}
Node *build(vector<int> &c, int l, int r)
{
    if (l > r)
        return nullptr;
    int mid = l + r >> 1;
    return merge(build(c, l, mid - 1), merge(new Node(c[mid]), build(c, mid + 1, r)));
}
void insert(int pos, int tot, vector<int> &c)
{
    Node *l, *r;
    tie(l, r) = split(root, pos);
    root = merge(merge(l, build(c, 0, tot - 1)), r);
}
void clear(Node *&cur)
{
    if (!cur)
        return;
    clear(cur->leftSon);
    clear(cur->rightSon);
    delete cur;
}
void del(int pos, int tot)
{
    Node *l, *mid, *r;
    tie(l, mid) = split(root, pos - 1);
    tie(mid, r) = split(mid, tot);
    clear(mid);
    root = merge(l, r);
}
void makeSame(int pos, int tot, int c)
{
    if (!tot) return;
    Node *l, *mid, *r;
    tie(l, mid) = split(root, pos - 1);
    tie(mid, r) = split(mid, tot);
    mid->modify(c);
    root = merge(l, merge(mid, r));
}
void reverse(int pos, int tot)
{
    if (!tot) return;
    Node *l, *mid, *r;
    tie(l, mid) = split(root, pos - 1);
    tie(mid, r) = split(mid, tot);
    mid->reverse();
    root = merge(l, merge(mid, r));
}
int getSum(int pos, int tot)
{
    if (!tot) return 0;
    Node *l, *mid, *r;
    tie(l, mid) = split(root, pos - 1);
    tie(mid, r) = split(mid, tot);
    int t = mid->sum;
    root = merge(l, merge(mid, r));
    return t;
}
int maxSum()
{
    return root->maxSum;
}
int n, m, pos, tot, cc;
vector<int> c;
int main()
{
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++)
    {
        int x;
        scanf("%d", &x);
        c.push_back(x);
    }
    root = build(c, 0, n - 1);
    while (m--)
    {
        string op;
        cin >> op;

        switch (op[2])
        {
        case 'S':
        {
            int pos, tot;
            scanf("%d%d", &pos, &tot);
            c.clear();
            for (int i = 1; i <= tot; i++)
            {
                int x;
                scanf("%d", &x);
                c.push_back(x);
            }
            insert(pos, tot, c);
            break;
        }
        case 'L':
        {
            int pos, tot;
            scanf("%d%d", &pos, &tot);
            del(pos, tot);
            break;
        }
        case 'K':
        {
            int pos, tot;
            scanf("%d%d%d", &pos, &tot, &cc);
            makeSame(pos, tot, cc);
            break;
        }
        case 'V':
        {
            int pos, tot;
            scanf("%d%d", &pos, &tot);
            reverse(pos, tot);
            break;
        }
        case 'T':
        {
            int pos, tot;
            scanf("%d%d", &pos, &tot);
            printf("%d\n", getSum(pos, tot));
            break;
        }
        case 'X':
        {
            printf("%d\n", maxSum());
            break;
        }
        }
    }
    return 0;
}
/*
10 100
1 -2 3 -4 5 -6 7 8 -9 0
xxX
KKK 5 2 -2
ttT 6 3
xxX

1 -2 3 -4 -2 -2 7 8 -9 0
*/