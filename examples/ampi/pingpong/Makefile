-include ../../common.mk
-include ../../include/conv-mach-opt.mak
OPTS=-O3 
MPIOPTS=-O3
CHARMC=../../../bin/ampicxx $(OPTS)
MPICC=mpicc $(MPIOPTS) # Should use 'cc' instead of 'mpicc' on Cray systems

#all : pingpong-2way.c
#	$(CHARMC) pingpong-2way.c -o pgm
#	$(MPICC) pingpong-2way.c -o pgm-mpi

all : pingpong-1way.c
	$(CHARMC) pingpong-1way.c -o pgm
#	$(MPICC) pingpong-1way.c -o pgm-mpi

pipe: pingpong-pipeline.c
	$(CHARMC) pingpong-pipeline.c -o pgm-pipe
	$(MPICC) pingpong-pipeline.c -o pgm-pipe-mpi

# This test runs longer on BGQ, so reduce the number of iterations run there:
test: all
ifeq ($(CMK_BLUEGENEQ),1)
	$(call run, +p1 ./pgm 1000 16 +vp2)
	$(call run, +p2 ./pgm 100 16 +vp2)
else
	$(call run, +p1 ./pgm 100000 16 +vp2)
	$(call run, +p2 ./pgm 10000 16 +vp2)
endif

bgtest: all
	$(call run, +p1 ./pgm 10 16 +vp2 +x1 +y1 +z1 )
	$(call run, +p2 ./pgm 10 16 +vp2 +x2 +y1 +z1 )

clean:
	rm -rf charmrun conv-host moduleinit* *.o pingpong pgm pgm-mpi pgm-pipe pgm-pipe-mpi *~ *.sts core ampirun

