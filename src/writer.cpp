#include <iostream>
#include "Huffman.h"

using namespace std;

int main(int argc, char** argv){
    Huffman h;

    if(argc != 3){
        cout << "Proper usage: ./huff  INPUT_FILE  ENCODED_FILE_NAME" << endl;
        return 0;
    }
    h.encode(argv[1],argv[2]);


    return 0;
}
