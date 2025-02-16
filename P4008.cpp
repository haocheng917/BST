#include <bits/stdc++.h>
using namespace std;
struct TextEditor {
    size_t cursor;
    struct Node {
        char data;
        Node* leftSon, * rightSon;
        int size, priority;
        Node(char data) : data(data), leftSon(nullptr), rightSon(nullptr), size(1), priority(rand()) {}
        void update() {
            size = (leftSon == nullptr ? 0 : leftSon->size) + (rightSon == nullptr ? 0 : rightSon->size) + 1;
        }
    };
    Node* root;
    TextEditor() : cursor(0), root(nullptr) {}
    std::pair<Node*, Node*>split(Node* rt, int k) {
        if (rt == nullptr) return { nullptr, nullptr };
        if (k == 0) return { nullptr, rt };
        int leftSonSize = rt->leftSon == nullptr ? 0 : rt->leftSon->size;
        if (k <= leftSonSize) {
            auto [left, right] = split(rt->leftSon, k);
            rt->leftSon = right;
            rt->update();
            return { left, rt };
        } else {
            auto [left, right] = split(rt->rightSon, k - leftSonSize - 1);
            rt->rightSon = left;
            rt->update();
            return { rt, right };
        }
    }
    Node* merge(Node* u, Node* v) {
        if (!u || !v) return u ? u : v;
        if (u->priority < v->priority) {
            u->rightSon = merge(u->rightSon, v);
            u->update();
            return u;
        } else {
            v->leftSon = merge(u, v->leftSon);
            v->update();
            return v;
        }
    }
    void insert(string s) {
        auto [left, right] = split(root, cursor);
        for (char c : s) {
            Node* newNode = new Node(c);
            left = merge(left, newNode);
        }
        root = merge(left, right);
    }
    void clear(Node* rt) {
        if (rt == nullptr) return;
        clear(rt->leftSon);
        clear(rt->rightSon);
        delete rt;
    }
    void del(int k) {
        auto [left, right] = split(root, cursor);
        auto [mid, right2] = split(right, k);
        clear(mid);
        root = merge(left, right2);
    }
    void move(int k) {
        cursor = k;
    }
    void prev() {
        cursor--;
    }
    void next() {
        cursor++;
    }
    string get(int k) {
        auto [left, right] = split(root, cursor);
        auto [mid, right2] = split(right, k);
        string res = "";
        function<void(Node*)> dfs = [&](Node* rt) {
            if (rt == nullptr) return;
            dfs(rt->leftSon);
            res += rt->data;
            dfs(rt->rightSon);
        };
        dfs(mid);
        root = merge(left, merge(mid, right2));
        return res;
    }
};
int n;
string s;
TextEditor editor;
int main() {
    cin >> n;
    while (n--) {
        string op;
        cin >> op;
        if (op == "Insert") {
            int t; cin >> t;
            string s;
            getchar();
            while (t) {
                char c = getchar();
                if (c != '\n' && c != '\r') s += c, t--;
            }
            editor.insert(s);
        } else if (op == "Move") {
            int k;
            cin >> k;
            editor.move(k);
        } else if (op == "Delete") {
            int k;
            cin >> k;
            editor.del(k);
        } else if (op == "Prev") {
            editor.prev();
        } else if (op == "Next") {
            editor.next();
        } else if (op == "Get") {
            int k;
            cin >> k;
            cout << editor.get(k) << endl;
        }
    }
    return 0;
}