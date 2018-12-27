# препроцессор --> компиляция --> компоновка (или линковка).

ifneq ($V,1)
Q ?= @
endif

#DEBUG	= -g -O0
DEBUG	= -O3
CC	= gcc -E
INCLUDE	= -I/usr/local/include -I./
CFLAGS	= $(DEBUG) -Wall $(INCLUDE) -Winline -pipe

LDFLAGS	= -L/usr/local/lib
LDLIBS    = -lwiringPi -lwiringPiDev -lpthread -lm -lcrypt -lrt

# Should not alter anything below this line
###############################################################################

SRC	=	isr.c
OBJ	=	$(SRC:.c=.o)

BINS	=	$(SRC:.c=)

all:	isr

#	$Q cat README.TXT
#	$Q echo "    $(BINS)" | fmt
#	$Q echo ""

isr:	isr.o
	$Q echo [link] $< $@
	$Q $(CC) -o $@ isr.o $(LDFLAGS) $(LDLIBS)

c.o:
	$Q echo [CC] $<
	$Q $(CC) -c $(CFLAGS) $< -o $@

clean:
	$Q echo "[Clean]"
	$Q rm -f $(OBJ) *~ core tags $(BINS)

tags:	$(SRC)
	$Q echo [ctags]
	$Q ctags $(SRC)

depend:
	makedepend -Y $(SRC)


# DO NOT DELETE
