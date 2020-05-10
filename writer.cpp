#include <iostream>
#include <unordered_map>
#include "heap.h"
#include "coding.h"
#include <fstream>
#include "BitWriter.h"
#include <vector>

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

bool myfunc(blah a, blah b){
    return a.car < b.car;
}

void inorder(unordered_map<char, coding> &map,Node* t,int code,int numBits){
    if(t == NULL){
        return;
    }
    int leftcode = code << 1;
    int rightcode = code << 1 | 1;

    inorder(map,t->left, leftcode,numBits+1);
    if(t->left == NULL && t->right == NULL){
        map[t->c] = {numBits, code};
        blah b = {{numBits,code},t->c};
        vect.push_back(b);
    } else if (t->left == NULL || t->right == NULL){
        cerr << "ERROR - Node in encoding tree has only child" << endl;
    }
    inorder(map,t->right,rightcode,numBits+1);
    
}


#ifndef DEBUG
int main(int argc, char** argv){
    unordered_map<char,int> map;
    string s;
    ifstream inf;
    if(argc != 3){
        cout << "Proper usage: ./huff  INPUT_FILE  ENCODED_FILE_NAME" << endl;
        return 0;
    }
    inf.open(argv[1],ios::in);
    if(!inf.good()){
        cout << "Could not open file "<< argv[1] << endl;
        return 0;
    }

    map['\n'] = 0;
    // Read in all of the input and get the frequency of each character
    while(getline(inf,s)){
        // cout << s << endl;
        for(int i = 0; i < s.size(); i++){
            if(map.find(s[i]) != map.end()){
                map[s[i]]++;
            } else {
                map[s[i]]= 1;
            }
        }
        map['\n']++;
    }

    // Build a heap out of the characters/frequencies
    Heap h(map.size());
    for(auto it : map){
        h.insert(it.first,it.second);
    }

    // Build the encoding tree by taking the 2 minimal nodes and making them the children of a new node, which gets inserted back into the heap
    while(h.getSize() != 1){
        Node* left = h.extractMin();
        Node* right = h.extractMin();
        Node* newNode = new Node;
        newNode->left = left;
        newNode->right = right;
        int sum = 0;
        if(left){
            sum += left->count;
        }
        if(right){
            sum += right->count;
        }
        newNode->count = sum;
        newNode->c = '\0';
        h.insert(newNode);
    }

    //Get the root of the encoding tree
    Node* root = h.extractMin();
    unordered_map<char,coding> codeMap;
    inorder(codeMap,root,0,0);
    sort(vect.begin(),vect.end(),myfunc);
    cout << "Char\tCode\t# Bits"<<endl;
    for(auto b : vect){
        cout << setw(8) << int(b.car) << setw(8) << b.c.code << setw(8) << b.c.numBits << endl;
    }
    BitWriter b(argv[2]);
    if(!b.open()){
        cout << "Could not open output file "<< argv[2] << endl;
        return 0;
    }
    b.writeTree(root);
    b.flush();
    inf.clear();
    inf.seekg(0,ios::beg);
    while(getline(inf,s)){
        // cout << s << endl;
        for(int i = 0; i < s.size(); i++){
            b.insert(codeMap,s[i]);
        }
        b.insert(codeMap,'\n');
    }
    b.close();
    inf.close();
    // cout << "Char\tCode\t# Bits" << endl;
    // for(auto code : codeMap){
    //     cout << code.first << "\t" << code.second.code << "\t" << code.second.numBits << endl;
    // }



    return 0;
}
#endif