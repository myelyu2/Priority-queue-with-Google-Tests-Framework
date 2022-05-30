// priorityqueue.h
//
// This file contains the class priorityqueue which implements
// abstract class priority queue using self-balanced BST - AVL tree
// In case of duplicates entries are sorted by addition time
#pragma once

#include <iostream>
#include <sstream>
#include <set>

using namespace std;

template<typename T>
class priorityqueue {
private:
    struct NODE {
        int priority;  // used to build BST
        T value;  // stored data for the p-queue
        bool dup;  // marked true when there are duplicate priorities
        NODE *parent;  // links back to parent
        NODE *link;  // links to linked list of NODES with duplicate priorities
        NODE *left;  // links to left child
        NODE *right;  // links to right child
        int height; // Parameter for tree balancing

        NODE(int priority, T value, NODE *parent = nullptr) : priority(priority), value(value),
                                                              dup(false), parent(parent), link(nullptr),
                                                              left(nullptr), right(nullptr), height(1) {};

        void print(ostream &out) {
            NODE *cur = this;
            bool finished = false;
            do {
                out << cur->priority << " value: " << cur->value << endl;
                if (cur->dup) {
                    cur = cur->link;
                } else {
                    finished = true;
                }
            } while (!finished);
        }
    };

    NODE *root;  // pointer to root node of the BST
    int size;  // # of elements in the pqueue
    NODE *curr;  // pointer to next item in pqueue (see begin and next)

    void setParent(NODE *t, NODE *parent) {
        if (t == nullptr) {
            return;
        }
        t->parent = parent;
        if (t->dup) {
            setParent(t->link, parent);
        }
    }

    void setRight(NODE *t, NODE *right) {
        if (t == nullptr) {
            return;
        }
        t->right = right;
        if (t->dup) {
            setRight(t->link, right);
        }
    }

    void setLeft(NODE *t, NODE *left) {
        if (t == nullptr) {
            return;
        }
        t->left = left;
        if (t->dup) {
            setLeft(t->link, left);
        }
    }

    // AVL tree
    int getHeight(NODE *t) {
        return t ? t->height : 0;
    }

    bool contains(NODE *t, NODE *val) {
        if (t == val) {
            return true;
        }
        if (t != nullptr && t->dup) {
            return contains(t->link, val);
        }
        return false;
    }

    NODE *rightRotate(NODE *y) {
        NODE *x = y->left;
        NODE *t2 = x->right;

        setRight(x, y);
        setParent(x, y->parent);
        setParent(y, x);
        setLeft(y, t2);
        setParent(t2, y);

        y->height = max(getHeight(y->left),
                        getHeight(y->right)) + 1;
        x->height = max(getHeight(x->left),
                        getHeight(x->right)) + 1;

        return x;
    }

    NODE *leftRotate(NODE *x) {
        NODE *y = x->right;
        NODE *t2 = y->left;

        setLeft(y, x);
        setParent(y, x->parent);
        setParent(x, y);
        setRight(x, t2);
        setParent(t2, x);

        x->height = max(getHeight(x->left),
                        getHeight(x->right)) + 1;
        y->height = max(getHeight(y->left),
                        getHeight(y->right)) + 1;

        return y;
    }

    int getBalance(NODE *N) {
        if (N == nullptr) {
            return 0;
        }
        return getHeight(N->left) - getHeight(N->right);
    }

    NODE *insert(NODE *node, int priority, T value, NODE *parent = nullptr) {
        if (node == nullptr) {
            return new NODE(priority, value, parent);
        }

        if (priority < node->priority) {
            setLeft(node, insert(node->left, priority, value, node));
        } else if (priority > node->priority) {
            setRight(node, insert(node->right, priority, value, node));
        } else {
            NODE *cur = node, *new_node = new NODE(priority, value);
            while (cur->dup) {
                cur = cur->link;
            }
            new_node->parent = cur->parent;
            new_node->left = cur->left;
            new_node->right = cur->right;
            cur->link = new_node;
            cur->dup = true;

            return node;
        }

        node->height = 1 + max(getHeight(node->left),
                               getHeight(node->right));

        int balance = getBalance(node);

        // Left Left Case
        if (balance > 1 && priority < node->left->priority)
            return rightRotate(node);

        // Right Right Case
        if (balance < -1 && priority > node->right->priority)
            return leftRotate(node);

        // Left Right Case
        if (balance > 1 && priority > node->left->priority) {
            setLeft(node, leftRotate(node->left));
            return rightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && priority < node->right->priority) {
            setRight(node, rightRotate(node->right));
            return leftRotate(node);
        }

        return node;
    }

