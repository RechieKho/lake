CC:=clang
CFLAG:=-Wall
OBJ_DIR:=@obj
BIN_DIR:=@bin
BIN_NAME:=lake

srcs:=$(wildcard *.c)
objs:=$(srcs:%.c=$(OBJ_DIR)/%.o)
bin:=$(BIN_DIR)/$(BIN_NAME)

default: debug

.PHONY: \
	default \
	clean \
	debug \
	release

clean:
	rm -f $(objs) $(bin)

debug: CFLAG+=-O0 -g -Wextra -DDEBUG
debug: $(bin)

release: CFLAG+=-O2 -DRELEASE
release: $(bin)

$(bin): $(objs)
	$(CC) $(CFLAG) $^ -o $@

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAG) $^ -c -o $@
