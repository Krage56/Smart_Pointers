#include <cstdlib>
#include <memory>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <cstddef>
#include <tuple>
#ifndef SMART_POINTERS_BINARYTREE_H
#define SMART_POINTERS_BINARYTREE_H
template <typename ValueType, typename KeyType>
class BinaryTree {
    class Node{
    public:
        Node(const KeyType& key, const ValueType& value);
        ~Node(){std::cout << key << " - node destructed\n";};
        KeyType getKey() const{
            return key;
        }
        std::shared_ptr<Node> getRightChild() {
            return child_right;
        }
        std::shared_ptr<Node> getLeftChild() {
            return child_left;
        }

        std::weak_ptr<Node> getParent() {
            return parent;
        }

        ValueType& getValue() {
            return value;
        }

        friend class BinaryTree;
    protected:
        void setKey(const KeyType& new_key);
        void setParent(const std::shared_ptr<Node>& new_parent);
        void setLeftChild(const std::shared_ptr<Node> &new_child);
        void setRightChild(const std::shared_ptr<Node> &new_child);
        void setValue(ValueType& val);
    private:
        KeyType key;
        ValueType value;
        std::weak_ptr<Node> parent;
        std::shared_ptr<Node> child_left;
        std::shared_ptr<Node> child_right;
    };
public:
    //Конструктор по умолчанию
    BinaryTree();
    //Конструктор копирования
    BinaryTree(const BinaryTree& copy);
    BinaryTree& operator=(const BinaryTree& copy);
    //Конструктор копирования перемещением
    BinaryTree(BinaryTree&& moveCopy) noexcept;
    BinaryTree& operator=(BinaryTree&& moveCopy) noexcept;
    /*Деструктор работает в режиме default,
     т.к. ноды хранятся в умных указателях и сами удалятся,
     когда время жизни дерева закончится*/
    ~BinaryTree()=default;
    //Добавить пару ключ-значение в дерево
    void add(const KeyType& key, const ValueType& value);
    //Удалить пару ключ-значение по ключу
    void remove(const KeyType& key);
    /*Вернуть значение по ключу.
     * Возвращаемый тип - кортеж <bool, ValueType>,
     * если первое значение истина - второе содержит
     * валидное значение, иначе там мусор*/
    auto find(const KeyType& key)const;
    //Размер дерева
    size_t getCapacity()const{return _cap;};
    //Является ли дерево пустым
    bool isEmpty()const{return _cap == 0;};
protected:
    //Производит процедуру удаления из дерева заданной пары значений
    void deleteNode(std::shared_ptr<Node> &node);
    //Возвращает указатель на контейнер с соответствующим ключом
    std::weak_ptr<Node> find(const KeyType& key,
                             const std::shared_ptr<BinaryTree::Node>& root)const;
    //Получить самый левый узел
    std::weak_ptr<Node> getLastLeft(const std::shared_ptr<Node>& root);
private:
    std::shared_ptr<Node> _root;
    size_t _cap;
};

template<typename ValueType, typename KeyType>
BinaryTree<ValueType, KeyType>::BinaryTree() {
    _cap = 0;
}

template<typename ValueType, typename KeyType>
BinaryTree<ValueType, KeyType>::BinaryTree(const BinaryTree &copy) {
    if(!copy.getCapacity()){
        return;
    }
    std::shared_ptr<Node*> current = copy._root;
    BinaryTree<KeyType, ValueType> tmp;
    std::stack<std::shared_ptr<Node*>> stack;
    do{
        if(current->getRightChild() && current->getLeftChild()){
            stack.push(current);
        }
        if(current->getLeftChild()){
            tmp.add(current->getKey(), current->getValue());
            current = current->getLeftChild();
        }
        else if(current->getRightChild()){
            tmp.add(current->getKey(), current->getValue());
            current = current->getRightChild();
        }
        else if(!current->getRightChild() && !current->getLeftChild()){
            tmp.add(current->getKey(), current->getValue());
            if(!stack.empty()){
                current = stack.top()->getRightChild();
                stack.pop();
            }
            else{
                current.reset();
            }
        }
    }while(current != copy._root && current);
    _root = tmp._root;
    _cap = tmp._cap;
    tmp._cap = 0;
}

