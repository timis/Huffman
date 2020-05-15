#include "Huffman.h"
#include "Heap.h"

bool codeCharComparison(coding a, coding b){
    return a.character < b.character;
}

/**
 * @brief Encodes and saves the input file into an encoded file
 * 
 * @param inputFile The file containing the info to encode
 * @param encodedFile The file name for the encoded data
 * @param mapFile If this is not the empty string, will also save the map in its own file (for debugging purposes)
 */
void Huffman::encode(string inputFile, string encodedFile,string mapFile) {
    unordered_map<char,int> map;
    string s;
    ifstream inf;
    inf.open(inputFile,ios::in);
    if(!inf.good()){
        cout << "Could not open file "<< inputFile << endl;
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
    buildMapInOrder(codeMap,root,0,0);
    sort(vect.begin(),vect.end(),codeCharComparison);
    cout << "Char\tCode\t# Bits"<<endl;
    for(auto b : vect){
        cout << setw(8) << int(b.character) << setw(8) << b.code << setw(8) << b.numBits << endl;
    }
    BitWriter b(encodedFile);
    if(!b.open()){
        cout << "Could not open output file "<< encodedFile << endl;
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
}

/**
 * @brief Decodes an encoded input and saves it into a file
 * 
 * @param encodedFile The file containing the encoded data
 * @param outputFile The filename of the output file to hold the decoded data
 * @param mapFile If this is not the empty string, this map will be used to decode data instead of the one held in the encoded data file.
 */
void Huffman::decode(string encodedFile, string outputFile, string mapFile){
    ofstream out;
    BitReader b(encodedFile);
    if(!b.open()){
        cout << "Could not open encoded input file "<< encodedFile << endl;
    }
    Node* root = b.readTree();
    b.setRoot(root);
    b.prep();
    inorderTreeTraversal(root,0,0);
    sort(vect.begin(),vect.end(),codeCharComparison);
    cout << "Char\tCode\t# Bits"<<endl;
    for(auto b : vect){
        cout << setw(8) << int(b.character) << setw(8) << b.code << setw(8) << b.numBits << endl;
    }

    out.open(outputFile, ios::out);
    if(!out.good()){
        cout << "Could not open output file "<< outputFile << endl;
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


}

/**
 * @brief Given a huffman encoding tree, builds a map between characters and codes
 * 
 * @param map Passed by reference (so shared between all recursive calls)
 * @param t The current node we are looking at
 * @param code The current code - code is built recursively by shifting and (possibly) adding one (ie, 0 becomes -> 00 and 01, which become 000,001,010,011 etc.)
 * @param numBits The current number of bits (just increases by one with each recursive level)
 */
void Huffman::buildMapInOrder(unordered_map<char, coding> &map,Node* t,int code,int numBits){
    if(t == NULL){
        return;
    }
    int leftcode = code << 1;
    int rightcode = code << 1 | 1;

    buildMapInOrder(map,t->left, leftcode,numBits+1);
    if(t->left == NULL && t->right == NULL){
        map[t->c] = {numBits, code,t->c};
        vect.push_back({numBits, code,t->c});
    } else if (t->left == NULL || t->right == NULL){
        cerr << "ERROR - Node in encoding tree has only child" << endl;
    }
    buildMapInOrder(map,t->right,rightcode,numBits+1);
    
}

/**
 * @brief Does an inorder traversal of the encoding tree in order to build up a single vector of codings (used mostly as a debugging tool)
 * 
 * @param t The current node in the tree
 * @param code The current code - code is built recursively by shifting and (possibly) adding one (ie, 0 becomes -> 00 and 01, which become 000,001,010,011 etc.)
 * @param numBits The current number of bits (just increases by one with each recursive level)
 */
void Huffman::inorderTreeTraversal(Node* t,int code,int numBits){
    if(t == NULL){
        return;
    }
    int leftcode = code << 1;
    int rightcode = code << 1 | 1;

    inorderTreeTraversal(t->left, leftcode,numBits+1);
    if(t->left == nullptr || t->right == nullptr){
        vect.push_back({numBits,code,t->c});
    }
    inorderTreeTraversal(t->right,rightcode,numBits+1);   
}

