#include <iostream>
#include <unordered_map>
#include "coding.h"
#include <fstream>
#include "BitReader.h"
#include <iostream>
#include <vector>
#include <iomanip>

#ifdef DEBUG
#define CATCH_CONFIG_MAIN 
#include "catch.hpp"
#endif

using namespace std;

struct blah {
    coding c;
    char car;
};

vector<blah> vect;

void inorder(Node* t,int code,int numBits){
    if(t == NULL){
        return;
    }
    int leftcode = code << 1;
    int rightcode = code << 1 | 1;

    inorder(t->left, leftcode,numBits+1);
    if(t->left == nullptr || t->right == nullptr){
        blah b = {{numBits,code},t->c};
        vect.push_back(b);
        // cout << "Key: "<< int(t->c) << "    " << code << "    " << numBits << endl;
    }
    inorder(t->right,rightcode,numBits+1);
    
}

bool myfunc(blah a, blah b){
    return a.car < b.car;
}

#ifndef DEBUG
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
    sort(vect.begin(),vect.end(),myfunc);
    cout << "Char\tCode\t# Bits"<<endl;
    for(auto b : vect){
        cout << setw(8) << int(b.car) << setw(8) << b.c.code << setw(8) << b.c.numBits << endl;
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
#endif