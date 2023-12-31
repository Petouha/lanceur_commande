CC= gcc
INC_DIR= ./include
SRC_DIR= ./src
LIB_DIR= ./lib

FLAGS= -std=c18 -I$(INC_DIR) -Wall -Wpedantic -Wextra -Wconversion -Wwrite-strings -Werror -fstack-protector-all -fpie -pthread -D__USE_XOPEN -D_POSIX_C_SOURCE=200809L -O3
BIN= file daemon client

OBJ = $(BIN).o

vpath %.h  $(INC_DIR)
vpath %.c  $(SRC_DIR)

.PHONY = clean all

all: $(BIN)

%.o: %.c $(DEPS)
	$(CC) -c $(FLAGS) -o $@ $^

#main: $(OBJ)
#	$(CC) -o main $^ $(CFLAGS)

file: file.o
	$(CC) -o $@ $^ $(CFLAGS)

daemon: daemon.o file.o analyse.o
	$(CC) -o $@ $^ $(CFLAGS)

client: client.o file.o analyse.o
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o $(BIN) tube*

tubes :
	rm -f tube*