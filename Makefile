all: main mainwin maintree mainblock

main: crabe.o rsa.o main.o keys.o sign.o protected.o cellkey.o protectedcell.o hash.o
	gcc -g -o main main.o rsa.o crabe.o keys.o sign.o protected.o cellkey.o protectedcell.o hash.o -lm

mainwin: mainwin.o rsa.o crabe.o keys.o sign.o protected.o cellkey.o protectedcell.o hash.o
	gcc -g -o mainwin mainwin.o rsa.o crabe.o keys.o sign.o protected.o cellkey.o protectedcell.o hash.o -lm

mainblock: mainblock.o hash.o block.o rsa.o crabe.o keys.o sign.o protected.o cellkey.o protectedcell.o
	gcc -g -o mainblock mainblock.o rsa.o crabe.o keys.o sign.o protected.o cellkey.o protectedcell.o hash.o block.o -lm -lssl -lcrypto

maintree: maintree.o blockchain.o hash.o block.o rsa.o crabe.o keys.o sign.o protected.o cellkey.o protectedcell.o
	gcc -g -o maintree maintree.o blockchain.o rsa.o crabe.o keys.o sign.o protected.o cellkey.o protectedcell.o hash.o block.o -lm -lssl -lcrypto

blockchain.o: blockchain.c
	gcc -g -c blockchain.c -lcrypto -lssl

block.o: block.c
	gcc -g -c block.c -lcrypto -lssl 

hash.o: hash.c
	gcc -g -c hash.c

protectedcell.o: protectedcell.c
	gcc -g -c protectedcell.c

cellkey.o: cellkey.c
	gcc -g -c cellkey.c

sign.o: sign.c
	gcc -g -c sign.c

protected.o: protected.c
	gcc -g -c protected.c

keys.o: keys.c header/keys.h
	gcc -g -c keys.c

crabe.o: header/crabe.h crabe.c
	gcc -g -c crabe.c

rsa.o: header/crabe.h rsa.c
	gcc -g -c rsa.c

mainwin.o:
	gcc -g -c mainwin.c

main.o: header/rsa.h
	gcc -g -c main.c

clean:
	rm -f *.o
	rm -f main