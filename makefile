#gcc -c connect.c -o conn -g -O1
#conn: connect.o
#	gcc -o conn connect.o
#connect.o: connect.c
#	gcc -c connect.c
serCom: serCom.o
	gcc -o serCom serCom.o
serCom.o: serCom.c
	gcc -c serCom.c
clean:
	rm -rf *.o
