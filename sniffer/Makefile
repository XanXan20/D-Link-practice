CC = gcc
CFLAGS = -W -Wall -Werror -Wpedantic -std=c99

sniffer: main.o ether.o ip.o tcp.o udp.o icmp.o dns.o http.o
	$(CC) $(CFLAGS) -o sniffer main.o ether.o ip.o tcp.o udp.o icmp.o dns.o http.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

ether.o: include/ether.c
	$(CC) $(CFLAGS) -c include/ether.c

ip.o: include/ip.c
	$(CC) $(CFLAGS) -c include/ip.c

tcp.o: include/tcp.c
	$(CC) $(CFLAGS) -c include/tcp.c

udp.o: include/udp.c
	$(CC) $(CFLAGS) -c include/udp.c

icmp.o: include/icmp.c
	$(CC) $(CFLAGS) -c include/icmp.c

dns.o: include/dns.c
	$(CC) $(CFLAGS) -c include/dns.c

http.o: include/http.c
	$(CC) $(CFLAGS) -c include/http.c

clean:
	rm -f *.o sniffer