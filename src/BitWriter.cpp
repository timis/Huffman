#include "BitWriter.h"

BitWriter::BitWriter(string outFile){
    fileName = outFile;
}

bool BitWriter::open(){
    if(isOpen()){
        return false;
    } else {
        f.open(fileName, ios::out | ios::binary | ios::trunc);
        // cout << "Opened file "<< fileName << " for encoded output" << endl;
        return f.good();
    }
}

bool BitWriter::isOpen(){
    return f.is_open();
}

void BitWriter::close(){
    if(!isOpen()){
        return;
    }
    // cout << "Closing file for encoded output "<< endl;
    if(numBits> 0){
        int bitsAdded = 0;
        char toPush = '\0';
        while(vec.size() != 0){
            coding front = vec[0];
            vec.pop_front();
            toPush <<= front.numBits;
            toPush |= (front.code & numbitsToMask[front.numBits]);
            bitsAdded += front.numBits;
        }
        if(bitsAdded >= 8){
            cerr << "ERROR - BitWriter::close - More than/equal to 8 bits leftover should not be possible" << endl;
        }
        toPush <<= (8-bitsAdded);
        toPush &= (numbitsToMask[bitsAdded]<<(8-bitsAdded));
        f.put(toPush);
    }
    f.close();

    //If there are remaining bits in the vector, write them to the file
    //then just close
}


void BitWriter::insert(unordered_map<char,coding> &codeMap, char c){
    coding code = codeMap[c];
    numBits += code.numBits;
    vec.push_back(code);
    // cout << "Numbits: "<<numBits << endl;
    while(numBits >= 8){
        char toPush = '\0';
        int toGo = 8;
        while(toGo != 0){
            coding front = vec[0];
            if(front.numBits <= toGo){
                toPush <<= front.numBits;
                toPush |= (front.code & numbitsToMask[front.numBits]);
                toGo -= front.numBits;
                vec.pop_front();
            } else {
                toPush <<= toGo;
                //4 bits in encoding, 3 to go -> take the high 3 bits
                //keep the remaining 1 bit in the deque; set the front of the deque's numbits to 1
                int bitsToChange = front.numBits - toGo;
                toPush |= ((front.code>>bitsToChange) & numbitsToMask[toGo]);
                vec[0].numBits = bitsToChange;
                vec[0].code &= numbitsToMask[bitsToChange];
                toGo = 0;
            }
        }
        numBits -= 8;
        // cout << "Pushing character: " << int(toPush) << endl;
        f.put(toPush);
    }
}

void BitWriter::writeTree(Node* root){
    if(root == NULL){
        return;
    }
    if(root->left == NULL && root->right == NULL){
        writeBit(1);
        char c = root->c;
        for(int i = 7; i >= 0; i--){
            writeBit((c>>i)&1);
        }
    } else if (root->left == NULL || root->right == NULL){
        cerr << "ERROR - BitWriter::writeTree - only one child is NULL; should not be possible" << endl;
    } else {
        writeBit(0);
        writeTree(root->left);
        writeTree(root->right);
    }
}

void BitWriter::writeBit(int bit){

    treeChar <<= 1;
    treeChar |= (bit&1);
    writeBits++;    
    if(writeBits> 8){
        cerr << "ERROR - BitWriter::writeBit - Somehow writeBits is above 8"<<endl;
    }
    if(writeBits == 8){
        f.put(treeChar);
        writeBits = 0;
    }
}

void BitWriter::flush(){
    for(int i = writeBits; i < 8; i++){
        writeBit(0);
    }
}
