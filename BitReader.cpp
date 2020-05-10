
// Node ReadNode(BitReader reader)
// {
//     if (reader.ReadBit() == 1)
//     {
//         return new Node(reader.ReadByte(), null, null);
//     }
//     else
//     {
//         Node leftChild = ReadNode(reader);
//         Node rightChild = ReadNode(reader);
//         return new Node(0, leftChild, rightChild);
//     }
// }

#include "BitReader.h"

BitReader::BitReader(string inFile){
    fileName = inFile;
}

bool BitReader::open(){
    f.open(fileName, ios::in | ios::binary);
    return f.good();
}

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

void BitReader::setRoot(Node* n){
    root = n;
}

char BitReader::getByte(){
    char c = '\0';
    for(int i = 0; i < 8; i++){
        c <<= 1;
        c |= getNextBit();
    }
    // cout << "Got byte " << int(c) << endl; 
    return c;
}

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

void BitReader::prep(){
    bitChar = '\0';
    bitsRead = 8;
}