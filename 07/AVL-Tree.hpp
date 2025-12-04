// Как по мне, это задание с огромной такой звездочкой, C++ для самых продвинутых, не иначе

#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include <memory>
#include <utility>
#include <functional>
#include <stdexcept>
#include <iterator>
#include <cstddef>

namespace avl_ns {
template<
    class Key,
    class T,
    class Compare = std::less<Key>,
    class Allocator = std::allocator<std::pair<const Key, T>>>
class avl
{
public:
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const Key, T>;
    using key_compare = Compare;
    using allocator_type = Allocator;
    using size_type = std::size_t;

private:
    // Структура узла AVL-дерева
    struct Node {
        value_type value;      // Ключ и значение
        Node* left = nullptr;  
        Node* right = nullptr; 
        Node* parent = nullptr; 
        int height = 1;        // Высота поддерева для балансировки

        template<typename... Args>
        Node(Args&&... args) : value(std::forward<Args>(args)...) {}
    };

    // Аллокаторы: пользовательский для pair -> наш для Node
    using AllocTraits = std::allocator_traits<allocator_type>;
    using NodeAlloc = typename AllocTraits::template rebind_alloc<Node>;
    using NodeAllocTraits = std::allocator_traits<NodeAlloc>;

    Node* root_ = nullptr;     // Корень дерева
    size_type size_ = 0;       // Количество элементов
    key_compare comp_;         // Компаратор для сравнения ключей
    allocator_type allocator_; // Аллокатор пользователя
    NodeAlloc node_alloc_;     // Аллокатор для узлов

public:
    // Двунаправленный итератор
    template<bool IsConst>
    class bst_iterator {
        using node_ptr = typename std::conditional<IsConst, const Node*, Node*>::type;
        node_ptr node_ = nullptr;
        node_ptr root_ = nullptr;  // Нужен для --end() 
        friend class avl; 

    public:
        // Типы, требуемые стандартом для итераторов
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = std::pair<const Key, T>;
        using difference_type = std::ptrdiff_t;
        using pointer = typename std::conditional<IsConst, const value_type*, value_type*>::type;
        using reference = typename std::conditional<IsConst, const value_type&, value_type&>::type;

        bst_iterator() = default;
        bst_iterator(node_ptr n, node_ptr root) : node_(n), root_(root) {}

        reference operator*() const { return node_->value; }
        pointer operator->() const { return std::addressof(node_->value); }

        bool operator==(const bst_iterator& o) const { return node_ == o.node_; }
        bool operator!=(const bst_iterator& o) const { return node_ != o.node_; }

        // Переход к следующему элементу
        bst_iterator& operator++() {
            if (!node_) 
                return *this; 
            if (node_->right) {
                node_ = node_->right;
                while (node_->left) node_ = node_->left;
            } 
            else {
                auto p = node_->parent;
                while (p && node_ == p->right) { node_ = p; p = p->parent; }
                node_ = p;
            }
            return *this;
        }

        // Переход к предыдущему элементу
        bst_iterator& operator--() {
            if (!node_) {
                if (root_) {
                    node_ = root_;
                    while (node_ && node_->right) node_ = node_->right;
                }
                return *this;
            }
            if (node_->left) {
                node_ = node_->left;
                while (node_->right) node_ = node_->right;
            } 
            else {
                auto p = node_->parent;
                while (p && node_ == p->left) { node_ = p; p = p->parent; }
                node_ = p;
            }
            return *this;
        }

        bst_iterator operator++(int) { auto tmp = *this; ++(*this); return tmp; }
        bst_iterator operator--(int) { auto tmp = *this; --(*this); return tmp; }
    };

    // Типы итераторов
    using iterator = bst_iterator<false>;
    using const_iterator = bst_iterator<true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    avl(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type()) : comp_(comp), allocator_(alloc), node_alloc_(NodeAlloc(alloc)) {}

    ~avl() { clear(); } 

    avl(const avl& other) = delete;
    avl& operator=(const avl& other) = delete;

    // Основные методы
    bool empty() const noexcept { return size_ == 0; }
    size_type size() const noexcept { return size_; }

    void clear() noexcept { clear_impl(root_); root_ = nullptr; size_ = 0; }

    mapped_type& at(const key_type& key) {
        Node* n = find_node(root_, key);
        if (!n) throw std::out_of_range("key not found");
        return const_cast<mapped_type&>(n->value.second);
    }

    const mapped_type& at(const key_type& key) const {
        Node* n = find_node(root_, key);
        if (!n) throw std::out_of_range("key not found");
        return n->value.second;
    }

    mapped_type& operator[](const key_type& key) {
        auto it = find(key);
        if (it != end()) return it->second;
        auto p = insert({key, mapped_type()});
        return p.first->second;
    }

    std::pair<iterator, bool> insert(const value_type& val) {
        bool inserted = false;
        root_ = insert_impl(root_, nullptr, val, inserted);
        if (inserted) ++size_;
        Node* n = find_node(root_, val.first);
        return {iterator(n, root_), inserted};
    }

    size_type erase(const key_type& key) {
        bool erased = false;
        root_ = erase_impl(root_, key, erased);
        if (erased) { --size_; }
        return erased ? 1 : 0;
    }

    iterator find(const key_type& key) {
        Node* n = find_node(root_, key);
        return n ? iterator(n, root_) : end();
    }
    const_iterator find(const key_type& key) const {
        Node* n = find_node(root_, key);
        return n ? const_iterator(n, root_) : cend();
    }