    NODE *minNode(NODE *node) {
        if (node == nullptr) {
            return nullptr;
        }
        NODE *current = node;

        while (current->left != nullptr)
            current = current->left;

        return current;
    }


    NODE *deleteNode(NODE *node, NODE *to_delete, bool remove_linked) {
        if (node == nullptr) {
            return node;
        }

        if (to_delete->priority < node->priority) {
            setLeft(node, deleteNode(node->left, to_delete, remove_linked));
        } else if (to_delete->priority > node->priority)
            setRight(node, deleteNode(node->right, to_delete, remove_linked));
        else {
            if (node->dup && !remove_linked) {
                NODE *temp = node->link;

                temp->height = node->height;
                setParent(temp, node->parent);
                setRight(temp, node->right);
                setLeft(temp, node->left);
                setParent(temp->left, temp);
                setParent(temp->right, temp);

                delete node;
                node = temp;
            } else {
                if (node->left == nullptr || node->right == nullptr) {
                    NODE *temp = node->left ? node->left : node->right;

                    if (temp == nullptr) {
                        temp = node;
                        node = nullptr;
                    } else {
                        setParent(temp, node->parent);
                        *node = *temp;
                    }
                    delete temp;
                } else {
                    NODE *temp = minNode(node->right);

                    node->priority = temp->priority;
                    node->value = temp->value;
                    node->dup = temp->dup;
                    node->link = temp->link;
                    if (node->dup) {
                        setParent(node->link, node);
                    }

                    setRight(node, deleteNode(node->right, temp, true));
                }
            }

        }

        if (node == nullptr) {
            return node;
        }

        node->height = 1 + max(getHeight(node->left),
                               getHeight(node->right));

        int balance = getBalance(node);

        // Left Left Case
        if (balance > 1 &&
            getBalance(node->left) >= 0)
            return rightRotate(node);

        // Left Right Case
        if (balance > 1 &&
            getBalance(node->left) < 0) {
            setLeft(node, leftRotate(node->left));
            return rightRotate(node);
        }

        // Right Right Case
        if (balance < -1 &&
            getBalance(node->right) <= 0)
            return leftRotate(node);

        // Right Left Case
        if (balance < -1 &&
            getBalance(node->right) > 0) {
            setRight(node, rightRotate(node->right));
            return leftRotate(node);
        }

        return node;
    }

    NODE *getNext(NODE *t) {
        if (t == nullptr) {
            return nullptr;
        }
        if (t->dup) {
            return t->link;
        }
        if (t->right != nullptr) {
            return minNode(t->right);
        }
        NODE *cur = t;
        while (cur->parent != nullptr) {
            NODE *par = cur->parent;
            if (contains(par->left, cur)) {
                return par;
            }
            cur = par;
        }
        return nullptr;
    }

    NODE *copyNode(NODE *t, NODE *parent = nullptr) {
        if (t == nullptr) {
            return nullptr;
        }
        NODE *result = new NODE(t->priority, t->value);

        result->height = t->height;
        result->dup = t->dup;
        result->link = t->link;

        NODE *cur = result;
        while (cur->dup) {
            cur = cur->link;
            auto tmp = new NODE(cur->priority, cur->value);
            tmp->dup = cur->dup;
            tmp->link = cur->link;
            cur = tmp;
        }

        setParent(result, parent);
        setLeft(result, copyNode(t->left, result));
        setRight(result, copyNode(t->right, result));

        return result;
    }


    void print(NODE *node, ostream &out) const {
        if (node == nullptr) {
            return;
        }

        print(node->left, out);

        node->print(out);

        print(node->right, out);
    }


    void clear(NODE *t) {
        if (t == nullptr) {
            return;
        }
        clear(t->left);
        clear(t->right);
        delete t;
    }

public:
    //
    // default constructor:
    //
    // Creates an empty priority queue.
    // O(1)
    //
    priorityqueue() : root(nullptr), size(0), curr(nullptr) {}

