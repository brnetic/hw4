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
    int8_t balance_; 
	   // effectively a signed char
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
		void insertFix(AVLNode<Key, Value>* r, AVLNode<Key, Value>* new_node);
		void rotateRight(AVLNode<Key, Value>* curr);
		void rotateLeft(AVLNode<Key, Value>* curr);
		int getBalance1(AVLNode<Key, Value>* node);
		void removeFix(AVLNode<Key, Value>* p, int8_t diff);


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
		AVLNode<Key, Value>* curr =  static_cast<AVLNode<Key,Value>*>(this->internalFind(new_item.first));
		if(curr){
			curr->setValue(new_item.second);
			return;
		}
		AVLNode<Key, Value>* new_node = new AVLNode<Key,Value>(new_item.first,new_item.second,nullptr);
		AVLNode<Key, Value>* t = static_cast<AVLNode<Key, Value>*>(this->root_);
		if(!this->root_){
			this->root_ = new_node;
		}
		while(t != nullptr){
        if(new_node->getKey()<t->getKey()){
					if(t->getLeft() == nullptr){	
							t->setLeft(new_node);
							new_node->setParent(t);
							new_node->setBalance(0);
							t->updateBalance(-1);
							if(t->getBalance() != 0){
								insertFix(t->getLeft(),t);
							}
							return;
						}
            t = t->getLeft();
        }
        else{
						if(t->getRight() == nullptr){	
							t->setRight(new_node);
							new_node->setParent(t);
							new_node->setBalance(0);
							t->updateBalance(1);
							if(t->getBalance() != 0){
								insertFix(t->getRight(),t);
							}
							return;
							
						}
            t = t->getRight();
        }

    }
		


}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* n, AVLNode<Key, Value>* p){
	if(!n || !p){
		return;
	}
	AVLNode<Key, Value>* g = p->getParent();
	if(g == nullptr){
		return;
	}
	if(g->getLeft() == p){
		g->updateBalance(-1);
		if(g->getBalance() == 0){
			return;
		}
		if(g->getBalance() == -1){
			insertFix(p,g);
		}
		if(g->getBalance() == -2){
			if(p->getLeft() == n){
				rotateRight(g);
				p->setBalance(0);
				g->setBalance(0);
			}
			else{
				rotateLeft(p);
				rotateRight(g);
				if(n->getBalance() == -1){
					p->setBalance(0);
					g->setBalance(1);
				}
				else if(n->getBalance() == 0){
					p->setBalance(0);
					g->setBalance(0);
				}
				else if(n->getBalance() == 1){
					p->setBalance(-1);
					g->setBalance(0);
				}
				n->setBalance(0);
			}
		}
	}
	else{
		g->updateBalance(1);
		if(g->getBalance() == 0){
			return;
		}
		if(g->getBalance() == 1){
			insertFix(p,g);
		}
		if(g->getBalance() == 2){
			if(p->getRight() == n){
				rotateLeft(g);
				p->setBalance(0);
				g->setBalance(0);
			}
			else{
				rotateRight(p);
				rotateLeft(g);
				if(n->getBalance() == -1){
					p->setBalance(1);
					g->setBalance(0);
				}
				else if(n->getBalance() == 0){
					p->setBalance(0);
					g->setBalance(0);
				}
				else if(n->getBalance() == 1){
					p->setBalance(0);
					g->setBalance(-1);
				}
				n->setBalance(0);
			}
		}
	}


}



template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* curr){
	AVLNode<Key, Value>* p = curr->getRight();
	AVLNode<Key, Value>* l = p->getLeft();
	if(curr == this->root_){
		this->root_	= p;
		this->root_->setParent(nullptr);
	}
	else{
		if(curr->getParent()->getLeft() == curr){
			curr->getParent()->setLeft(p);
		}
		else{
			curr->getParent()->setRight(p);
		}
		p->setParent(curr->getParent());
	}
	p->setLeft(curr);
	curr->setParent(p);
	curr->setRight(l);
	if(l){
		l->setParent(curr);
	}


	
}






template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* curr){
	AVLNode<Key, Value>* p = curr->getLeft();
	AVLNode<Key, Value>* r = p->getRight();
	if(curr == this->root_){
		this->root_	= p;
		this->root_->setParent(nullptr);
	}
	else{
		if(curr->getParent()->getLeft() == curr){
			curr->getParent()->setLeft(p);
		}
		else{
			curr->getParent()->setRight(p);
		}
		p->setParent(curr->getParent());
	}
	p->setRight(curr);
	curr->setParent(p);
	curr->setLeft(r);
	if(r){
		r->setParent(curr);
	}
}



