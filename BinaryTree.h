#include <cstdlib>
#include <memory>
#include <iostream>
#include <stack>
#include <stdexcept>
#ifndef SMART_POINTERS_BINARYTREE_H
#define SMART_POINTERS_BINARYTREE_H
template <typename ValueType, typename KeyType>
class BinaryTree {
    class Node{
    public:
        Node(const KeyType& key, const ValueType& value,
             Node* parent = nullptr,
             Node* child_left = nullptr, Node* child_right = nullptr);
        ~Node()=default;
        KeyType getKey() const{
            return key;
        }
        Node* getRightChild() {
            return child_right;
        }
        Node* getLeftChild() {
            return child_left;
        }

        Node* getParent() {
            return parent;
        }

        ValueType& getValue() {
            return value;
        }

        friend class BinaryTree;
    protected:
        Node* insert(const KeyType& input_key, const ValueType& input_value);
        void setKey(const KeyType& new_key);
        void setParent(Node* new_parent);
        void setLeftChild(Node* new_child);
        void setRightChild(Node* new_child);
        void setValue(ValueType& val);
    private:
        KeyType key;
        ValueType value;
        Node* parent;
        Node* child_left;
        Node* child_right;
    };
public:
    BinaryTree();
    BinaryTree(const BinaryTree& copy);
    BinaryTree& operator=(const BinaryTree& copy);
    BinaryTree(BinaryTree&& moveCopy) noexcept;
    BinaryTree& operator=(BinaryTree&& moveCopy) noexcept;
    ~BinaryTree(){forceNodeDelete(_root);};
    void add(const KeyType& key, const ValueType& value);
    void remove(const KeyType& key);
    ValueType find(const KeyType& key)const;
    size_t getCapacity()const{return _cap;};
    bool isEmpty()const{return _cap == 0;};
protected:
//
    Node* deleteNode(Node* node);
    void forceNodeDelete(Node* root);
//
    Node* find(const KeyType& key, Node* root)const;
//
    Node* getLastLeft(Node* root);//Получить самый левый узел
private:
    Node* _root;
    size_t _cap;
};

template<typename ValueType, typename KeyType>
BinaryTree<ValueType, KeyType>::BinaryTree() {
    _root = nullptr;
    _cap = 0;
}

template<typename ValueType, typename KeyType>
BinaryTree<ValueType, KeyType>::BinaryTree(const BinaryTree &copy) {
    if(!copy.getCapacity()){
        return;
    }
    Node* current = copy._root;
    BinaryTree<KeyType, ValueType> tmp;
    std::stack<Node*> stack;
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
                current = nullptr;
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
    Node* current = copy._root;
    BinaryTree<KeyType, ValueType> tmp;
    std::stack<Node*> stack;
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
                current = nullptr;
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
        _root = new Node(key, value);
    }
    else{
        _root->insert(key, value);
    }
    _cap += 1;
}

template<typename ValueType, typename KeyType>
void BinaryTree<ValueType, KeyType>::remove(const KeyType &key) {
    Node* node = find(key, _root);
    if(node){
        Node* free = deleteNode(node);
        delete(free);
        _cap -= 1;
    }
    else{
        throw std::runtime_error("No object with such key\n");
    }
}

template<typename ValueType, typename KeyType>
typename BinaryTree<ValueType, KeyType>::Node
*BinaryTree<ValueType, KeyType>::deleteNode(BinaryTree::Node *node) {
    Node* result = nullptr;
    if(!node){
        return result;
    }
    Node* current_node = _root;

    if(current_node->getKey() > node->getKey()){
        result = deleteNode(node->getRightChild());
    }
    if(current_node->getKey() < node->getKey()){
        result = deleteNode(node->getLeftChild());
    }
    else if(current_node->getKey() == node->getKey()){
        if(!current_node->getRightChild() && !current_node->getLeftChild()){
            result = current_node;
            if(result->getParent()->getRightChild() == result){
                result->getParent()->setRightChild(nullptr);
            }
            else if(result->getParent()->getLeftChild() == result){
                    result->getParent()->setLeftChild(nullptr);
            }
            result->setParent(nullptr);
        }
        else if(!current_node->getLeftChild()){
            Node* right = current_node->getRightChild();
            current_node->setKey(right->getKey());
            current_node->setLeftChild(right->getLeftChild());
            current_node->setRightChild(right->getRightChild());
            right->setRightChild(nullptr);
            right->setLeftChild(nullptr);
            right->setParent(nullptr);
            if(current_node->getRightChild()){
                current_node->getRightChild()->setParent(current_node);
            }
            if(current_node->getLeftChild()){
                current_node->getLeftChild()->setParent(current_node);
            }
            result = right;
        }
        else if(!current_node->getRightChild()){
            Node* left = current_node->getLeftChild();
            current_node->setKey(left->getKey());
            current_node->setLeftChild(left->getLeftChild());
            current_node->setRightChild(left->getRightChild());
            left->setRightChild(nullptr);
            left->setLeftChild(nullptr);
            left->setParent(nullptr);
            if(current_node->getRightChild()){
                current_node->getRightChild()->setParent(current_node);
            }
            if(current_node->getLeftChild()){
                current_node->getLeftChild()->setParent(current_node);
            }
            result = left;
        }
        else{
            if(!current_node->getRightChild()->getLeftChild()){
                current_node->setKey(current_node->getRightChild()->getKey());
                current_node->setValue(current_node->getRightChild()->getValue());
                result = current_node->getRightChild();
                current_node->setRightChild(result->getRightChild());
                result->setRightChild(nullptr);
                current_node->getRightChild()->setParent(current_node);
            }
            else{
                result = getLastLeft(current_node->getRightChild());
                current_node->setValue(result->getValue());
                current_node->setKey(result->getKey());
                result = deleteNode(result);
            }
        }
    }
    return result;
}

