#include <iostream>
#include <unordered_map>
#include "coding.h"
#include <fstream>
#include "BitReader.h"
#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

vector<coding> vect;

/**
 * @brief Does an inorder traversal of the encoding tree in order to build up a single vector of codings (used mostly as a debugging tool)
 * 
 * @param t The current node in the tree
 * @param code The current code - code is built recursively by shifting and (possibly) adding one (ie, 0 becomes -> 00 and 01, which become 000,001,010,011 etc.)
 * @param numBits The current number of bits (just increases by one with each recursive level)
 */
void inorder(Node* t,int code,int numBits){
    if(t == NULL){
        return;
    }
    int leftcode = code << 1;
    int rightcode = code << 1 | 1;

    inorder(t->left, leftcode,numBits+1);
    if(t->left == nullptr || t->right == nullptr){
        vect.push_back({numBits,code,t->c});
    }
    inorder(t->right,rightcode,numBits+1);   
}

int main(int argc, char** argv){
    if(argc != 3){
        cout << "Proper usage: ./reader  ENCODED_INPUT_FILE_NAME  DECODED_FILE_NAME" << endl;
        return 0;
    }
    ofstream out;
    BitReader b(argv[1]);
    if(!b.open()){
        cout << "Could not open encoded input file "<< argv[1] << endl;
        return 0;
    }
    Node* root = b.readTree();
    b.setRoot(root);
    b.prep();
    inorder(root,0,0);
    sort(vect.begin(),vect.end(),codeCharComparison);
    cout << "Char\tCode\t# Bits"<<endl;
    for(auto b : vect){
        cout << setw(8) << int(b.character) << setw(8) << b.code << setw(8) << b.numBits << endl;
    }

    out.open(argv[2], ios::out);
    if(!out.good()){
        cout << "Could not open output file "<< argv[2] << endl;
    }
    char c;
    while(true){
        c = b.getNext();
        if(c == '\0'){
            break;
        }
        out.put(c);
    }
    out.close();


    return 0;
}
