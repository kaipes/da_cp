#include "lzw.h"
#include "huffman.h"

int main(int argc, char *argv[]) {
    vector<string> filenames;
    struct flag flags = {0};
    Input(&flags, filenames, argc, argv);
    if (flags.r) {
        filesystem::recursive_directory_iterator dir(filenames[0]);
        filesystem::recursive_directory_iterator end;
        while (dir != end) {
            if (filesystem::is_regular_file(dir->path())) {
                filenames.push_back(dir->path());
            }
            dir++;
        }
        filenames.erase(filenames.begin());
    }
    if (!flags.d && !flags.max) {
        HuffmanCompress(filenames, flags);
    } else if (flags.d && !flags.max) {
        HuffmanDecompress(filenames, flags);
    }
    if (flags.max) {
        if (flags.d) {
            LZWDecompress(filenames, flags);
        } else {
            LZWCompress(filenames, flags);
        }
    }
    if (flags.t) {
        CheckKeyFile(filenames);
    }
    if (!flags.k) {
        for (int i = 0; i < filenames.size(); i++) {
            uintmax_t n = filesystem::remove(filenames[i]);
        }
    }
    if (flags.l) {
        HuffmanCompress(filenames, flags);
        Stats(filenames, flags);
    }
}
