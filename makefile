MCC_SRC = src/tokenizer.c src/symbol_table.c
CALC_SRC = src/tokenizer.c src/symbol_table.c src/calc.c
DIAL_SRC = src/tokenizer.c src/symbol_table.c src/dial.c 
DEBUG ?= -DDEBUG -g
CFLAGS = -Wno-implicit-function-declaration -I./include -I.
LDFLAGS = -lm
CC ?= gcc

mcc: $(MCC_SRC)
	$(CC) $(MCC_SRC) -I/usr/local/include/ $(LDFLAGS) $(DEBUG) $(CFLAGS) -o $@

calc: $(CALC_SRC)
	$(CC) $(CALC_SRC) -I/usr/local/include/ $(LDFLAGS) $(DEBUG) $(CFLAGS) -o $@

dial: $(DIAL_SRC)
	$(CC) $(DIAL_SRC) -I/usr/local/include/ $(LDFLAGS) $(DEBUG) $(CFLAGS) -o $@

.PHONY: all tools install clean
