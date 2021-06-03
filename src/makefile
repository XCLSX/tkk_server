CC=gcc
INCLUDE=-I../include/
CFLAGS=-c -g -Wall $(INCLUDE)
CPPFLAGS=
LDFLAGS=-lpthread
TARGET=server
SRCFILE=$(wildcard *.c)
DESFILE=$(patsubst %.c,%.o,$(SRCFILE))
RM=-rm -rf

$(TARGET):$(DESFILE)
	$(CC) $^ $(LDFLAGS) -o $@ -lmysqlclient
%.o:%.c
	$(CC) $(CFLAGS) $<
output:
	@echo "$(SRCFILE)"
	@echo "$(DESFILE)"


clean:
	$(RM) $(DESFILE)
	-rm server

