CC = gcc
inc_dir = -I. -Ilib
CFLAGS = -Wall -Werror -pedantic -g -lm $(inc_dir) -O2
targets = llama frames_llama

all: $(targets)

llama: llama.c paleta.h variaciones.h lib/lv_pnm.o
	$(CC) $(CFLAGS) $< lib/lv_pnm.o -o $@

frames_llama: frames_llama.c paleta.h variaciones.h lib/lv_pnm.o
	$(CC) $(CFLAGS) $< lib/lv_pnm.o -o $@

clean:
	rm -f $(targets)
