CXX=g++
FILES=huffman.cpp lzw.cpp main.cpp

prog: $(FILES)
	$(CXX) $^ -o prog

clean:
	rm -rf prog