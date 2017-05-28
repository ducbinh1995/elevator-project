all:
	rm -f *.o main liftMng liftCtr opePanel1 opePanelX 
	gcc -c commonfunction.c
	gcc -o main main.c commonfunction.o
	gcc -o liftMng liftMng.c commonfunction.o `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
	gcc -o liftCtr liftCtr.c commonfunction.o	
	gcc -o opePanel1 opePanel1.c commonfunction.o `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
	gcc -o opePanelX opePanelX.c commonfunction.o `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
main: main.c
	rm -f main
	gcc -o main main.c commonfunction.o
liftMng: liftMng.c
	rm -f liftMng
	gcc -o liftMng liftMng.c commonfunction.o `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
liftCtr: liftCtr.c
	rm -f liftCtr
	gcc -o liftCtr liftCtr.c commonfunction.o
opePanel1: opePanel1.c
	rm -f opePanel1
	gcc -o opePanel1 opePanel1.c commonfunction.o `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
opePanelX: opePanelX.c
	rm -f opePanelX
	gcc -o opePanelX opePanelX.c commonfunction.o `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
clean:
	rm -f *.o main liftMng liftCtr opePanel1 opePanelX 