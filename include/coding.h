#ifndef CODING_H
#define CODING_H

/**
 * @brief Basic object containing information about each character encoding
 * 
 */
struct coding {
    int numBits; // The number of bits for a given encoding (ie, 001 has 3 bits, 110101 has 6 bits)
    int code; // The integer representation of a code (ie 001 would be 1, 110101 would be 53)
    char character; // The character represented by the code
};

/**
 * @brief A Node of the Huffman tree (also used by the Heap implementation)
 * 
 */
struct Node {
    char c; // The character if we are at a leaf node; the null character if an internal node
    int count = 0; // The number of instances of this character (used by the Heap - only used for encoding)
    int index = 0; // This nodes location in the Heap
    Node* left = nullptr; // The left child of this node (null if a leaf node)
    Node* right = nullptr; // The right child of this node (null if a leaf node)
};

/**
 * @brief A helper funtion for ordering a vector<coding> by the character
 * 
 * @param a 
 * @param b 
 * @return true 
 * @return false 
 */
bool codeCharComparison(coding a, coding b){
    return a.character < b.character;
}

#endif