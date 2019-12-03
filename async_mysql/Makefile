
MYSQL_INC=-I ../mysql/mysql-connector-c-6.1.11-src/include  -I ../mysql/mysql-connector-c-6.1.11-src/build/include
MYSQL_LIB=-Wl,-Bstatic -L ../mysql/mysql-connector-c-6.1.11-src/build/libmysql/ -lmysqlclient -Wl,-Bdynamic -lm -lpthread -lrt -ldl

MT_INC=-I ../spp_mt/
MT_LIB=-L../spp_mt/ -Wl,-Bstatic -lmt -Wl,-Bdynamic -ldl
MT_DYN_LIB=-Wl,-rpath=/home/syswj/Documents/coro_mysql/spp_mt -L../spp_mt/ -Wl,-Bdynamic -lmt -lm -ldl
MYLIB_TEST = -L./ -ltest

all:lib test

lib:
	g++ $(MYSQL_INC) $(MT_INC) -c test.cpp
	ar crs libtest.a test.o

test:
	g++ -g -o main main.cpp $(MYLIB_TEST) $(MYSQL_INC) $(MYSQL_LIB) $(MT_INC) $(MT_LIB)

clean:
	rm ./main ./libtest.a ./*.o
