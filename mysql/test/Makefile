


all:test

test:
	g++ -o test mysql-test.cpp -I ../include/ -Wl,-Bstatic -L ../lib/ -lmysqlclient -Wl,-Bdynamic -lm -lpthread -lrt -ldl
clean:
	rm ./test
