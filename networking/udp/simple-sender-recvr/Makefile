GC=gcc
STD=c2x
FLAGS=-Wall -Wextra

build-sender:
	$(GC) -std=$(STD) $(FLAGS) src/sender.c -o bin/sender.out
	
build-reciever:
	$(GC) -std=$(STD) $(FLAGS) src/reciever.c -o bin/reciever.out

reciever:
	./bin/reciever.out

test:
	sudo tcpdump -i any udp port 9800 -A