    bool contains(const key_type& key) const { return find_node(root_, key) != nullptr; }

    // Итераторы начала и конца
    iterator begin() { return iterator(min_node(root_), root_); }
    const_iterator begin() const { return const_iterator(min_node(root_), root_); }
    const_iterator cbegin() const { return const_iterator(min_node(root_), root_); }

    iterator end() { return iterator(nullptr, root_); }
    const_iterator end() const { return const_iterator(nullptr, root_); }
    const_iterator cend() const { return const_iterator(nullptr, root_); }

    // Обратные итераторы (через адаптер std::reverse_iterator)
    reverse_iterator rbegin() { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(cend()); }

    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(cbegin()); }

private:
    // Вспомогательные функции для AVL балансировки
    static int height(Node* n) { return n ? n->height : 0; }
    static int balance_factor(Node* n) { return n ? height(n->left) - height(n->right) : 0; }
    static void update_height(Node* n) { n->height = 1 + std::max(height(n->left), height(n->right)); }

    Node* rotate_right(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        x->parent = y->parent;
        y->parent = x;
        if (T2) T2->parent = y;

        update_height(y);
        update_height(x);
        return x;
    }

    Node* rotate_left(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        y->parent = x->parent;
        x->parent = y;
        if (T2) T2->parent = x;

        update_height(x);
        update_height(y);
        return y;
    }

    // Балансировка узла (4 случая: LL, RR, LR, RL)
    Node* balance(Node* node) {
        update_height(node);
        int bf = balance_factor(node);
        if (bf > 1) { 
            if (balance_factor(node->left) < 0) { 
                node->left = rotate_left(node->left);
                if (node->left) node->left->parent = node;
            }
            return rotate_right(node);
        }
        if (bf < -1) { 
            if (balance_factor(node->right) > 0) { 
                node->right = rotate_right(node->right);
                if (node->right) node->right->parent = node;
            }
            return rotate_left(node);
        }
        return node; 
    }

    Node* min_node(Node* n) const {
        if (!n) return nullptr;
        while (n->left) n = n->left;
        return n;
    }

    Node* max_node(Node* n) const {
        if (!n) return nullptr;
        while (n->right) n = n->right;
        return n;
    }

    Node* find_node(Node* n, const key_type& key) const {
        while (n) {
            if (comp_(key, n->value.first)) n = n->left;
            else if (comp_(n->value.first, key)) n = n->right;
            else return n;
        }
        return nullptr;
    }

    // Создание узла с использованием аллокатора
    template<typename... Args>
    Node* create_node(Args&&... args) {
        Node* p = NodeAllocTraits::allocate(node_alloc_, 1);
        try {
            NodeAllocTraits::construct(node_alloc_, p, std::forward<Args>(args)...);
        } catch(...) {
            NodeAllocTraits::deallocate(node_alloc_, p, 1); 
            throw;
        }
        return p;
    }

    // Удаление узла
    void destroy_node(Node* p) noexcept {
        NodeAllocTraits::destroy(node_alloc_, p);
        NodeAllocTraits::deallocate(node_alloc_, p, 1);
    }

    // Рекурсивная очистка дерева
    void clear_impl(Node* n) noexcept {
        if (!n) return;
        clear_impl(n->left);
        clear_impl(n->right);
        destroy_node(n);
    }

    // Рекурсивная вставка с балансировкой
    Node* insert_impl(Node* node, Node* parent, const value_type& val, bool& inserted) {
        if (!node) {
            Node* n = create_node(val);
            n->parent = parent;
            inserted = true;
            return n;
        }
        if (comp_(val.first, node->value.first)) {
            node->left = insert_impl(node->left, node, val, inserted);
        } 
        else if (comp_(node->value.first, val.first)) {
            node->right = insert_impl(node->right, node, val, inserted);
        } 
        else {
            inserted = false;
            return node;
        }

        if (node->left) node->left->parent = node;
        if (node->right) node->right->parent = node;

        Node* res = balance(node);
        if (res != node) {
            if (res->parent) {
                if (res->parent->left == node) res->parent->left = res;
                else if (res->parent->right == node) res->parent->right = res;
            }
        }
        return res;
    }

    // Рекурсивное удаление с балансировкой
    Node* erase_impl(Node* node, const key_type& key, bool& erased) {
        if (!node) return nullptr;
        if (comp_(key, node->value.first)) {
            node->left = erase_impl(node->left, key, erased);
            if (node->left) node->left->parent = node;
        } 
        else if (comp_(node->value.first, key)) {
            node->right = erase_impl(node->right, key, erased);
            if (node->right) node->right->parent = node;
        } 
        else {
            erased = true;
            if (!node->left || !node->right) {
                Node* tmp = node->left ? node->left : node->right;
                if (!tmp) {
                    destroy_node(node);
                    return nullptr;
                } 
                else {
                    tmp->parent = node->parent;
                    destroy_node(node);
                    return tmp;
                }
            } 
            else {
                Node* succ = min_node(node->right);
                node->value.second = succ->value.second;
                node->right = erase_impl(node->right, succ->value.first, erased);
                if (node->right) node->right->parent = node;
                erased = true; 
            }
        }

        Node* res = node;
        res = balance(res);
        return res;
    }
};

} // Это все в пространстве avl_ns

#endif 


// Помогите :(