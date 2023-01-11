/*

MIT License

Copyright (c) 2023 Matthew James Austin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#ifndef LINKEDLIST_H
#define LINKEDLIST_H


template <class T> class LinkedList; //so Node<T> can have a friend relationship with LinkedList<T>

//Linked List Node
template <class T>
class Node {

friend class LinkedList<T>;

public:
    T* obj; //public so object can be accessed outside of the Linked List

private:

    Node(T* o){ obj = o;}; //base constructor, prev and next are nullptrs (e.g. first item in the list)
    Node(T* o, Node<T>* p, Node<T>* n) : Node(o) {
        if (p != nullptr){
            prev = p;
            prev->next = this;
        }
        if (n != nullptr){
            next = n;
            next->prev = this;
        }
    };
    //destructor, fixes linked list positioning automatically
    ~Node(){
        if (prev != nullptr){
            prev->next = next; //if not the first then set prev node to point at the next node
        }
        if (next != nullptr){
            next->prev = prev; //if not the last then set next node to point at the prev node
        }
        delete obj; //dynamic memory clear up
        prev = nullptr;
        next = nullptr;
        obj = nullptr;
    };

    Node* prev = nullptr; //ptr to the previous node in the linked list
    Node* next = nullptr; //ptr to the next node in the linked list

};

//Linked List data structure
template <class T>
class LinkedList {

public:

    LinkedList(){first = nullptr; last = nullptr; cycle = nullptr;}; //initialise as an empty linked list
    ~LinkedList(){ //remove all items when list is destroyed
        while(first!=nullptr){
            remFront();
        }
    };
    //adds a new object to the front of the list
    void addFront(T* obj){
        //if list is empty
        if(first == nullptr){
            first = new Node<T>(obj);
            last = first;
        } else {
            first = new Node<T>(obj,nullptr,first);
        }
    };

    //adds a new object to the back of the list
    void addEnd(T* obj){
        //if list is empty
        if(last == nullptr){
            last = new Node<T>(obj);
            first = last;
        } else {
            last = new Node<T>(obj,last,nullptr);
        }
    };

    //removes the first object
    void remFront(){
        //if list is not empty
        if (first != nullptr){
            //if not the only item left
            if (first!=last){
                first = first->next; //set 2nd item to 1st
                delete first->prev; //delete original 1st item
            } else {
                delete first; //delete item
                first = nullptr; //set both first and last to null as list is empty
                last = nullptr;
            }
        }
    }

    //removes the last object
    void remEnd(){
        //if list is not empty
        if (last != nullptr){
            //if not the only item left
            if (last!=first){
                last = last->prev; //set 2nd to last item to last
                delete last->next; //delete original last item
                //last->next = nullptr; //set next pointer on new last to null
            } else {
                delete last; //delete item
                last = nullptr; //set both first and last to null as list is empty
                first = nullptr;
            }
        }
    }

    //removes a given node
    void remAtNode(Node<T>* node){
        if (node == first){
            remFront();
        } else if(node == last){
            remEnd();
        } else {
            delete node;
            node = nullptr;
        }
    }

    //removes the currently cycled object
    void remCycle(){
        if (cycle != nullptr){
            if (cycle == first){
                remFront();
            } else if (cycle == last){
                remEnd();
            } else {
                cycle = cycle->prev;
                delete cycle->next;
            }
        }
    }

    //resets the cycle to the front of the list, returns true if the list isn't empty
    bool resetCycle(){cycle = first; return (cycle != nullptr);}

    //pushes the cycle along the list by 1 iteration, returns true until the last item has been processed
    bool cycleUp(){
        if (cycle != nullptr){
            cycle = cycle->next;
        }
        return (cycle != nullptr);
    }

    Node<T>* first; //start node of the linked list
    Node<T>* last; //end node of the linked list
    Node<T>* cycle; //node ptr used to access items within the linked list

};





#endif