    //
    // operator=
    //
    // Clears "this" tree and then makes a copy of the "other" tree.
    // Sets all member variables appropriately.
    // O(n), where n is total number of nodes in custom BST
    // Resets current to first inorder element
    priorityqueue &operator=(const priorityqueue &other) {
        if (this == &other) {
            return *this;
        }

        clear();
        root = copyNode(other.root);
        size = other.size;
        begin();

        return *this;
    }

    //
    // clear:
    //
    // Frees the memory associated with the priority queue but is public.
    // O(n), where n is total number of nodes in custom BST
    //
    void clear() {
        clear(root);
        root = nullptr;
        size = 0;
        curr = nullptr;
    }

    //
    // destructor:
    //
    // Frees the memory associated with the priority queue.
    // O(n), where n is total number of nodes in custom BST
    //
    ~priorityqueue() {
        clear();
    }

    //
    // enqueue:
    //
    // Inserts the value into the custom BST in the correct location based on
    // priority.
    // O(logn + m), where n is number of unique nodes in tree and m is number of
    // duplicate priorities
    //
    void enqueue(T value, int priority) {
        size++;
        root = insert(root, priority, value);
    }

    //
    // dequeue:
    //
    // returns the value of the next element in the priority queue and removes
    // the element from the priority queue.
    // O(logn + m), where n is number of unique nodes in tree and m is number of
    // duplicate priorities
    //
    T dequeue() {
        size--;
        auto nodeOut = minNode(root);
        T valueOut = nodeOut->value;
        root = deleteNode(root, nodeOut, false);
        return valueOut;


    }

    //
    // Size:
    //
    // Returns the # of elements in the priority queue, 0 if empty.
    // O(1)
    //
    int Size() {
        return this->size;
    }

    //
    // begin
    //
    // Resets internal state for an inorder traversal.  After the
    // call to begin(), the internal state denotes the first inorder
    // node; this ensure that first call to next() function returns
    // the first inorder node value.
    //
    // O(logn), where n is number of unique nodes in tree
    //
    // Example usage:
    //    pq.begin();
    //    while (tree.next(value, priority)) {
    //      cout << priority << " value: " << value << endl;
    //    }
    //    cout << priority << " value: " << value << endl;
    void begin() {
        curr = minNode(root);
    }

    //
    // next
    //
    // Uses the internal state to return the next inorder priority, and
    // then advances the internal state in anticipation of future
    // calls.  If a value/priority are in fact returned (via the reference
    // parameter), true is also returned.
    //
    // False is returned when the internal state has reached null,
    // meaning no more values/priorities are available.  This is the end of the
    // inorder traversal.
    //
    // O(?) - hard to say.  But approximately O(logn + m).  Definitely not O(n).
    //
    // Example usage:
    //    pq.begin();
    //    while (tree.next(value, priority)) {
    //      cout << priority << " value: " << value << endl;
    //    }
    //
    bool next(T &value, int &priority) {
        if (curr == nullptr) {
            return false;
        }

        value = curr->value;
        priority = curr->priority;

        curr = getNext(curr);

        return true;


    }

    //
    // toString:
    //
    // Returns a string of the entire priority queue, in order.  Format:
    // "1 value: Ben
    //  2 value: Jen
    //  2 value: Sven
    //  3 value: Gwen"
    //
    string toString() const {
        stringstream st;
        print(root, st);
        return st.str();
    }

    //
    // peek:
    //
    // returns the value of the next element in the priority queue but does not
    // remove the item from the priority queue.
    // O(logn + m), where n is number of unique nodes in tree and m is number of
    // duplicate priorities
    //
    T peek() {
        T valueOut = minNode(root)->value;
        return valueOut;


    }

    //
    // ==operator
    //
    // Returns true if this priority queue as the priority queue passed in as
    // other.  Otherwise returns false.
    // O(n), where n is total number of nodes in custom BST
    //
    bool operator==(const priorityqueue<T> &other) const {
        return this->toString() == other.toString();
    }

    //
    // getRoot - Do not edit/change!
    //
    // Used for testing the BST.
    // return the root node for testing.
    //
    void *getRoot() {
        return root;
    }
};
