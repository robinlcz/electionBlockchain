all: crabe.o rsa.o main.o mainTestK.o keys.o sign.o protected.o cellkey.o protectedcell.o hash.o
	gcc -o main main.o rsa.o crabe.o keys.o sign.o protected.o cellkey.o protectedcell.o hash.o -lm

mainwin: mainwin.o rsa.o crabe.o keys.o sign.o protected.o cellkey.o protectedcell.o hash.o
	gcc -o mainwin mainwin.o rsa.o crabe.o keys.o sign.o protected.o cellkey.o protectedcell.o hash.o -lm
	
hash.o: hash.c
	gcc -c hash.c

protectedcell.o: protectedcell.c
	gcc -c protectedcell.c

cellkey.o: cellkey.c
	gcc -c cellkey.c

sign.o: sign.c
	gcc -c sign.c

protected.o: protected.c
	gcc -c protected.c

keys.o: keys.c header/keys.h
	gcc -c keys.c

crabe.o: header/crabe.h crabe.c
	gcc -c crabe.c

rsa.o: header/crabe.h rsa.c
	gcc -c rsa.c

main.o: header/rsa.h
	gcc -c main.c

clean:
	rm -f *.o
	rm -f main
	rm -f *.txt