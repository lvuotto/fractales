
CC = gcc
inc_dir = -I.
CFLAGS = -Wall -Werror -pedantic -g $(inc_dir)
targets = lv_pnm.o lv_pnm_filters.o

all: $(targets)

lv_pnm.o: lv_pnm.c lv_pnm.h lv_pnm_types.h
	$(CC) $(CFLAGS) -c $<

lv_pnm_filters.o: lv_pnm_filters.c lv_pnm_filters.h
	$(CC) $(CFLAGS) -lm -c $<

clean:
	rm -f $(targets)
