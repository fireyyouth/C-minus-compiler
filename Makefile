tree.jpg: tree.dot
	dot -T jpg tree.dot -o tree.jpg
tree.dot: test
test: test.cc parser.o
	g++ test.cc parser.o -o test
parser.o: parser.cc parser.h
	g++ -c parser.cc

