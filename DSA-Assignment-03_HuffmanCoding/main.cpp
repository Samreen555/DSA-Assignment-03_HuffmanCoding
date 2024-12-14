/*
* in this i'm immplementing the logic of huffman coding to compressing and decompressiing the string.
the logic is to:
 * 1. Build a frequency table of characters in the input string.
 * 2. Use a priority queue (min-heap) to construct a Huffman Tree based on the character frequencies.
 * 3. Generate Huffman codes for each character based on the tree structure, with shorter codes assigned to characters with higher frequencies.
 * 4. Encode the input string using the generated Huffman codes.
 * 5. Decode the encoded string by traversing the Huffman Tree.
 * 6. Verify the accuracy of the compression by comparing the decoded string with the original input.
 * 7. Analyze the compression ratio by comparing the size of the original string in bits to the compressed string size.
 */

#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>
#include <bitset>

using namespace std;

// Node structure for Huffman Tree
struct Node {
    char ch;
    int freq;
    Node* left;
    Node* right;

    Node(char character, int frequency) : ch(character), freq(frequency), left(nullptr), right(nullptr) {}
};

// Comparison operator for the priority queue
struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

// Function to build the frequency table
unordered_map<char, int> buildFrequencyTable(const string& input) {
    unordered_map<char, int> frequency;
    for (char ch : input) {
        if (ch == ' ') {
            frequency['-']++;
        }
        else {
            frequency[ch]++;
        }
    }
    return frequency;
}

// Function to build the Huffman Tree
Node* buildHuffmanTree(const unordered_map<char, int>& frequency) {
    priority_queue<Node*, vector<Node*>, Compare> minHeap;

    // Push all characters and their frequencies into the minHeap
    for (const auto& pair : frequency) {
        minHeap.push(new Node(pair.first, pair.second));
    }

    // Build the tree
    while (minHeap.size() > 1) {
        Node* left = minHeap.top(); minHeap.pop();
        Node* right = minHeap.top(); minHeap.pop();

        Node* merged = new Node('\0', left->freq + right->freq);
        merged->left = left;
        merged->right = right;
        minHeap.push(merged);
    }

    return minHeap.top();
}

// Function to generate Huffman Codes
void generateHuffmanCodes(Node* root, const string& code, unordered_map<char, string>& huffmanCodes) {
    if (!root) return;

    // If it's a leaf node
    if (!root->left && !root->right) {
        huffmanCodes[root->ch] = code;
    }

    generateHuffmanCodes(root->left, code + "0", huffmanCodes);
    generateHuffmanCodes(root->right, code + "1", huffmanCodes);
}

// Function to encode the input string
string encode(const string& input, const unordered_map<char, string>& huffmanCodes) {
    string encodedString;
    for (char ch : input) {
        if (ch == ' ') {
            encodedString += huffmanCodes.at('-');
        }
        else {
            encodedString += huffmanCodes.at(ch);
        }
    }
    return encodedString;
}

// Function to decode the encoded string
string decode(const string& encodedString, Node* root) {
    string decodedString;
    Node* current = root;

    for (char bit : encodedString) {
        current = (bit == '0') ? current->left : current->right;

        // If it's a leaf node
        if (!current->left && !current->right) {
            decodedString += (current->ch == '-') ? ' ' : current->ch;
            current = root;
        }
    }

    return decodedString;
}

int main() {
    string input;
    string testAnother;

    // Step 1: Repeat the process until the user chooses to exit
    do {
        // Input the string
        cout << "Enter a string: ";
        getline(cin, input);

        // Step 2: Build the frequency table
        unordered_map<char, int> frequency = buildFrequencyTable(input);

        // Display the frequency table
        cout << "\nFrequency Table:" << endl;
        cout << "Character | Frequency" << endl;
        cout << "-----------------------" << endl;
        for (const auto& pair : frequency) {
            cout << "    " << pair.first << "       | " << pair.second << endl;
        }

        // Step 3: Build the Huffman Tree
        Node* root = buildHuffmanTree(frequency);

        // Step 4: Generate Huffman Codes
        unordered_map<char, string> huffmanCodes;
        generateHuffmanCodes(root, "", huffmanCodes);

        // Display the Huffman Codes
        cout << "\nHuffman Codes:" << endl;
        cout << "Character | Frequency | Code" << endl;
        cout << "---------------------------------" << endl;
        for (const auto& pair : huffmanCodes) {
            cout << "    " << pair.first << "       | " << frequency[pair.first] << "        | " << pair.second << endl;
        }

        // Step 5: Encode the input string
        string encodedString = encode(input, huffmanCodes);
        cout << "\nOriginal String: " << input << endl;
        cout << "Encoded String: " << encodedString << endl;

        // Step 6: Decode the encoded string
        string decodedString = decode(encodedString, root);
        cout << "Decoded String: " << decodedString << endl;

        // Verify that the decoded string matches the original
        if (decodedString == input) {
            cout << "\nVerification Successful: Decoded string matches the original string." << endl;
        }
        else {
            cout << "\nVerification Failed: Decoded string does not match the original string." << endl;
        }

        // Step 7: Analyze compression
        size_t originalSize = input.size() * 8; // Original size in bits
        size_t compressedSize = encodedString.size(); // Compressed size in bits

        cout << "\nOriginal Size: " << originalSize << " bits" << endl;
        cout << "Compressed Size: " << compressedSize << " bits" << endl;
        cout << "Compression Ratio: " << ((double)originalSize / compressedSize) << endl;

        // Ask the user if they want to test another string
        cout << "\nWould you like to test another string? (y/n): ";
        getline(cin, testAnother);
        cin.ignore(); // To ignore the leftover newline character from the previous input

    } while (testAnother == "y" || testAnother == "Y");

    cout << "\nThank you for using the Huffman Encoding Program!" << endl;

    return 0;
}

