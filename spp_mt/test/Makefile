


all:test

test:
	g++ -g -o test test.cpp -I../ -L../ -Wl,-Bstatic -lmt -Wl,-Bdynamic -ldl

clean:
	rm ./test
