#ifndef CODING_H
#define CODING_H
struct coding {
    int numBits;
    int code;
};

struct Node {
    char c;
    int count = 0;
    int index = 0;
    Node* left = nullptr;
    Node* right = nullptr;
};

#endif