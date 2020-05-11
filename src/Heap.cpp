#include <iostream>

#include "Heap.h"

using namespace std;

/**
 * @brief Construct a new Heap object with the given capacity
 * 
 * @param capacity The maximum number of nodes in the heap
 */
Heap::Heap(int capacity){
    this->capacity = capacity;
    this->heap = new Node*[capacity];
    this->size = 0;
}

/**
 * @brief Get the number of nodes in the heap
 * 
 * @return int 
 */
int Heap::getSize(){
    return size;
}

/**
 * @brief Remove a node from the heap
 * 
 * Done by making this node have the smallest count possible, restoring the heap properrty, and extracting the minimum
 * 
 * @param node 
 */
void Heap::remove(Node* node){
    node->count = INT_MIN;
    heapify(node->index);
    extractMin();
}

/**
 * @brief Insert a new node given the character and the count (number of occurences of that character in the huffman source text - aka the key for our heap)
 * 
 * Is basically a wrapper around the Heap::insert(Node*) function
 * 
 * @param c The character getting inserted
 * @param count The number of occurences of that character/the key
 */
void Heap::insert(char c, int count){
    Node* newNode = new Node;
    newNode->c = c;
    newNode->count = count;
    insert(newNode);
}

/**
 * @brief Insert the given node into the heap.
 * 
 * First, just insert it at the first open slot. Then, continually compare it with its parent, swapping up the tree until this node is larger than its immediate parent 
 * (which means the heap property is satisfied)
 * 
 * @param node The node to be inserted
 */
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

/**
 * @brief Returns the 0-th element of the heap (guaranteed to be the minimum due to the nature of the heap property). Then restores the heap property.
 * 
 * @return Node* 
 */
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

/**
 * @brief Restores the heap property starting from the given index and continuing down.
 * 
 * Works recursively - finds the smallest of a node and its left and right children, then swaps around and recurses if need be. If the parent is the smallest, does nothing
 * since that means the heap property is already satisfied
 * 
 * @param i The index in the heap
 */
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


#ifdef TEST
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