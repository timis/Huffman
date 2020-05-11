#include "BitWriter.h"

/**
 * @brief Construct a new BitWriter object
 * 
 * @param outFile The name of the encoded output file
 */
BitWriter::BitWriter(string outFile){
    fileName = outFile;
}

/**
 * @brief Opens the encoded output file and returns whether it actually opened
 * 
 * @return true 
 * @return false 
 */
bool BitWriter::open(){
    f.open(fileName, ios::out | ios::binary | ios::trunc);
    return f.good();
}

/**
 * @brief Pushes an remaining bits and then closes the file.
 * 
 */
void BitWriter::close(){
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
}

/**
 * @brief Writes a single character/byte to the output file in its encoded form.
 * 
 * First, the byte is sent through the map to find the coding. This coding is then added to a queue. There is a running tally of the number of bits
 * that are located in a queue. Once this tally is above 8 (8 bits per byte, which is the minimum we can add at once), we write 8 bits to the file.
 * This is done by building up a byte using bit shifts and bitwise OR-ing things together. Once a full byte is made, it is then inserted into the file.
 * 
 * @param codeMap 
 * @param c 
 */
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

/**
 * @brief Writes the huffman tree map from the beginning of the input file
 * 
 * The tree is encoded in the following scheme:
 *  If there is a leaf node, write a "1" bit and then the 8 bits representing the ASCII character of this leaf node
 *  If there is an internal node, write a "0" bit and recurse along the left and right subtrees
 * 
 * @param root The root of the encoding tree
 */
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

/**
 * @brief Writes a single bit at a time to the file
 * 
 * Since a byte is the smallest unit that can be written to the file, there is a class member character variable and a running tally of bitsUsed.
 * Shift the current class character to the left by one bit and concatenate/OR the incoming bit to the least significant bit. Add one to the running tally.
 * Once there are 8 bits written, push that character to the file (and reset the running tally)
 * 
 * @param bit 
 */
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

/**
 * @brief Writes enough bits to the file to finish out the byte so the actual encoding can start from a byte boundary
 * 
 * Could be refactored out with a slight edit to the writeTree architecture
 */
void BitWriter::flush(){
    for(int i = writeBits; i < 8; i++){
        writeBit(0);
    }
}
