CFLAGS=-O2 -s
proxy-pac-server: -lmicrohttpd

.PHONY: clean
clean:
	rm -f proxy-pac-server
