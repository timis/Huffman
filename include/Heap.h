#ifndef HEAP_H
#define HEAP_H

#include "coding.h"


class Heap {
    public: 
        Heap(int capacity);
        void insert(Node* node);
        void insert(char c, int count);
        int getSize();
        void remove(Node* node);
        Node* extractMin();
        void heapify(int i);

    private:
        int size;
        int capacity;
        Node** heap;
        int parent(int i) {return (i-1)/2;}
        int left(int i) { return (2*i + 1);}
        int right(int i) { return (2*i + 2);}
};

#endif