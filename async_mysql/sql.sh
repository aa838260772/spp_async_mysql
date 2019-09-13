#!bin/bash

rm  tmp.txt
sql="insert into innodb_lock(a,b,extra) values" 
for i in {100000..150000}
do
	sql+="(${i},'$((i+1))',$((i+1))),"
done
sql+=";"
echo ${sql} >> tmp.txt
