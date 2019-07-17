TRAD=./translater/
SCORE=./scoreboarding/
TRAD_SOURCES=$(TRAD)*.c -lfl
SCORE_SOURCES=$(SCORE)*.c

all: C

main: T
	gcc sim.c memory.c registers.c clock.c cpu.c instructions_queue.c $(SCORE_SOURCES) -o sim

T:
	@make -C $(TRAD)

C: main
	@rm -f *.o
	@rm -f */*.o