template<typename ValueType, typename KeyType>
BinaryTree<ValueType, KeyType> &BinaryTree<ValueType, KeyType>::operator=(const BinaryTree &copy) {
    if(this == &copy){
        return *this;
    }
    forceNodeDelete(_root);
    if(!copy.getCapacity()){
        return *this;
    }
    std::shared_ptr<Node*> current = copy._root;
    BinaryTree<KeyType, ValueType> tmp;
    std::stack<std::shared_ptr<Node*>> stack;
    do{
        if(current->getRightChild() && current->getLeftChild()){
            stack.push(current);
        }
        if(current->getLeftChild()){
            tmp.add(current->getKey(), current->getValue());
            current = current->getLeftChild();
        }
        else if(current->getRightChild()){
            tmp.add(current->getKey(), current->getValue());
            current = current->getRightChild();
        }
        else if(!current->getRightChild() && !current->getLeftChild()){
            tmp.add(current->getKey(), current->getValue());
            if(!stack.empty()){
                current = stack.top()->getRightChild();
                stack.pop();
            }
            else{
                current.reset();
            }
        }
    }while(current != copy._root && current);
    _root = tmp._root;
    _cap = tmp._cap;
    tmp._cap = 0;
    return *this;
}

template<typename ValueType, typename KeyType>
BinaryTree<ValueType, KeyType>::BinaryTree(BinaryTree &&moveCopy) noexcept {
    if (this == &moveCopy) {
        return;
    }
    _root = moveCopy._root;
    _cap = moveCopy._cap;
    moveCopy._root = nullptr;
    moveCopy._cap = 0;
}

template<typename ValueType, typename KeyType>
BinaryTree<ValueType, KeyType> &BinaryTree<ValueType, KeyType>::operator=(BinaryTree &&moveCopy) noexcept {
    if (this == &moveCopy) {
        return *this;
    }
    _root = moveCopy._root;
    _cap = moveCopy._cap;
    moveCopy._root = nullptr;
    moveCopy._cap = 0;
    return *this;
}

template<typename ValueType, typename KeyType>
void BinaryTree<ValueType, KeyType>::add(const KeyType &key, const ValueType &value) {
    if(_cap == 0){
        _root.reset(new Node(key, value));
    }
    else{
        std::shared_ptr<Node> current_node = _root;
        bool success = false;
        while(!success){
            if(current_node->getKey() > key){
                if(!current_node->getLeftChild()){
                    std::shared_ptr<Node> new_child(new Node(key, value));
                    current_node->setLeftChild(new_child);
                    current_node.get()->getLeftChild()->setParent(current_node);
                    success = true;
                }
                else{
                    current_node = current_node.get()->getLeftChild();
                }
            }
            else{
                if(current_node->getKey() < key){
                    if(!current_node->child_right){
                        std::shared_ptr<Node> new_child(new Node(key, value));
                        current_node->setRightChild(new_child);
                        current_node.get()->getRightChild()->setParent(current_node);
                        success = true;
                    }
                    else{
                        current_node = current_node.get()->getRightChild();
                    }
                }
            }
        }
    }
    _cap += 1;
}

template<typename ValueType, typename KeyType>
std::weak_ptr<typename BinaryTree<ValueType, KeyType>::Node>
BinaryTree<ValueType, KeyType>::getLastLeft(const std::shared_ptr<Node>& root) {
    std::shared_ptr<Node> node = root;
    if(!node)
        return std::weak_ptr<Node>();
    while(node->getLeftChild()){
        node = node->getLeftChild();
    }
    return std::weak_ptr<Node>(node);
}


template<typename ValueType, typename KeyType>
auto BinaryTree<ValueType, KeyType>::find(const KeyType &key) const {
    std::weak_ptr<Node> result_node = find(key, _root);
    if(!result_node.expired()){
        std::shared_ptr<Node> tmp = result_node.lock();
        return std::make_tuple(true, tmp->getValue());
    }
    return std::make_tuple(false, -1);
}

