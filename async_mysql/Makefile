
MYSQL_INC=-I ../mysql/include 
MYSQL_LIB=-Wl,-Bstatic -L ../mysql/lib/ -lmysqlclient -Wl,-Bdynamic -lm -lpthread -lrt -ldl

MT_INC=-I ../spp_mt/
MT_LIB=-L../spp_mt/ -Wl,-Bstatic -lmt -Wl,-Bdynamic -ldl
MT_DYN_LIB=-Wl,-rpath=/home/syswj/Documents/coro_mysql/spp_mt -L../spp_mt/ -Wl,-Bdynamic -lmt -lm -ldl

all:test

test:
	g++ -g -o test test.cpp $(MYSQL_INC) $(MYSQL_LIB) $(MT_INC) $(MT_DYN_LIB)

clean:
	rm ./test
