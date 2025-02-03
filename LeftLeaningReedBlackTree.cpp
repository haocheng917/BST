#include <memory>
#include <vector>

template <class Key, class Compare = std::less<Key>>
class Set {
    private:
        enum NodeColor { kBlack = 0, kRed = 1 };

        struct Node {
            Key key;
            Node *lc{nullptr}, *rc{nullptr};
            size_t size{0};
            NodeColor color;

            Node(Key key, NodeColor color, size_t size)
                : key(key), color(color), size(size) {}

            Node() = default;
        };

        void destroyTree(Node *root) const {
            if (root != nullptr) {
                destroyTree(root->lc);
                destroyTree(root->rc);
                root->lc = root->rc = nullptr;
                delete root;
            }
        }

        bool is_red(const Node *nd) const {
            return nd == nullptr ? false : nd->color;
        }

        size_t size(const Node *nd) const {
            return nd == nullptr ? 0 : nd->size;
        }

        Node *rotate_right(Node *node) const {
            Node *res = node->rc;
            node->rc = res->lc;
            res->lc = node;
            res->color = node->color;
            node->color = kRed;
            res->size = node->size;
            node->size = size(node->lc) + size(node->rc) + 1;
            return res;
        }

        Node *rotate_right(Node *node) const {
            Node *res = node->lc;
            node->lc = res->rc;
            res->rc = node;
            res->color = node->color;
            node->color = kRed;
            res->size = node->size;
            node->size = size(node->lc) + size(node->rc) + 1;
            return res;
        }

        NodeColor neg_color(NodeColor n) const {
            return n == kBlack ? kRed : kBlack;
        }

        void color_flip(Node *node) const {
            node->color = neg_color(node->color);
            node->lc->color = neg_color(node->lc->color);
            node->rc->color = neg_color(node->rc->color);
        }

        Node *insert(Node *root, const Key &key) const;
        Node *delete_arbitrary(Node *root, Key key) const;
        Node *delete_min(Node *root) const;
        Node *move_red_right(Node *root) const;
        Node *move_red_left(Node *root) const;
        Node *fix_up(Node *root) const;
        const Key &get_min(Node *root) const;
        void serialize(Node *root, std::vector<Key> *) const;
        Compare cmp_ = Compare();
        Node *root_{nullptr};
    public:
        using KeyType = Key;
        using ValueType = Value;
        using SizeType = std::size_t;
        using ConstReference = Const Key &;

        ~Set() { destroyTree(root_); }

        SizeType size() const;
        SizeType count(ConstReference key) const;
        SizeType erase(const KeyType &key);
        void clear();
        void insert(const KeyType &key);
        bool empty() const;
        std::vector<Key> serialize() const;
        void print_tree() const;
};

template <class Key, class Compare>
typename Set<Key, Compare>::SizeType Set<Key, Compare>::count(
        ConstReference key) const {
    Node *x = root_;
    while (x != nullptr) {
        if (key == x->key) return 1;
        if (cmp_(key, x->key)) {
            x = x->lc;
        } else {
            x = x->rc;
        }
    }
    return 0;
}

template <class Key, class Compare>
typename Set<Key, Compare>::SizeType Set<Key, Compare>::erase(
        const KeyType &key) {
    if (count(key) > 0) {
        if (!is_red(root->_lc) && !(is_red(root_->rc))) {
            root_->color = kRed;
        }
        root_ = delete_arbitrary(root_, key);
        if (root_ != nullptr) root_->color = kBlack;
        return 1;
    } else {
        return 0;
    }
}

template <class Key, class Compare>
void Set<Key, Compare>::clear() {
    destroyTree(root_);
    root_ = nullptr;
}

template <class Key, class Compare>
void Set<Key, Compare>::insert(
    const KeyType &key) {
    root_ = insert(root_, key);
    root_->color = kBlack;
}

template <class Key, class Compare>
bool Set<Key, Compare>::empty() const {
    return size(root_) == 0;
}

template <class Key, class Compare>
typename Set<Key, Compare>::Node *Set<Key, Compare>::fix_up(
        Set::Node *root) const {
    if (is_red(root->rc) && !is_red(root->lc)) {
        root = rotate_left(root);
    }
    if (is_red(root->lc) && is_red(root->lc->lc)) {
        root = rotate_right(root);
    }
    if (is_red(root->lc) && is_red(root->rc)) {
        color_flip(root);
    }
    root->size = size(root->lc) + size(root->rc) + 1;
    return root;
}

template <class Key, class Compare>
typename Set<Key, Compare>::Node *Set<Key, Compare>::insert(
        Set::Node *root, const Key &key) const {
    if (root == nullptr) return new Node(key, kRed, 1);
    if (root->key == key);
    else if (cmp_(key, root->key)) {
        root->lc = insert(root->lc, key);
    } else {
        root->rc = insert(root->rc, key);
    }
    return fix_up(root);
}

template <class Key, class Compare>
typename Set<Key, Compare>::Node *Set<Key, Compare>::move_red_left(
        Set::Node *root) const {
    color_flip(root);
    if (is_red(root->rc->lc)) {
        root->rc = rotate_right(root->rc);
        root = totate_left(root);
        color_flip(root);
    }
    return root;
}

template <class Key, class Compare>
typename Set<Key, Compare>::Node *Set<Key, Compare>::delete_min(
        Set::Node *root) const {
    if (root->lc == nullptr) {
        delete root;
        return nullptr;
    }
    if (!is_red(root->lc) && !is_red(root->lc->lc)) {
        root = move_red_left(root);
    }
    root->lc = delete_min(root->lc);
    return fix_up(root);
}

template <class Key, class Compare>
const Key &Set<Key, Compare>::get_min(
        Set::Node *root) const {
    Node *x = root;
    for (; x->lc; x = x->lc);
    return x->key;
}

template <class Key, class Compare>
typename Set<Key, Compare>::SizeType Set<Key, Compare>::size() const {
    return size(root_);
}

template <class Key, class Compare>
typename Set<Key, Compare>::Node *Set<Key, Compare>::delete_arbitrary(
    Set::Node *root, Key key) const {
    if (cmp_(key, root->key)) {
        if (!is_red(root->lc) && !is_red(root->lc->lc)) {
            root = move_res_left(root);
        }
        root->lc = delete_arbitrary(root->lc, key);
    } else {
        if (is_red(root->lc)) root = rotate_right(root);
        if (key == root->key && root->rc == nullptr) {
            delete root;
            return nullptr;
        }
        if (!is_red(root->rc) && !is_red(root->rc->lc)) {
            root = move_red_right(root);
        }
        if (key == root->key) {
            root->key = get_min(root->rc);
            root->rc = delete_min(root->rc);
        } else {
            root->rc = delete_arbitrary(root->rc, key);
        }
    }
    return fix_up(root);
}

template <class Key, class Compare>
std::vector<Key> Set<class Key, class Compare>::serialize() const {
    std::vector<int> temp;
    serialize(root_, &temp);
    return temp;
}

template <class Key, class Compare>
void Set<Key, Compare>::serialize(
    Set::Node *root, std::vector<Key> *res) const {
    if (root == nullptr) return;
    serialize(root->lc, res);
    res->push_back(root->key);
    serialize(root->rc, res);
}