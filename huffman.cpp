#include "huffman.h"

void Input(struct flag* flags, vector<string> &filenames, int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        string s(argv[i]);
        if (s == "-c") {
            flags->c = true;
        } else if (s == "-d") {
            flags->d = true;
        } else if (s == "-k") {
            flags->k = true;
        } else if (s == "-l") {
            flags->l = true;
        } else if (s == "-r") {
            flags->r = true;
        } else if (s == "-t") {
            flags->t = true;
        } else if (s == "-1") {
            flags->min = true;
        } else if (s == "-9") {
            flags->max = true;
        } else {
            filenames.push_back(s);
        }
    }
}

HuffmanNode* PopUpdate(vector<HuffmanNode*> &huffArray) {
    HuffmanNode* tmp = huffArray.front();
    huffArray.erase(huffArray.begin());
    return tmp;
}

map<char, vector<bool>> distrub;
vector<bool> code;
void CodeDisplay(HuffmanNode* root) {
    if (root->left != NULL) {
        code.push_back(0);
        CodeDisplay(root->left);
    }
    if (root->right != NULL) {
        code.push_back(1);
        CodeDisplay(root->right);
    }
    if (root->left == NULL && root->right == NULL) {
        distrub[root->symbol] = code;
    } 
    if (code.size() != 0) {
        code.pop_back();
    }    
}

void HuffmanCompress(vector<string> filenames, struct flag flags) {
    for (int i = 0; i < filenames.size(); i++) {
        code.clear();
        char c;
        vector<HuffmanNode*> huffArray;
        map<char, long long> cntSymbol;
        ifstream fileRead(filenames[i]);	
	    while (!fileRead.eof()) { 
            char c = fileRead.get(); 
            cntSymbol[c]++;
        }
        for (map<char, long long>::iterator it = cntSymbol.begin(); it != cntSymbol.end(); it++) {
            HuffmanNode *newNode = new HuffmanNode;
            newNode->frequency = it->second;
            newNode->symbol = it->first;
            huffArray.push_back(newNode);
        }
        while (huffArray.size() != 1) {
            sort(huffArray.begin(), huffArray.end(), Comparator());
            HuffmanNode* first = PopUpdate(huffArray);
            HuffmanNode* second = PopUpdate(huffArray);
            huffArray.push_back(new HuffmanNode(first, second));
        }
        HuffmanNode *root = huffArray.front();
        CodeDisplay(root);
        ofstream serial;
        serial.open(filenames[i]+".key");
        for (map<char, vector<bool>>::iterator it = distrub.begin(); it != distrub.end(); it++) {
            serial << it->first << " ";
            for (int i = 0; i < it->second.size(); i++) {
                serial << it->second[i];
            }
            serial << "\n";
        }
        serial.close();
        fileRead.clear(); 
        fileRead.seekg(0);
        ofstream out;
        if (!flags.c) {
            out.open(filenames[i]+".haf",  ios::binary|ios::ate);
        }
        int count=0; char buf=0;
        while (!fileRead.eof()) { 
            char c = fileRead.get();
	        vector<bool> x = distrub[c];
	        for (int i = 0; i < x.size(); i++) {
                buf = buf | x[i] << (7-count);   
	            count++;   
	            if (count==8) { 
                    count=0;
                    if (flags.c) {
                        cout << buf; 
                    } else {
                        out << buf;
                    }
                    buf=0;
                } 
            }
        }
        fileRead.close();
        if (!flags.c) {
            out.close(); 
        }
	}
}

void PrintTree(HuffmanNode *root, unsigned k = 0) {
    if (root != NULL) {
        PrintTree(root->left, k+3);
        for (unsigned i = 0; i < k; i++) {
            cout << " ";
        }
        if (root->symbol) cout << root->frequency << " ( " << root->symbol << ")" << endl;
        else cout << root->frequency << endl;
        PrintTree(root->right, k+3);
    }
}

void HuffmanDecompress(vector<string> filenames, struct flag flags) {
    for (int i = 0; i < filenames.size(); i++) {
        string name = filenames[i];
        name[name.size() - 1] = 'y';
        name[name.size() - 2] = 'e';
        name[name.size() - 3] = 'k';
        ifstream key(name);
        map<char, string> codes;
        char symb;
        string r;
        while (getline(key, r)){
            char q = r[0];
            r.erase(0,1);
            codes[q] = r;
        }
        HuffmanNode *root = new HuffmanNode();
        root->left = NULL;
        root->right = NULL;
        for (map<char, string>::iterator it = codes.begin(); it != codes.end(); it++) {
            HuffmanNode *cur = root;
            for (char c: it->second) {
                if (c == '0') {
                    if (cur->left == NULL) {
                        cur->left = new HuffmanNode();
                        cur = cur->left;
                    } else {
                        cur = cur->left;
                    }
                } else if (c == '1') {
                    if (cur->right == NULL) {
                        cur->right = new HuffmanNode();
                        cur = cur->right;
                    } else {
                        cur = cur->right;
                    }
                }
            }
            cur->symbol = it->first;
        }
    	ifstream fi(filenames[i]);
        ofstream of;
        if (!flags.c) {
            of.open(filenames[i]+"unpack");
        }
        HuffmanNode *tmp = root;
	    int cnt=0; 
        char byte; 
	    byte = fi.get();
	    while(!fi.eof()) {   
            bool bit = byte & (1 << (7 - cnt)); 
		    if (bit) {
                tmp = tmp->right;
            } else {
                tmp = tmp->left;
            }
		    if (tmp->left == NULL && tmp->right == NULL) {
                if (flags.c) {
                    cout << tmp->symbol; 
                } else {
                    of << tmp->symbol;
                }
                tmp = root;
            }  
		    cnt++;
		    if (cnt == 8) {
                cnt = 0; 
                byte = fi.get();
            }
        }
	    fi.close();
        if (!flags.c) {
            of.close();
        }
    }	
   
}
    
void CheckKeyFile(vector<string> filenames) {
    for (int i = 0; i < filenames.size(); i++) {
        int flag = 0;
        if (filenames[i].substr(filenames[i].length() - 4, filenames[i].length()) != ".key") {
            cout << "Check only filenames with .key" << endl;
            flag = 1;
        }
        ifstream file(filenames[i]);
        char symbol;
        string code;
        while (getline(file, code)) {
            symbol = code[0];
            code.erase(0,1);
            if ((!(symbol > -127 && symbol < 128)) && flag == 0) {
                file.close();
                cout << "Error symbol" << endl;
                flag = 1;
            }
            for (char c : code) {
                if ((!((c == '0') || (c == '1'))) && flag == 0) {
                    file.close();
                    cout << "Error code" << endl;
                    flag = 1;
                }
            }
        }
        if (!flag) {
            file.close();
            cout << "File is good" << endl;
        }
    }
}

void Stats(vector<string> filenames, struct flag flags) {
    for (int i = 0; i < filenames.size(); i++) {
        ifstream file;
        file.open(filenames[i], ios::binary|ios::ate);
        ifstream archive;
        archive.open(filenames[i] + ".haf", ios::binary|ios::ate);
        cout << filenames[i] << endl;
        cout << "uncompressed: " << file.tellg()  << endl; 
        cout << "compressed: " << archive.tellg() << endl;
        cout << "ratio: " << archive.tellg()*100/file.tellg() << "%" << endl;
        file.close();
        archive.close();
    }
}