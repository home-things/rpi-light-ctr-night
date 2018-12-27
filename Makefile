# gcc
# препроцессор --> компиляция --> ассемблирование --> компоновка (aka линковка).

# make
# $@: the target filename.
# $*: the target filename without the file extension.
# $<: the first prerequisite filename.
# $^: the filenames of all the prerequisites, separated by spaces, discard duplicates.
# $+: similar to $^, but includes duplicates.
# $?: the names of all prerequisites that are newer than the target, separated by spaces.

.PHONY : all clean

ifneq ($V,1)
Q ?= @
endif

#DEBUG	= -ggdb -O0 -v
DEBUG	= -O3
CC	= gcc
INCLUDE	= -I/usr/local/include -I.
CFLAGS	= -c -std=c99 $(DEBUG) -Wall $(INCLUDE) -Winline -pipe

LDFLAGS	= -L/usr/local/lib
LDLIBS    = -lwiringPi -lwiringPiDev -lpthread -lm -lcrypt -lrt

# Should not alter anything below this line
###############################################################################

SRC  := $(wildcard *.c)
OBJS := $(SRC:.c=.o)
BINS :=	$(SRC:.c=)

all:	isr

#	$Q cat README.TXT
#	$Q echo "    $(BINS)" | fmt
#	$Q echo ""

isr:	$(OBJS)
	$Q echo [link] $^ '-->' $@
	$Q $(CC) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@

%.o:    %.c
	$Q echo [CC] $< '-->' $@
	$Q $(CC) -I. -c $(CFLAGS) $< #-o $@
			
clean:
	$Q echo "[Clean]"
	$Q rm -f $(OBJS) *~ core tags $(BINS)

#tags:	$(SRC)
#	$Q echo [ctags]
#	$Q ctags $(SRC)

#depend:
#	makedepend -Y $(SRC)
