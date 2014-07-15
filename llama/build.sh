#!/usr/bin/env sh

gcc -Wall -Werror -pedantic -g -lm -I. -Ilib -O2 $@ llama.c lib/lv_pnm.o -o llama
gcc -Wall -Werror -pedantic -g -lm -I. -Ilib -O2 $@ frames_llama.c lib/lv_pnm.o -o frames_llama
