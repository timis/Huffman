#include <iostream>

#include "Heap.h"

using namespace std;

Heap::Heap(int capacity){
    this->capacity = capacity;
    this->heap = new Node*[capacity];
    this->size = 0;
}

int Heap::getSize(){
    return size;
}

void Heap::remove(Node* node){
    node->count = INT_MIN;
    heapify(node->index);
    extractMin();
}

void Heap::insert(char c, int count){
    Node* newNode = new Node;
    newNode->c = c;
    newNode->count = count;
    insert(newNode);
}

void Heap::insert(Node* node){
    if(size >= capacity){
        cerr << "Heap is full already - errors must exist somewhere" << endl;
        return;
    }
    int i = size;
    size++;
    heap[i] = node;
    node->index = i;
    while(i != 0 && heap[parent(i)]->count > heap[i]->count){
        Node* temp = heap[i];
        heap[i] = heap[parent(i)];
        heap[parent(i)] = temp;
        temp->index = parent(i);
        heap[i]->index = i;
        i = parent(i);
    }
}

Node* Heap::extractMin(){
    if(size <= 0){
        return NULL;
    }
    if(size == 1){
        size--;
        return heap[0];
    }
    Node* root = heap[0];
    heap[0] = heap[size-1];
    heap[0]->index = 0;
    size--;
    heapify(0);
    return root;
}

void Heap::heapify(int i){
    int l = left(i);
    int r = right(i);
    int smallest = i;
    if(l < size && heap[l]->count < heap[smallest]->count){
        smallest = l;
    }
    if(r < size && heap[r]->count < heap[smallest]->count){
        smallest = r;
    }
    if(smallest != i){
        Node* temp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = temp;
        temp->index = smallest;
        heap[i]->index = i;
        heapify(smallest);
    }
}


#ifdef DEBUG
    #include "catch.hpp"
    TEST_CASE("Heap works correctly", "[Heap]"){
        Heap h(10);
        SECTION("Empty on start"){
            REQUIRE(h.getSize() == 0);
        }
        SECTION("Insertion grows size"){
            h.insert('a',0);
            REQUIRE(h.getSize() == 1);
        }
        SECTION("Extract of size one returns the element and then has size 0"){
            h.insert('a',5);
            REQUIRE(h.extractMin()->count == 5);
            REQUIRE(h.getSize() == 0);
        }
        SECTION("Cannot extract from an empty heap"){
            REQUIRE(h.extractMin() == NULL);
        }
        SECTION("Extract does get the min of a multi-sized heap"){
            h.insert('a', 5);
            h.insert('b', 0);
            Node* min = h.extractMin();
            REQUIRE(min->count == 0);
            REQUIRE(min->c == 'b');
            h.insert('b', 0);
            h.insert('c', 3);
            REQUIRE(h.extractMin()->count == 0);
        }
    }
#endif