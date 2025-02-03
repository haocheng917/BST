#include <random>
#include <vector>
#include <tuple>

#ifndef NO_ROTATE_TREAP_H
#define NO_ROTATE_TREAP_H

std::mt19937 Rand;

template <typename Type>
class NoRotateTreap
{
    struct Node
    {
        Type val;
		int priority, cnt = 1, size = 1;
		Node *children[2];
		Node(Type val) : val(val) {
			priority = Rand();
		}
		void update() {
			size = cnt;
			if (leftSon) size += leftSon->size;
			if (rightSon) size += rightSon->size;
		}
    };
    
private:
    
    class NoSuchValueException : std::exception {
    private:
        const char *message;
    public:
        NoSuchValueException(const char *msg) : message(msg) {}
        const char *what() const noexcept override { 
            return message; 
        }
    };

    Node *root = nullptr;

    std::pair<Node*, Node*> split(Node *cur, int val) {
        if (cur == nullptr) return {nullptr, nullptr};
        if (!cmp(cur->val, val)) {
#if (__cplusplus >= 201703L)
            auto [left, right] = split(cur->rightSon, val);
#elif
            Node *left, *right;
            tie(left, right) = split(cur->rightSon, val);
#endif
            cur->rightSon = left;
            cur->update();
            return {cur, right};
        } else {
#if (__cplusplus >= 201703L)
            auto [left, right] = split(cur->leftSon, val);
#elif
            Node *left, *right;
            tie(left, right) = split(cur->leftSon, val);
#endif
            cur->leftSon = right;
            cur->updateSize();
            return {left, cur};
        }
    }


    Node *merge(Node *u, Node *v) {
        if (u == nullptr && v == nullptr) return nullptr;
        if (v == nullptr) return u;
        if (u == nullptr) return v;
        if (u->priority < v->priority) {
            v->leftSon = merge(u, v->leftSon);
            v->updateSize();
            return v;
        } else {
            u->rightSon = merge(u->rightSon, v);
            u->updateSize();
            return u;
        }
    }

    std::tuple<Node*, Node*, Node*> splitByRank(Node *cur, int rank) {
        if (cur == nullptr) return {nullptr, nullptr, nullptr};
        int LeftSonSize = cur->leftSon ? cur->leftSon->size : 0;
        if (rank <= LeftSonSize) {
            Node *l, *mid, *r;
            std::tie(l, mid, r) = splitByRank(cur->leftSon, rank);
            cur->leftSon = r;
            cur->update();
            return {l, mid, cur};
        } else if (rank <= LeftSonSize + cur->count) {
            Node *l = cur->leftSon, *r = cur->rightSon;
            cur->leftSon = cur->rightSon = nullptr;
            return {l, cur, r};
        } else {
            Node *l, *mid, *r;
            std::tie(l, mid, r) = splitByRank(cur->rightSon, rank - LeftSonSize - cur->count);
            cur->rightSon = l;
            cur->update();
            return {cur, mid, r};
        }
    }

    void print(Node *cur, std::vector<Type> &out) {
        if (cur == nullptr) return;
        print(cur->leftSon, out);
        out.push_back(cur->val);
        print(cur->rightSon, out);
    }

public:
    
    ~NoRotateTreap() {
        
    }

    int size() {
        return root ? root->size : 0;
    }

    int count(Type val) {
        std::pair<Node*, Node*> temp = split(root, val);
        std::pair<Node*, Node*> l = split(temp.first, val - 1);
        root = merge(merge(l.first, l.second), temp.second);
        return l.second ? l.second->count : 0;
    }

    void insert(int val) {
        Node *left, *mid, *right;
        tie(left, right) = split(root, val);
        tie(left, mid) = split(left, val - 1);
        if (mid) {
            mid->cnt++, mid->size++;
        } else {
            mid = new Node(val);
        }
        root = merge(merge(left, mid), right);
    }

    void erase(int val) {
        Node *left, *mid, *right;
        tie(left, right) = split(root, val);
        tie(left, mid) = split(left, val - 1);
        if (mid) {
            if (mid->cnt > 1) {
                mid->cnt--, mid->size--;
            } else {
                delete mid;
                mid = nullptr;
            }
        }
        root = merge(merge(left, mid), right);
    }

    int queryRank(Type val) {
        Node *l, *r;
        std::tie(l, r) = split(root, val - 1);
        int res = (l ? l->size : 0) + 1;
        root = merge(l, r);
        return res;
    }

    Type queryKth(int rank) {
        if (rank > root->size) {
            throw NoSuchValueException("No so many vals in the treap");
        }
        Node *l, *mid, *r;
        std::tie(l, mid, r) = splitByRank(root, rank);
        root = merge(l, merge(mid, r));
        return mid->val;
    }

    Type predecessor(Type val) {
        Node *l, *r;
        std::tie(l, r) = split(root, val - 1);
        if (l == nullptr) {
            root = merge(l, r);
            throw NoSuchValueException("No lower val in the treap");
            return Type();
        }
        Node *ll, *mid, *rr;
        std::tie(ll, mid, rr) = splitByRank(l, l->size);
        root = merge(merge(ll, mid), r);
        return mid->val;
    }

    Type successor(Type val) {
        Node *l, *r;
        std::tie(l, r) = split(root, val);
        if (r == nullptr) {
            root = merge(l, r);
            throw NoSuchValueException("No higher val in the treap");
            return Type();
        }
        Node *ll, *mid, *rr;
        std::tie(ll, mid, rr) = splitByRank(r, 1);
        root = merge(l, merge(mid, rr));
        return mid->val;
    }

    operator std::vector<Type>() {
        std::vector<Type> temp;
        print(root, temp);
        return temp;
    }
};

#endif

#include <iostream>
using namespace std;
NoRotateTreap<int> tr;
int n, opt, x;
int main()
{
	scanf("%d", &n);
	while (n--)
	{
		scanf("%d %d", &opt, &x);
		switch (opt)
		{
			case 1: tr.insert(x); break;
			case 2: tr.erase(x); break;
			case 3: printf("%d\n", tr.queryRank(x)); break;
			case 4: printf("%d\n", tr.queryKth(x)); break;
			case 5: printf("%d\n", tr.predecessor(x)); break;
			case 6: printf("%d\n", tr.successor(x)); break;
            case 7: printf("%d\n", tr.count(x)); break;
            case 8: printf("%d\n", tr.size()); break;
			case 9: for (int x : vector<int>(tr)) printf("%d ", x); break;
		}
	}
	return 0;
}