template<typename ValueType, typename KeyType>
std::weak_ptr<typename BinaryTree<ValueType, KeyType>::Node>
BinaryTree<ValueType, KeyType>::find(const KeyType &key,
                                     const std::shared_ptr<BinaryTree::Node>& root) const {
    std::shared_ptr<Node> node = root;
    while(node){
        if(key < node->getKey()){
            node = node->getLeftChild();
        }
        else if(key > node->getKey()){
            node = node->getRightChild();
        }
        else{
            return std::weak_ptr<Node>(node);
        }
    }
    return std::weak_ptr<Node>(node);
}

template<typename ValueType, typename KeyType>
void BinaryTree<ValueType, KeyType>::remove(const KeyType &key) {
    std::weak_ptr<Node> result_ptr = find(key, _root);
    if(result_ptr.expired())
        return;
    std::shared_ptr<Node> tmp(result_ptr.lock());
    deleteNode(tmp);
}

template<typename ValueType, typename KeyType>
void BinaryTree<ValueType, KeyType>::deleteNode(std::shared_ptr<Node> &node) {
    if(!node->getRightChild() && !node->getLeftChild()){
        std::shared_ptr<Node> parent = node->getParent().lock();
        if((parent->getRightChild().get()) == (node.get())){
            parent->setRightChild(nullptr);
        }
        else if((parent->getLeftChild().get()) == (node.get())){
            parent->setLeftChild(nullptr);
        }
    }
    else if(!node->getLeftChild()){
        std::shared_ptr<Node> right = node->getRightChild();
        node->setKey(right->getKey());
        node->setValue(right->getValue());
        node->setLeftChild(right->getLeftChild());
        node->setRightChild(right->getRightChild());
        if(node->getRightChild())
            node->getRightChild()->setParent(node);
        if(node->getLeftChild())
            node->getLeftChild()->setParent(node);
    }
    else if(!node->getRightChild()){
        std::shared_ptr<Node> left = node->getLeftChild();
        node->setKey(left->getKey());
        node->setValue(left->getValue());
        node->setLeftChild(left->getLeftChild());
        node->setRightChild(left->getRightChild());
        node->getRightChild()->setParent(node);
        node->getLeftChild()->setParent(node);
        if(node->getRightChild())
            node->getRightChild()->setParent(node);
        if(node->getLeftChild())
            node->getLeftChild()->setParent(node);
    }
    else{
        if(!node->getRightChild()->getLeftChild()){
            node->setKey(node->getRightChild()->getKey());
            node->setValue(node->getRightChild()->getValue());
            node->setRightChild(node->getRightChild()->getRightChild());
            node->getRightChild()->setParent(node);
        }
        else{
            std::weak_ptr<Node> last_left = getLastLeft(node->getRightChild());
            std::shared_ptr<Node> new_aim = last_left.lock();
            node->setValue(new_aim->getValue());
            node->setKey(new_aim->getKey());
            deleteNode(new_aim);
        }
    }
}


template<typename ValueType, typename KeyType>
BinaryTree<ValueType, KeyType>::Node::Node(const KeyType &key, const ValueType &value) {
    this->key = key;
    this->value = value;
}

template<typename ValueType, typename KeyType>
void BinaryTree<ValueType, KeyType>::Node::setKey(const KeyType &new_key) {
    key = new_key;
}

template<typename ValueType, typename KeyType>
void BinaryTree<ValueType, KeyType>::Node::setParent(const std::shared_ptr<Node>& new_parent) {
    parent = new_parent;
}

template<typename ValueType, typename KeyType>
void BinaryTree<ValueType, KeyType>::Node::setLeftChild(const std::shared_ptr<Node> &new_child) {
    child_left = new_child;
}

template<typename ValueType, typename KeyType>
void BinaryTree<ValueType, KeyType>::Node::setRightChild(const std::shared_ptr<Node> &new_child) {
    child_right = new_child;
}

template<typename ValueType, typename KeyType>
void BinaryTree<ValueType, KeyType>::Node::setValue(ValueType &val) {
    value = val;
}


#endif //SMART_POINTERS_BINARYTREE_H
