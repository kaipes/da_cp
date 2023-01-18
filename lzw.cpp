#include "lzw.h"
#include "huffman.h"

void Init(unordered_map<string, long long> &mp) {
    char tmp = 'a';
    int i = 0;
    while (tmp <= 'z') {
        string s(1, tmp);
        mp.insert({s, i});
        tmp++;
        i++;
    }
    mp.insert({"\0", 26});
}

void Compress(string input, unordered_map<string, long long> &mp, struct flag flags, string filename) {
    int i = 0;
    bool flag = 1;
    ofstream out;
    if (!flags.c) {
        out.open(filename+".lzw");
    }
    while (i != input.size()) {
        string word(1, input[i]);
        while (mp.find(word) != mp.end() && i < input.size()) {
            i++;
            word+=input[i];
        }
        if (flag) {
            if (!flags.c) {
                out << mp.find(word.substr(0, word.size()-1))->second;
            } else {
                cout << mp.find(word.substr(0, word.size()-1))->second;
            }
            flag = 0;
        } else {
            if (!flags.c) {
                out << " " << mp.find(word.substr(0, word.size()-1))->second;
            } else {
                cout << " " << mp.find(word.substr(0, word.size()-1))->second;
            }
        }
        mp.insert({word, mp.size()});
    }
    if (!flags.c) {
        out << " " << mp.find("\0")->second;
    } else {
        cout << " " << mp.find("\0")->second;
    }
    if (flags.c) {
        out.close();
    }
}

string FindByValue(unordered_map<string, long long> &mp, long long elem) {
    string res;
    for (unordered_map<string, long long>::iterator it = mp.begin(); it != mp.end(); ++it) {
        if (it->second == elem) {
            res = it->first;
            break;
        }
    } 
    return res;
}

void Decompress(vector<long long> code, unordered_map<string, long long> &mp, struct flag flags, string filename) {
    string res;
    ofstream out;
    if (!flags.c) {
        out.open(filename+".unlzw");
    }
    for (int i = 0; i < code.size(); i++) {
        res = FindByValue(mp, code[i]);
        if (!res.empty()) {
            if (!flags.c) {
                out << res;
            } else {
                cout << res;
            }
        } else {
        	if (i > 0) {
                string pres = FindByValue(mp,code[i-1]);
                mp.insert({pres+pres[0], mp.size()});
                if (!flags.c) {
                    out << FindByValue(mp, code[i]);
                } else {
                    cout << FindByValue(mp, code[i]);
                }
            }
        }
        if (i > 0) {
            string prev = FindByValue(mp, code[i-1]);
            string pres = FindByValue(mp,code[i]);
            mp.insert({prev + pres[0], mp.size()});
        }
    }
    if (!flags.c) {
        out.close();
    }
}

void LZWCompress(vector<string> filenames, struct flag flags) {
    for (int i = 0; i < filenames.size(); i++) {
        ifstream file;
        file.open(filenames[i]);
        string str;
        string s;
        while (getline(file, s)){
            str = str + s;
        }
        unordered_map<string, long long> mp;
        Init(mp);
        Compress(str, mp, flags, filenames[i]);
    }
}

void LZWDecompress(vector<string> filenames, struct flag flags) {
    for (int i = 0; i < filenames.size(); i++) {
        ifstream file;
        vector<long long> c;
        file.open(filenames[i]);
        long long number;
        while (file >> number) {
            c.push_back(number);
        }
        unordered_map<string, long long> mp;
        Init(mp);
        Decompress(c, mp, flags, filenames[i]);
    }
}
