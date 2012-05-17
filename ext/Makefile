MRUBY_ROOT = ../../../src/mruby

INCLUDES = -I/usr/local/include -I$(MRUBY_ROOT)/include -I$(MRUBY_ROOT)/src -I.
CFLAGS = $(INCLUDES) -O3 -g -Wall -Werror-implicit-function-declaration

CC = gcc
LL = gcc
AR = ar

all : libmrtcc.a

mrb_uv.o : mrtcc.c
	gcc -c $(CFLAGS) mrtcc.c

libmrtcc.a : mrtcc.o
	$(AR) r libmrtcc.a mrtcc.o

clean :
	rm -f *.o libmrtcc.a