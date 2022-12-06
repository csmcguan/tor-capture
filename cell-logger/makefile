CC = gcc
SRC = src

CFILES = $(addprefix $(SRC)/, clogger.c handle.c network.c clpacket.c log.c)
OFILES = $(addprefix $(SRC)/, clogger.o handle.o network.o clpacket.o log.o)
IFLAGS = -I./include
CFLAGS = -g $(IFLAGS)

all: cell-logger

cell-logger: $(OFILES)
	$(CC) -o clogger $(OFILES)

$(SRC)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm $(SRC)/*.o
