CFLAGS= -Wall -Wextra -Werror -Wfatal-errors -Iinclude -g

run: auvm.out
	./auvm.out

clean:
	rm auvm.out

auvm.out: machine/machine.c include/instructions.h machine/machine.h
	gcc machine/machine.c $(CFLAGS) -o auvm.out
