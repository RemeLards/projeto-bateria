##
#  @file Makefile
#



default: run

CFLAGS=-g
APP=Wave2C
SRCS=Wave2C.c
OBJS=$(SRCS:.c=.o)
TSTS=Bass-Drum-1.wav  Bass-Drum-2.wav  Bass-Drum-3.wav  Claves.wav
TSTOUT:=$(TSTS:.wav=.c)
APPFLAGS=-v -c -s


$(APP): $(OBJS)
	$(CC) -o $@ $(CFLAGS)  $^


run: $(APP)
	echo "Testing"
	for f in $(TSTS) ; do  echo "$$f";  ./$(APP) $(APPFLAGS) "$$f"; done

clean:
	rm -rf $(APP) $(OBJS)  $(TSTOUT) $(subst -,_,$(TSTOUT))

