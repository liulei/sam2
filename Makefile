EXEC	=	sam2

OBJS	=	main.o allvars.o init.o aux.o io.o tree.o galaxy.o \
			cooling.o

INCL	=	Millennium.h allvars.h proto.h

CC		=	gcc

OPTIMIZE	=	-O2 -Wall -W

#OPTIMIZE	=	-Wall -W

LIBS	=	-lm

OPT		+=	-DSALPETER
#OPT			+=	-DSCALO

CFLAGS	=	$(OPTIMIZE) $(OPT)

$(EXEC):$(OBJS)
		$(CC) $(OPTIMIZE) $(OBJS) $(LIBS) -o $(EXEC)	

.PHONY	:	clean
clean:
		rm -f $(OBJS) $(EXEC)