template<typename ValueType, typename KeyType>
typename BinaryTree<ValueType, KeyType>::Node *BinaryTree<ValueType, KeyType>::getLastLeft(BinaryTree::Node *root) {
    Node* node = root;
    if(!node)
        return nullptr;
    while(node->getLeftChild()){
        node = node->getLeftChild();
    }
    return node;
}

template<typename ValueType, typename KeyType>
typename BinaryTree<ValueType, KeyType>::Node
*BinaryTree<ValueType, KeyType>::find(const KeyType &key,
                                      BinaryTree::Node *root)const {
    Node* node = root;
    while(node){
        if(key < node->getKey()){
            node = node->getLeftChild();
        }
        else if(key > node->getKey()){
            node = node->getRightChild();
        }
        else{
            return node;
        }
    }
    return node;
}

template<typename ValueType, typename KeyType>
ValueType BinaryTree<ValueType, KeyType>::find(const KeyType &key) const {
    Node* result_node = find(key, _root);
    return result_node? result_node->getValue(): NULL;
}

template<typename ValueType, typename KeyType>
void BinaryTree<ValueType, KeyType>::forceNodeDelete(BinaryTree::Node *root) {
    if(!_cap){
        return;
    }
    Node* node = root;
    Node* leaf = nullptr;
    while(node){
        if(node->getLeftChild()){
            node = node->getLeftChild();
        }
        else if(node->getRightChild()){
            node = node->getRightChild();
        }
        else{
            leaf = node;
            node = node->getParent()? node->getParent(): nullptr;
            if(node && node->getRightChild() && node->getRightChild() == leaf){
                node->setRightChild(nullptr);
            }
            if(node && node->getLeftChild() && node->getLeftChild() == leaf){
                node->setLeftChild(nullptr);
            }
            delete leaf;
        }
    }
}

template<typename ValueType, typename KeyType>
BinaryTree<ValueType, KeyType>::Node::Node(const KeyType &key, const ValueType &value, Node* parent,
                                           Node* child_left, Node* child_right) {
    this->key = key;
    this->value = value;
    this->child_left = child_left;
    this->child_right = child_right;
    this->parent = parent;
}

template<typename ValueType, typename KeyType>
void BinaryTree<ValueType, KeyType>::Node::setKey(const KeyType &new_key) {
    key = new_key;
}

template<typename ValueType, typename KeyType>
void BinaryTree<ValueType, KeyType>::Node::setParent(BinaryTree::Node *new_parent) {
    parent = new_parent;
}

template<typename ValueType, typename KeyType>
void BinaryTree<ValueType, KeyType>::Node::setLeftChild(BinaryTree::Node *new_child) {
    child_left = new_child;
}

template<typename ValueType, typename KeyType>
void BinaryTree<ValueType, KeyType>::Node::setRightChild(BinaryTree::Node *new_child) {
    child_right = new_child;
}

template<typename ValueType, typename KeyType>
void BinaryTree<ValueType, KeyType>::Node::setValue(ValueType &val) {
    value = val;
}

template<typename ValueType, typename KeyType>
typename BinaryTree<ValueType, KeyType>::Node *BinaryTree<ValueType, KeyType>::Node::insert(const KeyType &input_key, const ValueType &input_value) {
    Node* current_node = this;
    if(current_node->key > input_key){
        if(!current_node->child_left){
            current_node->child_left =
                    new Node(input_key, input_value, current_node);
            return current_node->child_left;
        }
        else{
            return current_node->child_left->insert(input_key, input_value);
        }
    }
    else{
        if(!current_node->child_right){
            current_node->child_right =
                    new Node(input_key, input_value, current_node);
            return current_node->child_right;
        }
        else{
            return current_node->child_right->insert(input_key, input_value);
        }
    }
}


#endif //SMART_POINTERS_BINARYTREE_H
