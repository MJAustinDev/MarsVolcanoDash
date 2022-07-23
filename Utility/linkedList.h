/*

MIT License

Copyright (c) 2022 Matthew James Austin

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

//TODO convert functionality into .cpp file


template <class T>
class LinkedList;



template <class T>
class Node {

friend class LinkedList<T>;

public:
    T* obj;

private:
    Node(T* o){ obj = o; prev = nullptr; next = nullptr;};
    Node(T* o, Node<T>* p, Node<T>* n){
        obj = o;
        prev = p;
        next = n;
        if (prev!=nullptr){
            prev->next = this;
        }
        if (next!=nullptr){
            next->prev = this;
        }
    };

    ~Node(){
        if (prev!=nullptr){
            prev->next = next;
        }
        if (next!=nullptr){
            next->prev = prev;
        }
        delete obj;
        prev = nullptr;
        next = nullptr;
    };

    Node* prev;
    Node* next;



};


template <class T>
class LinkedList {

public:

    LinkedList(){first = nullptr; last = nullptr; cycle = nullptr;};
    ~LinkedList(){
        while(first!=nullptr){
            remFront();
        }
    };

    void addFront(T* obj){
        //if list is empty
        if(first==nullptr){
            first = new Node<T>(obj);
            last = first;
        } else {
            first = new Node<T>(obj,nullptr,first);
        }
    };

    void addEnd(T* obj){
        //if list is empty
        if(last==nullptr){
            last = new Node<T>(obj);
            first = last;
        } else {
            last = new Node<T>(obj,last,nullptr);
        }
    };

    void remFront(){
        //if list is not empty
        if (first!=nullptr){
            //if not the only item left
            if (first!=last){
                first = first->next; //set 2nd item to 1st
                delete first->prev; //delete original 1st item
                first->prev = nullptr; //set prev pointer on new 1st to null
            } else {
                delete first; //delete item
                first = nullptr; //set both first and last to null as list is empty
                last = nullptr;
            }
        }
    }

    void remEnd(){
        //if list is not empty
        if (last!=nullptr){
            //if not the only item left
            if (last!=first){
                last = last->prev; //set 2nd to last item to last
                delete last->next; //delete original last item
                last->next = nullptr; //set next pointer on new last to null
            } else {
                delete last; //delete item
                last = nullptr; //set both first and last to null as list is empty
                first = nullptr;
            }
        }
    }

    bool remAtNode(Node<T>* node){
        if (node==first){
            remFront();
            return true;
        } else if(node==last){
            remEnd();
            return true;
        } else {
            delete node;
            node = nullptr;
            return true;
        }
        return false;
    }

    void remCycle(){
        if (cycle!=nullptr){
            if (cycle==first){
                remFront();
            } else if (cycle==last){
                remEnd();
            } else {
                cycle = cycle->prev;
                delete cycle->next;
            }
        }

    }

    bool resetCycle(){cycle = first; return (cycle!=nullptr);}

    bool cycleUp(){
        if (cycle!=nullptr){
            cycle = cycle->next;
        }
        return (cycle!=nullptr);
    }

    Node<T>* first;
    Node<T>* last;
    Node<T>* cycle;

};





#endif
