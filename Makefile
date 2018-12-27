# gcc
# -c: препроцессор --> компиляция --> ассемблирование; -lm: --> компоновка (aka линковка).

# make
# $@: the target filename.
# $*: the target filename without the file extension.
# $<: the first prerequisite filename.
# $^: the filenames of all the prerequisites, separated by spaces, discard duplicates.
# $+: similar to $^, but includes duplicates.
# $?: the names of all prerequisites that are newer than the target, separated by spaces.

.PHONY : all clean

#DEBUG	= -ggdb -O0 -v
DEBUG	  = -O3
CC	    = gcc
INCLUDE	= -I/usr/local/include -I.
CFLAGS	= -c -std=c99 $(DEBUG) -Wall $(INCLUDE) -Winline -pipe

LDFLAGS	= -L/usr/local/lib
LDLIBS  = -lwiringPi -lwiringPiDev -lpthread -lm -lcrypt -lrt

# Should not alter anything below this line
###############################################################################

SRC  := $(wildcard *.c)
OBJS := $(SRC:.c=.o)
BINS :=	$(SRC:.c=)

all:		run

#	@cat README.TXT
#	@echo "    $(BINS)" | fmt
#	@echo ""

run:		isr
	./isr

isr:		$(OBJS)
	@echo [link] $^ '-->' $@
	$(CC) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@

%.o:    %.c
	@echo [CC] $< '-->' $@
	$(CC) -I. -c $(CFLAGS) $< #-o $@

clean:
	@echo "[Clean]"
	rm -f $(OBJS) *~ core tags $(BINS)

#tags:	$(SRC)
#	echo [ctags]
#	ctags $(SRC)

#depend:
#	makedepend -Y $(SRC)
