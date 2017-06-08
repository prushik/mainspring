COMMON_SRC = src/tokenizer.c src/symbol_table.c
MCC_SRC = src/tokenizer.c src/symbol_table.c
CALC_SRC = src/tokenizer.c src/symbol_table.c src/tools/calc.c
DIAL_SRC = src/tokenizer.c src/symbol_table.c src/parser.c src/tools/dial.c 
DIAL2_SRC = src/tokenizer.c src/symbol_table.c src/parser.c src/tools/dial2.c
EDITOR_SRC = src/tokenizer.c src/symbol_table.c src/parser.c src/tools/editor.c 
DEBUG ?= -DDEBUG -g -Og
CFLAGS = -Wno-implicit-function-declaration -I./include -I.
LDFLAGS = -lm
CC ?= gcc
MCC ?= mainspring

mcc: $(MCC_SRC)
	$(CC) $^ -I/usr/local/include/ $(LDFLAGS) $(DEBUG) $(CFLAGS) -o $@

calc: $(CALC_SRC)
	$(CC) $^ -I/usr/local/include/ $(LDFLAGS) $(DEBUG) $(CFLAGS) -o $@

dial: $(DIAL_SRC)
	$(CC) $^ -I/usr/local/include/ $(LDFLAGS) $(DEBUG) $(CFLAGS) -o $@

dial2: $(DIAL2_SRC)
	$(CC) $^ -I/usr/local/include/ $(LDFLAGS) $(DEBUG) $(CFLAGS) -o $@

editor: $(EDITOR_SRC)
	$(CC) $^ -I/usr/local/include/ $(LDFLAGS) $(DEBUG) $(CFLAGS) -o $@

.PHONY: all tools install clean
