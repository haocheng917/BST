#include <functional>

#ifndef SPLAY_BINARY_SEARCH_SET_H
#define SPLAY_BINARY_SEARCH_SET_H

template<class Type, class Compare = std::less<Type> >
class Splay {

    struct Node {
        Type val;
        int cnt = 1, size = 1;
        Node *leftSon = nullptr;
        Node *rightSon = nullptr;
        Node *father = nullptr;
        Node(Type val, Node* fa = nullptr): val(val), father(fa) {
        }

        void update() {
            size = cnt;
            if (leftSon) size += leftSon->size;
            if (rightSon) size += rightSon->size;
        }

        bool isLeftSon() {
            return this == father->leftSon;
        }
    };

protected:

    Node *root = nullptr;
    Compare cmp;

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
         for (Node *fa; fa = cur->father;) {
            if (fa->father) {
                rotate(cur->isLeftSon() == fa->isLeftSon() ? fa : cur);
            }
            rotate(cur);
        }
        return root = cur;
    }

    void _insert(Node *&cur, Type val, Node *father = nullptr) {
        if (!cur) {
            cur = new Node(val, father);
            if (father) father->update();
            splay(cur);
            return;
        }
        if (cur->val == val) {
            cur->cnt++;
            cur->update();
            splay(cur);
        } else if (cmp(val, cur->val)){
            _insert(cur->leftSon, val, cur);
        } else {
            _insert(cur->rightSon, val, cur);
        }
    }

    Node *find(Type val) {
        Node *cur = root;
        while (cur) {
            if (cmp(val, cur->val)) {
                cur = cur->leftSon;
            } else if (val == cur->val) {
                return splay(cur);
            } else {
                cur = cur->rightSon;
            }
        }
        return nullptr;
    }

    Type _queryKth(Node *&cur, int rank) {
        int leftSonSize = cur->leftSon ? cur->leftSon->size : 0;
        if (rank <= leftSonSize) {
            return _queryKth(cur->leftSon, rank);
        } else if (rank <= leftSonSize + cur->cnt) {
            return splay(cur)->val;
        } else if (rank <= cur->size) {
            return _queryKth(cur->rightSon, rank - leftSonSize - cur->cnt);
        }
    }

    Node *_predecessor() {
        Node *cur = root->leftSon;
        if (!cur) return nullptr;
        while (cur->rightSon) {
            cur = cur->rightSon;
        }
        return splay(cur);
    }

    Node *_successer() {
        Node *cur = root->rightSon;
        if (!cur) return nullptr;
        while (cur->leftSon) {
            cur = cur->leftSon;
        }
        return splay(cur);
    }

public:

    void insert(Type val) {
        _insert(root, val);
    }

    bool erase(Type val) {
        Node *cur = find(val);
        if (cur == nullptr) return false;
        if (cur->cnt > 1) {
            cur->cnt--;
            cur->update();
            return true;
        }
        if (!cur->leftSon && !cur->rightSon) {
            delete cur;
            root = nullptr;
        } else if (!cur->leftSon) {
            root = cur->rightSon;
            root->father = nullptr;
        } else if (!cur->rightSon) {
            root = cur->leftSon;
            root->father = nullptr;
        } else {
            cur->leftSon->father = nullptr;
            splay(_predecessor());
            root->rightSon = cur->rightSon;
            if (cur->rightSon) cur->rightSon->father = root;
            delete cur;
        }
        return true;
    }

    Type queryKth(int rank) {
        return _queryKth(root, rank);
    }

    int queryRank(Type val) {
        _insert(root, val);
        int res = root->leftSon ? root->leftSon->size + 1 : 1;
        erase(val);
        return res;
    }

    Type predecessor(Type val) {
        _insert(root, val);
        int res = _predecessor()->val;
        erase(val);
        return res;
    }

    Type successor(Type val) {
        _insert(root, val);
        int res = _successer()->val;
        erase(val);
        return res;
    }
};

#endif

#include <iostream>
using namespace std;
Splay<int> tr;
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
		}
	}
	return 0;
}

/*

*/