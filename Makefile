CFLAGS= -Wall -Wextra -Werror -Wfatal-errors -Iinclude

run: auvm.out
	./auvm.out

auvm.out: machine/machine.c include/instructions.h machine/machine.h
	gcc machine/machine.c $(CFLAGS) -o auvm.out
