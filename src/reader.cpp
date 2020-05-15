#include <iostream>
#include "Huffman.h"

using namespace std;

int main(int argc, char** argv){
    if(argc != 3){
        cout << "Proper usage: ./reader  ENCODED_INPUT_FILE_NAME  DECODED_FILE_NAME" << endl;
        return 0;
    }
    Huffman h;
    h.decode(argv[1],argv[2]);
    return 0;
}
