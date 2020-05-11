#include "BitReader.h"

/**
 * @brief Construct a new BitReader object
 * 
 * @param inFile The name of the encoded input file
 */
BitReader::BitReader(string inFile){
    fileName = inFile;
}

/**
 * @brief Opens the encoded input file and returns whether it actually opened
 * 
 * @return true 
 * @return false 
 */
bool BitReader::open(){
    f.open(fileName, ios::in | ios::binary);
    return f.good();
}

/**
 * @brief Gets the next bit from the encoded input file
 * 
 * Uses a class member character and integer that keeps track of the number of bits read. After 8 bits (ie, the entire byte)
 * have been read, it will read in a new character from the file and reset the bitsRead counter.
 * 
 * @return int Returns either 1 or 0
 */
int BitReader::getNextBit(){
    if(bitsRead > 8){
        cerr << "ERROR - BitReader::getNextBit - Somehow bitsRead is above 8"<<endl;
    }
    if(bitsRead == 8){
        // cout << "Gettng a new char"<<endl;
        if(!f.get(bitChar)){
            return -1;
        }
        bitsRead = 0;
    }
    int ret = (bitChar >> (7-bitsRead))&1;
    bitsRead++;
    return ret;
}

/**
 * @brief Reads the huffman tree map from the beginning of the input file
 * 
 * The tree is encoded in the following scheme:
 *  If there is a "1" bit, the next 8 bits represent the ASCII character of this leaf node
 *  If there is a "0" bit, recurse twice to get the left and right of this internal node
 * 
 * @return Node* The root of the encoding tree
 */
Node* BitReader::readTree(){
    if(getNextBit() == 1){
        Node* ret = new Node;
        ret->c = getByte();
        return ret;
    } else {
        Node* left = readTree();
        Node* right = readTree();
        Node* ret = new Node;
        ret->left = left;
        ret->right = right;
        return ret;
    }
}

/**
 * @brief Just sets the root of the tree
 * 
 * TODO - This class can be refactored to have this set automatically by changing readTree to add a helper
 * 
 * @param n The root of the encoding tree
 */
void BitReader::setRoot(Node* n){
    root = n;
}

/**
 * @brief Reads a full byte from the file by reading the next 8 bits (since there's no guarantee that the character will be aligned on an actual byte boundary)
 * 
 * @return char 
 */
char BitReader::getByte(){
    char c = '\0';
    for(int i = 0; i < 8; i++){
        c <<= 1;
        c |= getNextBit();
    }
    // cout << "Got byte " << int(c) << endl; 
    return c;
}

/**
 * @brief Gets the next decoded character from the file
 * 
 * Reads bit by bit in order to descend the huffman tree. When it hits a leaf node it returns the character at that node
 * 
 * For instance, if the huffman tree is as follows:
 *               NODE
 *            NODE    C
 *          A     B
 * Reading a bit of '1', will make us go down the right side of the root. This puts us at the leaf node containing 'C'.
 * If we read a bit of '0', we would go to the left. If we then read another 0, we'd be at the 'A' node. If we read a 
 * '1', then we would go to the 'B' node and return it.
 * 
 * @return char The decoded character
 */
char BitReader::getNext(){
    Node* temp = root;
    while(temp->left || temp->right){
        int i = getNextBit();
        if(i == -1){
            return '\0';
        }
        if(i == 1){
            temp = temp->right;
        } else {
            temp = temp->left;
        }
    }
    return temp->c;
}

/**
 * @brief Helper function that clears out the remaining state from reading the tree in order to prep for reading the encoded file
 * 
 * Can probably be rendered obsolete with the refactor of readTree
 * 
 */
void BitReader::prep(){
    bitChar = '\0';
    bitsRead = 8;
}