#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
        Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    AVLNode<Key,Value>* insert_fix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n);
    AVLNode<Key,Value>* remove_fix(AVLNode<Key,Value>* n, int diff);
    AVLNode<Key,Value>* internalFind(const Key& key) const;
    AVLNode<Key,Value>* r_rotate(AVLNode<Key,Value>* n);
    AVLNode<Key,Value>* l_rotate(AVLNode<Key,Value>* n);
		AVLNode<Key,Value>* predecessor(AVLNode<Key, Value>* current);
};

/*
 * Recall: If key is already in the tree, you should
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    AVLNode<Key,Value>* n = new AVLNode<Key,Value>(new_item.first,new_item.second,nullptr);
    n->setLeft(nullptr);
    n->setRight(nullptr);
    // if tree is empty, set root_ as the given keyValuePair
    if (this->root_ == nullptr) this->root_->setValue(new_item.second);
    // update key if node with key is already on
    if (internalFind(new_item.first) != nullptr) {
        AVLNode<Key,Value>* n = internalFind(new_item.first);
        n->setValue(new_item.second);
    }
    // did not find the key
    // find the proper spot in the tree
    AVLNode<Key,Value>* curr;
    curr->setLeft(this->root_->getLeft());
    curr->setRight(this->root_->getRight());
    while (!(curr->getLeft() != nullptr && curr->getRight() != nullptr)) {
        // n's key is greater than the current node's key
        if (n->getKey() > curr->getKey()) {
            // found the spot
            if (n->getRight() == nullptr) {
                n->setParent(curr);
                curr->setRight(n);
                n->setBalance(0);
                if (curr->getBalance() == -1) curr->setBalance(0);
                else if (curr->getBalance() == 1) curr->setBalance(0);
                else if (curr->getBalance() == 0) {
                    curr->setBalance(0);
                    insert_fix(curr,n);
                }
            }
                // continue to iterate if not empty
            else curr = curr->getRight();
        }
            // n's key is less than the current node's key
        else {
            // found the spot
            if (curr->getLeft() == nullptr) {
                n->setParent(curr);
                curr->setLeft(n);
                n->setBalance(0);
                if (curr->getBalance() == -1) curr->setBalance(0);
                else if (curr->getBalance() == 1) curr->setBalance(0);
                else if (curr->getBalance() == 0) {
                    curr->setBalance(0);
                    insert_fix(curr,n);
                }
            }
                // continue to iterate if not empty
            else curr = curr->getLeft();
        }
    }
}

template<class Key, class Value>
AVLNode<Key,Value>* AVLTree<Key,Value>::insert_fix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n) {
    if (p == nullptr) return nullptr;
    AVLNode<Key,Value>* g = p->getParent();

    // p is left child of g
    if (g->getLeft() == p) {
        g->setBalance(g->getBalance()-1);
        if (g->getBalance() == 0) return nullptr;
        else if (g->getBalance() == -1) insert_fix(g,p);
        else if (g->getBalance() == -2) {
            // zig zig
            if (p->getLeft() == n) {
                r_rotate(g);
                p->setBalance(0);
                g->setBalance(0);
            }
                // zig zag
            else {
                l_rotate(p);
                r_rotate(g);
                if (n->getBalance() == -1) {
                    n->setBalance(0);
                    p->setBalance(0);
                    g->setBalance(1);
                }
                else if (n->getBalance() == 0) {
                    n->setBalance(0);
                    p->setBalance(0);
                    g->setBalance(0);
                }
                else if (n->getBalance() == 1) {
                    n->setBalance(0);
                    p->setBalance(-1);
                    g->setBalance(0);
                }
            }
        }
    }
        // p is right child of g
    else if (g->getRight() == p) {
        g->setBalance(g->getBalance()+1);
        if (g->getBalance() == 0) return nullptr;
        else if (g->getBalance() == 1) insert_fix(g,p);
        else if (g->getBalance() == 2) {
            // zig zig
            if (p->getRight() == n) {
                l_rotate(g);
                p->setBalance(0);
                g->setBalance(0);
            }
                // zig zag
            else {
                r_rotate(p);
                l_rotate(g);
                if (n->getBalance() == -1) {
                    n->setBalance(0);
                    p->setBalance(1);
                    g->setBalance(0);
                }
                else if (n->getBalance() == 0) {
                    n->setBalance(0);
                    p->setBalance(0);
                    g->setBalance(0);
                }
                else if (n->getBalance() == 1) {
                    n->setBalance(0);
                    p->setBalance(0);
                    g->setBalance(-1);
                }
            }
        }
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key) {
    // TODO
    AVLNode<Key,Value>* curr = internalFind(key);
    AVLNode<Key,Value>* pcurr = curr->getParent(); // parent of curr
    if (curr == nullptr) return;

    // both children
    if ((curr->getLeft() != nullptr) && (curr->getRight() != nullptr)) {
        AVLNode<Key,Value>* pred = predecessor(curr);
        nodeSwap(pred,curr);
        if (pcurr != nullptr) {
            if (pcurr->getLeft() == curr) remove_fix(pcurr,1);
            else remove_fix(pcurr,-1);
        }
    }
    // no children
    if ((curr->getLeft() == nullptr) && (curr->getRight() == nullptr)) {
        if (curr->getKey() == this->root_->getKey()) {
            this->root_ = nullptr;
        }
        else {
            if (pcurr->getLeft() == curr) pcurr->setLeft(nullptr);
            else if (pcurr->getRight() == curr) pcurr->setRight(nullptr);
        }
        delete curr;
    }
        // right child != nullptr
    else if ((curr->getLeft() == nullptr) && (curr->getRight() != nullptr)) {
        AVLNode<Key,Value>* temp = curr->getRight();
        if (curr->getKey() == this->root_->getKey()) {
            this->root_ = temp;
            this->root_->setParent(nullptr);
            delete curr;
        }
        else if (curr->getKey() != this->root_->getKey()) {
            if (pcurr->getLeft() == curr) pcurr->setLeft(temp);
            else pcurr->setRight(temp);
            delete curr;
        }
    }
        // left child
    else if ((curr->getRight() == nullptr) && (curr->getLeft() != nullptr)) {
        AVLNode<Key, Value> *temp = curr->getLeft();
        if (curr == this->root_) {
            this->root_ = temp;
            this->root_->setParent(nullptr);
            delete curr;
        } else {
            if (pcurr->getLeft() == curr) pcurr->setLeft(temp);
            else pcurr->setRight(temp);
            temp->setParent(pcurr);
            delete curr;
        }
    }
}

template<class Key, class Value>
AVLNode<Key,Value>* AVLTree<Key,Value>::remove_fix(AVLNode<Key,Value>* n, int diff) {
    if (n == nullptr) return nullptr;
    AVLNode<Key,Value>* pn = n->getParent();
    int ndiff = 0;
    if (pn != nullptr) {
        // if n is left child
        if (pn->getLeft() == n) ndiff = 1;
            // if n is right child
        else ndiff = -1;
    }
    if (n->getBalance() + diff == -2) {
        AVLNode<Key,Value>* c = n->getLeft();
        if (c->getBalance() == -1) {
            r_rotate(n);
            n->setBalance(0);
            c->setBalance(0);
            remove_fix(pn,ndiff);
        }
        else if (c->getBalance() == 0) {
            r_rotate(n);
            n->setBalance(-1);
            c->setBalance(1);
        }
        else if (c->getBalance() == 1) {
            AVLNode<Key,Value>* g = c->getRight();
            l_rotate(c);
            r_rotate(n);
            if (g->getBalance() == 1) {
                n->setBalance(0);
                c->setBalance(-1);
                g->setBalance(0);
            }
            else if (g->getBalance() == 0) {
                n->setBalance(0);
                c->setBalance(0);
                g->setBalance(0);
            }
            else if (g->getBalance() == -1) {
                n->setBalance(1);
                c->setBalance(0);
                g->setBalance(0);
            }
            remove_fix(pn,ndiff);
        }
    }
    else if (n->getBalance() + diff == 2) {
        AVLNode<Key,Value>* c = n->getRight();
        if (c->getBalance() == 1) {
            l_rotate(n);
            n->setBalance(0);
            c->setBalance(0);
            remove_fix(pn,ndiff);
        }
        else if (c->getBalance() == 0) {
            l_rotate(n);
            n->setBalance(1);
            c->setBalance(-1);
        }
        else if (c->getBalance() == -1) {
            AVLNode<Key,Value>* g = c->getRight();
            r_rotate(c);
            l_rotate(n);
            if (g->getBalance() == -1) {
                n->setBalance(0);
                c->setBalance(1);
                g->setBalance(0);
            }
            else if (g->getBalance() == 0) {
                n->setBalance(0);
                c->setBalance(0);
                g->setBalance(0);
            }
            else if (g->getBalance() == 1) {
                n->setBalance(-1);
                c->setBalance(0);
                g->setBalance(0);
            }
            remove_fix(pn,ndiff);
        }
    }
    else if (n->getBalance() + diff == -1) {
        n->setBalance(-1);
    }
    else if (n->getBalance() + diff == 1) {
        n->setBalance(1);
    }
    else if (n->getBalance() + diff == 0) {
        n->setBalance(0);
        remove_fix(pn,ndiff);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::internalFind(const Key& key) const
{
    AVLNode<Key,Value>* curr;
		curr->setLeft(this->root_->getLeft());
		curr->setRight(this->root_->getRight());
    while (true) {
        if (curr == nullptr) return nullptr; // i.e. key not found
        // Equal to key
        if (curr->getKey() == key) return curr;
            // Less than key
        else if (curr->getKey() < key) curr = curr->getRight();
            // Greater than key
        else curr = curr->getLeft();
    }
}

template<typename Key, typename Value>
AVLNode<Key,Value>* AVLTree<Key,Value>::r_rotate(AVLNode<Key,Value>* n) {
    AVLNode<Key,Value>* parent_n = n->getParent();
    AVLNode<Key,Value>* left_n = n->getLeft();
    AVLNode<Key,Value>* right_n = n->getRight();

    if (parent_n->getLeft() == n) parent_n->setLeft(right_n);
    else parent_n->setRight(right_n);
    left_n->setParent(parent_n);
    left_n->setRight(n);
    n->setRight(right_n);
}

template<typename Key, typename Value>
AVLNode<Key,Value>* AVLTree<Key,Value>::l_rotate(AVLNode<Key,Value>* n) {
    AVLNode<Key,Value>* parent_n = n->getParent();
    AVLNode<Key,Value>* left_n = n->getLeft();
    AVLNode<Key,Value>* right_n = n->getRight();

    if (parent_n->getLeft() == n) parent_n->setLeft(left_n);
    else parent_n->setRight(left_n);
    right_n->setParent(parent_n);
    right_n->setLeft(n);
    n->setRight(right_n);
}

template<class Key, class Value>
AVLNode<Key, Value>*
AVLTree<Key, Value>::predecessor(AVLNode<Key, Value>* current)
{
    // TODO
    if (current == nullptr) return current;
    if (current->getLeft() != nullptr) {
        AVLNode<Key,Value>* temp = current->getLeft();
        while (temp->getRight()) temp = temp->getRight();
        return temp;
    }
    return current->getParent();
}



#endif
