#ifndef CS1_BTREE_HPP_
#define CS1_BTREE_HPP_
////////////////////////////////////////////////////////////////////
// File:         btree.hpp
// Programmer:   Dr. J. Maletic
//
// Description:  Binary Tree
//
//

#include <iostream>
#include <cassert>

template <typename T> class bnode;

//IMPLEMENTED bnode dtor and copy ctor.

///////////////////////////////////////////////
//
//   Binary Search Tree
//
//   REQUIRES: DefaultConstructable(T) && 
//             Assignable(T) && 
//             Comparable(T) &&
//             Copyconstructable(T) && 
//             Printable(T)
//
template <typename T>
class btree {
public:
              btree     () : root(0)      {};
              btree     (const btree<T>&);
              ~btree    ();
    void      swap      (btree<T>&);
    btree<T>& operator= (btree<T> rhs)    {swap(rhs); return *this;};
    bool      isEmpty   () const          {return root;};
    bool      isFull    () const;
    void      binsert   (const T&);
    void      bremove   (const T&);
    bool      find      (const T&) const;
    void      inorder   (std::ostream&) const;

private:
    bnode<T>  *root;
};

///////////////////////////////////////////////
// binary tree node
//      -------------------
//      |       data      |
//      -------------------
//      | *left  | *right |
//      -------------------
template <typename T>
class bnode {
public:
              bnode      ()           : left(0), right(0)          {};
              bnode      (const T& x) : left(0), right(0), data(x) {};
              bnode      (const bnode<T>&);
              ~bnode     ();
    bool      find       (const T&) const;
    void      binsert    (const T&);
    bnode<T>* bremove    (const T&);
    void      inorder    (std::ostream&) const;
    T         predecessor() const;
    bnode<T>  *left,
              *right;
    T         data;
};






////////////////////////////////////////////////////////////////
// ENSURES: if key in Tree then
//            RETVAL == true else RETVAL == false
//
template <typename T>
bool btree<T>::find(const T& key) const {
    if (!root) return false;
    return root->find(key);
}

template <typename T>
bool bnode<T>::find(const T& key) const {
    if (key == data) return true;
    if (key < data) {
        if (left) return left->find(key);
        else return false;
    }else {
        if (right) return right->find(key);
        else return false;
    }
}


////////////////////////////////////////////////////////////////
// REQUIRES: !isFull()
// ENSURES: x is in BST
//
template <typename T>
void btree<T>::binsert(const T& x) {
    if (!root) root = new bnode<T>(x);
    else root->binsert(x);
}

template <typename T>
void bnode<T>::binsert(const T& x) {
    if (x < data) {
        if (left) left->binsert(x);
        else left = new bnode<T>(x);
    } else {
        if (right) right->binsert(x);
        else right = new bnode<T>(x);
    }
}


////////////////////////////////////////////////////////////////
template <typename T>
void btree<T>::inorder(std::ostream& out) const {
    if (root) root->inorder(out);
}

template <typename T>
void bnode<T>::inorder(std::ostream& out) const {
    if (left) left->inorder(out);
    out << data << " ";
    if (right) right->inorder(out);
}



////////////////////////////////////////////////////////////////
// REQUIRES:
// ENSURES:
//
template <typename T>
btree<T>::btree(const btree<T>& actual) : btree() {
    if (actual.root)
        root = new bnode<T>(*(actual.root));
}

template <typename T>
bnode<T>::bnode(const bnode<T>& actual) : bnode() {
    data = actual.data;
    if (actual.left) left = new bnode<T>(*(actual.left));
    if (actual.right) right = new bnode<T>(*(actual.right));
}


////////////////////////////////////////////////////////////////
// REQUIRES:
// ENSURES:
//
template <typename T>
btree<T>::~btree() {
    if (root)
        delete root;
}

template <typename T>
bnode<T>::~bnode() {
    if (left)  delete left;
    if (right) delete right;
    // this will be deleted next.
}


///////////////////////////////////////////////
//PRE:  Given left subtree of a node x
//POST: Returns predecessor of x
template <typename T>
T bnode<T>::predecessor () const {
    if (right) return right->predecessor();
    return data;
}

///////////////////////////////////////////////////////////
// REQUIRES x in Tree
// ENSURES:  x removed from Tree
template <typename T>
void btree<T>::bremove(const T& x) {
    if (find(x)) root = root->bremove(x);
}

template <typename T>
bnode<T>* bnode<T>::bremove(const T& x) {
    if (x == data) {
        if (!left && !right) { // No children
            delete this;
            return 0;
        }
        if (!left && right) { // Right child only
            bnode<T> *temp = right;
            right = 0;
            delete this;
            return temp;
        }
        if (left && !right) { // Left child only
            bnode<T> *temp = left;
            left = 0;
            delete this;
            return temp;
        }
        //Two children
        data = left->predecessor();
        left = left->bremove(data);
        return this;
    }
    else if (x < data) left = left->bremove(x);
    else right = right->bremove(x);
    return this;
}




#endif