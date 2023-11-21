CC=clang
CFLAGS=-pthread -g -Wall -Werror -Wno-unused-variable

OBJS=       chat.o \
						util.o \
						# line eater

all: chat

chat: $(OBJS)
	$(CC) -pthread -o $@ $^

clean:
	rm -f *.o chat
