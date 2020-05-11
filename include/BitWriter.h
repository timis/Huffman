#ifndef BITWRITER_H
#define BITWRITER_H

#include "coding.h"
#include <unordered_map>
#include <deque>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/**
 * @brief Writes the huffman tree and huffman coded file bit by bit
 * 
 */
class BitWriter {
    public:
        BitWriter(string outFile);
        bool open();
        bool isOpen();
        void close();
        void insert(unordered_map<char,coding> &codeMap, char c);
        void writeTree(Node* root);
        void writeBit(int bit);
        void flush();
    
    private:
        string fileName;
        ofstream f;
        int numBits = 0;
        deque<coding> vec;
        char treeChar = '\0';
        int writeBits = 0;
        const int numbitsToMask[9] = {0x00,0x01,0x03,0x07,0x0f,0x1f,0x3f,0x7f,0xff};

};



#endif