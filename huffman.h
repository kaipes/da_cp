#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <fstream>
#include <algorithm>
#include <filesystem>
using namespace std;

struct flag {
    bool c;
    bool d;
    bool k;
    bool t;
    bool f;
    bool l;
    bool r;
    bool min;
    bool max;
};

class HuffmanNode {
	public:
        long long frequency;
	    char symbol;
	    HuffmanNode *left, *right;
	    HuffmanNode(){
            left = NULL;
            right = NULL;
        }
        HuffmanNode(long long freq, char symb) {
            frequency = freq;
            symbol = symb;
        }
        HuffmanNode(HuffmanNode* first, HuffmanNode* second) {
            left = first;
            right = second;
            frequency = first->frequency + second->frequency;
        }
};


struct Comparator{
    bool operator()(const HuffmanNode* l, const HuffmanNode* r) const { 
        return l->frequency < r->frequency; 
    }
};

void Input(struct flag* flags, vector<string> &filenames, int argc, char* argv[]);
HuffmanNode* PopUpdate(vector<HuffmanNode*> &huffArray);
void CodeDisplay(HuffmanNode* root);
void HuffmanCompress(vector<string> filenames, struct flag flags);
void HuffmanDecompress(vector<string> filenames, struct flag flags);
void CheckKeyFile(vector<string> filenames);
void Stats(vector<string> filenames, struct flag flags);