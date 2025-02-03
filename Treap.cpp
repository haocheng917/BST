#include <random>
#include <vector>

#ifndef TREAP_BINARY_SEARCH_SET_H
#define TREAP_BINARY_SEARCH_SET_H

std::mt19937 Rand; 

template <class Type, class Compare = std::less<Type> >
class Treap {

	struct Node {
		Type val;
		int priority, cnt = 1, size = 1;
		Node *leftSon = nullptr;
		Node *rightSon = nullptr;
		Node(Type val): val(val) {
			priority = Rand();
		}
		void update() {
			size = cnt;
			if (leftSon) size += leftSon->size;
			if (rightSon) size += rightSon->size;
		}
	}

protected:

	Node *root = nullptr;
	Compare cmp;

	void leftRotate(Node *&cur) {
		Node *tmp = cur->rightSon;
		cur->rightSon = tmp->leftSon;
		tmp->leftSon = cur;
		cur->update(), tmp->update();
		cur = tmp;
	}

	void rightRotate(Node *&cur) {
		Node *tmp = cur->leftSon; // x
		cur->leftSon = tmp->rightSon; // 移动 B 子树
		tmp->rightSon = cur; // 移动 f
		cur->update(), tmp->update(); // 更新
		cur = tmp; // 将根改成 tmp(x节点)
	}

	void _insert(Node *&cur, Type val) {
		if (!cur) {
			cur = new Node(val);
			return;
		}
		if (cur->val == val) {
			cur->cnt++;
		} else if (cmp(val, cur->val)) { // val < cur->val
			_insert(cur->leftSon, val);
			if (cur->leftSon->priority > cur->priority) {
				rightRotate(cur);
			}
		} else {
			_insert(cur->rightSon, val);
			if (cur->rightSon->priority > cur->priority) {
				leftRotate(cur);
			}
		}
		cur->update();
	}

	bool _erase(Node *&cur, Type val) {
		if (cur == nullptr) return false;
		if (val < cur->val) {
			if (_erase(cur->leftSon, val)) {
				cur->update();
				return true;
			}
			return false;
		} else if (val > cur->val) {
			if (_erase(cur->rightSon, val)) {
				cur->update();
				return true;
			}
			return false;
		}
		if (cur->count > 1) {
			cur->count--, cur->size--;
		} else if (cur->leftSon && cur->rightSon) {
			if (cur->leftSon->priority < cur->rightSon->priority) {
				rightRotate(cur);
				_erase(cur->rightSon, val);
			} else {
				leftRotate(cur);
				_erase(cur->leftSon, val);
			}
			cur->update();
		} else if (cur->leftSon) {
			Node* tmp = cur;
			cur = tmp->leftSon;
			delete tmp;
		} else if (cur->rightSon) {
			Node* tmp = cur;
			cur = tmp->rightSon;
			delete tmp;
		} else {
			delete cur;
			cur = nullptr;
		}
		return true;
	}

	Type _successor(Node *&cur, Type val) {
		if (cmp(cur->val, val) || cur->val == val) {
			if (cur->rightSon) return _successor(cur->rightSon, val);
			else throw NoSuchValueException("No greater value in the tree");
		}
		if (cur->leftSon) {
			try {
				return _successor(cur->leftSon, val);
			} catch(const NoSuchValueException &e) {
				return cur->val;
			}
		}
		return cur->val;
	}

	Type _predecessor(Node *&cur, Type val) {
		if (cmp(val, cur->val) || val == cur->val) { 
			// 如果val与当前节点val相同或者小于当前节点val都需要在左子树继续查找前驱
			if (cur->leftSon) return _predecessor(cur->leftSon, val);
			else throw NoSuchValueException("No lesser value in the tree");
		} // 否则val大于当前节点val
		if (cur->rightSon) { 
			try { // 若有右子树，先在右子树中查找
				return _predecessor(cur->rightSon, val);
			} catch(const NoSuchValueException &e) {
				return cur->val;
			}
		}
		return cur->val; // 没有右子树返回当前节点
	}

	Type _queryKth(Node *&cur, int rank) {
		int leftSonSize = cur->leftSon ? cur->leftSon->size : 0;
		if (rank <= leftSonSize) {
			return _queryKth(cur->leftSon, priority);
		} else if (rank <= leftSonSize + cur->count) {
			return cur->val;
		} else if (rank <= cur->size) {
			return _queryKth(cur->rightSon, rank - leftSonSize - cur->count);
		}
	}

	int _queryRank(Node *&cur, Type val) {
		if (!cur) return 1; // 空树中任何值排名都为1
		int leftSonSize = cur->leftSon ? cur->leftSon->size : 0;
		if (val == cur->val) {
			return leftSonSize + 1;
		} else if (cmp(val, cur->val)) { // val < cur->val
			return _queryRank(cur->leftSon, val);
		} else {
			return leftSonSize + cur->count + _queryRank(cur->rightSon, val);
		} // 在右子树中查询排名，加上当前节点的个数和左子树大小
	}

	int _count(Node *cur, Type val) {
		if (cur == nullptr) return 0;
		if (cur->val == val) {
			return cur->count;
		} else if (val < cur->val) {
			return _count(cur->leftSon, val);
		} else {
			return _count(cur->rightSon, val);
		}
	}

	void print(std::vector<Type> &out, Node *cur) {
		if (cur == nullptr) return;
		print(out, cur->children[0]);
		for (int i = 1; i <= cur->count; i++) {
			out.push_back(cur->val);
		}
		print(out, cur->children[1]);
	}

	void _clear(Node *&cur)
	{
		if (cur == nullptr) return;
		_clear(cur->children[0]);
		_clear(cur->children[1]);
		delete cur;
		cur = nullptr;
	}

public:

	class NoSuchValueException : std::exception {
	private:
		const char *message;
	public:
		NoSuchValueException(const char *msg) : message(msg) {}
		const char *what() const noexcept override { 
			return message; 
		}
	};

	~Treap() {
		_clear(root);
	}

	int size() {
		return root ? root->size : 0;
	}

	void insert(Type x) {
		_insert(root, x);
	}

	bool erase(Type x) {
		return _erase(root, x);
	}
	
	int queryRank(Type x) {
		return _queryRank(root, x);
	}

	Type queryKth(int x) { 
		if (x > size()) throw NoSuchValueException("No so many vals in the treap");
		return _queryKth(root, x);
	}

	Type predecessor(Type x) {
		return _predecessor(root, x);
	}

	Type successor(Type x) {
		return _successor(root, x);
	}

	int count(Type x) {
		return _count(root, x);
	}

	void clear() {
		_clear(root);
	}

	operator std::vector<Type> () {
		std::vector<Type> temp;
		print(temp, root);
		return temp;
	}
}

#endif

#include <iostream>
using namespace std;
Treap<int> tr;
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