all:
	gcc connect.c -shared -rdynamic -fPIC -g -O0 -o connect.so
