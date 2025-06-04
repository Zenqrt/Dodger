all:
	g++ -I src/include -L src/lib -o dodger *.cpp -lmingw32 -lSDL3