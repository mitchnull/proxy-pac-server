CFLAGS=-O2 -Wall -Wextra -s
#CFLAGS=-Wall -g -Wextra
.PHONY: all clean
all: proxy-pac-server
clean:
	rm -f proxy-pac-server

proxy-pac-server: -lmicrohttpd

