CC=cc     
CFLAGS=
CLIB= -lm


#
# Nothing to change below this line
#


all: hetroPlot hetroEdit hetroMix hetroReduce


hetroPlot: hetroPlot.c allocate.h sub_iodata.o sub_2Dgraphics.o
	$(CC) $(CFLAGS) -o hetroPlot hetroPlot.c \
                           sub_iodata.o sub_2Dgraphics.o $(CLIB)

hetroEdit: hetroEdit.c allocate.h sub_iodata.o
	$(CC) $(CFLAGS) -o hetroEdit hetroEdit.c sub_iodata.o $(CLIB)


hetroMix: hetroMix.c allocate.h sub_iodata.o 
	$(CC) $(CFLAGS) -o hetroMix hetroMix.c sub_iodata.o $(CLIB)


hetroReduce: hetroReduce.c allocate.h sub_iodata.o sub_math.o 
	$(CC) $(CFLAGS) -o hetroReduce hetroReduce.c  \
                           sub_iodata.o sub_math.o $(CLIB)




sub_iodata.o: sub_iodata.c allocate.h
	$(CC) $(CFLAGS) -c sub_iodata.c  -o sub_iodata.o

sub_2Dgraphics.o: sub_2Dgraphics.c allocate.h
	$(CC) $(CFLAGS) -c sub_2Dgraphics.c  -o sub_2Dgraphics.o

sub_math.o: sub_math.c
	$(CC) $(CFLAGS) -c sub_math.c  -o sub_math.o  

