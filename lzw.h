#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
using namespace std;

void Init(unordered_map<string, long long> &mp);
void Compress(string input, unordered_map<string, long long> &mp, struct flag flags, string filename);
void Decompress(vector<long long> code, unordered_map<string, long long> &mp, struct flag flags, string filename);
string FindByValue(unordered_map<string, long long> &mp, long long elem);
void LZWCompress(vector<string> filenames, struct flag flags);
void LZWDecompress(vector<string> filenames, struct flag flags);