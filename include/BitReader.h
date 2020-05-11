#ifndef BIT_READER_H
#define BIT_READER_H

#include "coding.h"
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

/**
 * @brief Reads the huffman tree and huffman coded file bit by bit
 * 
 */
class BitReader {
    public:
        BitReader(string infile);
        bool open();
        void close();
        Node* readTree();
        char getNext();
        void prep();
        void setRoot(Node* n);
    private:
        ifstream f;
        string fileName;
        int getNextBit();
        char getByte();
        char bitChar;
        int bitsRead=8;
        Node* root;
};

#endif