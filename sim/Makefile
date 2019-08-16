TRAD=./translater/
SCORE=./scoreboarding/
TRAD_SOURCES=$(TRAD)*.c -lfl
SCORE_SOURCES=$(SCORE)*.c
CXX=gcc
CXXFLAGS=-W

all: C

C: main
	@rm -f *.out
	@rm -f */*.out

main: T 
	$(CXX) $(CXXFLAGS) sim.c memory.c registers.c clock.c cpu.c instructions_queue.c $(SCORE_SOURCES) $(TRAD_SOURCES) -o mips32sim
	@echo "Sucesso na compilação!"

T:
	@make -C $(TRAD)