#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "BitWriter.h"
#include "BitReader.h"
#include <string>

using namespace std;

class Huffman {
    public:
        void encode(string inputFile, string encodedFile,string mapFile="");
        void decode(string encodedFile, string outputFile, string mapFile = "");
    private:
        void buildMapInOrder(unordered_map<char, coding> &map,Node* t,int code,int numBits);
        void inorderTreeTraversal(Node* t,int code,int numBits);
        vector<coding> vect;

        
};


#endif