template<class Key, class Value>
int AVLTree<Key, Value>::getBalance1(AVLNode<Key, Value>* r){
	if(!r){
		return -1;
	}
	else{
		return this->height(r->getLeft()) - this->height(r->getRight());
	}
}
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO

		AVLNode<Key,Value>* t = static_cast<AVLNode<Key,Value>*>(this->internalFind(key));
    if(t == nullptr){
        std::cout<<"This item is not stored in this tree"<<std::endl;
        return;
    }
		if(t->getLeft() && t->getRight()){  //2 children
			AVLNode<Key,Value>* pred = static_cast<AVLNode<Key,Value>*>(this->predecessor(t));
			nodeSwap(t,pred);
			if(pred->getLeft() == t && !t->getLeft()){
				pred->setLeft(nullptr);
				removeFix(pred,1);
				delete t;
				return;
			}
			if(t->getLeft()){
				if(t->getParent()->getLeft() == t){
					t->getParent()->setLeft(t->getLeft());
					t->getLeft()->setParent(t->getParent());
					removeFix(t->getParent(),1);
					delete t;
				}
				else{
					t->getParent()->setRight(t->getLeft());
					t->getLeft()->setParent(t->getParent());
					removeFix(t->getParent(),-1);
					delete t;
				}
				return;
			}
			if(t->getParent()->getLeft() == t){
					t->getParent()->setLeft(nullptr);
					removeFix(t->getParent(),1);
					delete t;
				}
			else{
				removeFix(t->getParent(),-1);
				t->getParent()->setRight(nullptr);
				delete t;
			}
			return;

		}
		if(t->getLeft()){
			if(t == this->root_){
				AVLNode<Key,Value> *temp = t;
				this->root_ = t->getLeft();
				t->getLeft()->setParent(nullptr);
				removeFix(static_cast<AVLNode<Key,Value>*>(this->root_),1);
				delete temp;
				return;
			}
			if(t->getParent()->getLeft() == t){
				t->getParent()->setLeft(t->getLeft());
				removeFix(t->getParent(),1);
				t->getLeft()->setParent(t->getParent());
				delete t;
			}
			else{
				t->getParent()->setRight(t->getLeft());
				t->getLeft()->setParent(t->getParent());
				removeFix(t->getParent(),-1);
				delete t;
			}
			return;
		}
		if(t->getRight()){
			if(t == this->root_){
				AVLNode<Key,Value> *temp = t;
				this->root_ = t->getRight();
				t->getRight()->setParent(nullptr);
				removeFix(static_cast<AVLNode<Key,Value>*>(this->root_),-1);
				delete temp;
				return;
			}
			if(t->getParent()->getLeft() == t){
				t->getParent()->setLeft(t->getRight());
				t->getRight()->setParent(t->getParent());
				removeFix(t->getParent(),1);
				delete t;
			}
			else{
				t->getParent()->setRight(t->getRight());
				t->getRight()->setParent(t->getParent());
				removeFix(t->getParent(),-1);
				delete t;
			}
			return;
		}
		if(!t->getLeft() && !t->getRight()){
			if(this->root_ == t){
				delete this->root_;
				this->root_ = nullptr;
				return;
			}
			if(t->getParent()->getLeft() == t){
				
				t->getParent()->setLeft(nullptr);
				removeFix(t->getParent(),1);
				delete t;
				return;
			}
			else{
				
				t->getParent()->setRight(nullptr);
				removeFix(t->getParent(),-1);
				delete t;
				return;
			}
		}
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n, int8_t diff){
	if(!n){
		return;
	}
	int8_t ndiff;
    AVLNode<Key, Value>* p = n->getParent();
		if(p){
			if(p->getLeft() == n){
				ndiff = 1;
			}
			else{
				ndiff = -1;
			}
		}
    
        
        if(n->getBalance()+diff == -2){
          AVLNode<Key, Value>* c = n->getLeft();
					if(c->getBalance() == -1){
						rotateRight(n);
						n->setBalance(0);
						c->setBalance(0);
						removeFix(p,ndiff);
					}
					else if(c->getBalance() == 0){
						rotateRight(n);
						n->setBalance(-1);
						c->setBalance(1);
					}
					else if(c->getBalance() == 1){
						AVLNode<Key, Value>* g = c->getRight();
						rotateLeft(c);
						rotateRight(n);
						if(g->getBalance() == 1){
							n->setBalance(0);
							c->setBalance(-1);
							g->setBalance(0);
						}
						else if(g->getBalance() == 0){
							n->setBalance(0);
							c->setBalance(0);
							g->setBalance(0);
						}
						else if(g->getBalance() == -1){
							n->setBalance(1);
							c->setBalance(0);
							g->setBalance(0);
						}
						removeFix(p,ndiff);
					}
					

        }
				else if(n->getBalance()+diff == -1){
					n->setBalance(-1);
				}
				else if(n->getBalance()+diff == 0){
					n->setBalance(0);
					removeFix(p,ndiff);
				}
    
    
        
        else if(n->getBalance()+diff == 2){
          AVLNode<Key, Value>* c = n->getRight();
					if(c->getBalance() == 1){
						rotateLeft(n);
						n->setBalance(0);
						c->setBalance(0);
						removeFix(p,ndiff);
					}
					else if(c->getBalance() == 0){
						rotateLeft(n);
						n->setBalance(1);
						c->setBalance(-1);
					}
					else if(c->getBalance() == -1){
						AVLNode<Key, Value>* g = c->getLeft();
						rotateRight(c);
						rotateLeft(n);
						if(g->getBalance() == -1){
							n->setBalance(0);
							c->setBalance(1);
							g->setBalance(0);
						}
						else if(g->getBalance() == 0){
							n->setBalance(0);
							c->setBalance(0);
							g->setBalance(0);
						}
						else if(g->getBalance() == 1){
							n->setBalance(-1);
							c->setBalance(0);
							g->setBalance(0);
						}
						removeFix(p,ndiff);
					}
					

        }
				else if(n->getBalance()+diff == 1){
					n->setBalance(1);
				}
				else if(n->getBalance()+diff == 0){
					n->setBalance(0);
					removeFix(p,ndiff);
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


#endif
