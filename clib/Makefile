
SRC  := $(wildcard *.c) 
SRC += $(wildcard ./adp/*.c)

OBJ  := $(SRC:%.c=%.o)

TARGET := app.out
.PHONY : clean all

all: $(TARGET)


CFLAGS+= -Wall -g -I/usr/include/ -fno-aggressive-loop-optimizations -D_POSIX_C_SOURCE -D_GNU_SOURCE
LDFLAGS+= -Wl,-O3 -Wl,--hash-style=gnu -Wl,--as-needed -Wl,-rpath -Wl,/ -L/usr/lib/ -L/lib/ 
LDFLAGS+= -ldl -lpthread -lm -lz -lcrypto -lssl -lcurl -lrt -lstdc++


$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^  $(LDFLAGS) 

clean:
	@rm -f *.out $(OBJ) *.o