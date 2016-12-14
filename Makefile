CFLAGS= -Wall -Wextra -Werror -Wfatal-errors -Iinclude -g

run: auvm.out
	./auvm.out

clean:
	rm -r machine/obj
	rm auvm.out

auvm.out: machine/obj/io.o machine/obj/machine.o
	gcc machine/obj/* $(CFLAGS) -o auvm.out

asm.out: assembler/assembler.c
	gcc assembler/assembler.c $(CFLAGS) -o asm.out

translator.out: tools/translator.c
	gcc tools/translator.c $(CFLAGS) -o translator.out

machine/obj/machine.o: machine/machine.c machine/machine.h machine/obj machine/obj/io.o include/instructions.h
	gcc machine/machine.c $(CFLAGS) -c -o machine/obj/machine.o

machine/obj/io.o: machine/io.c machine/io.h machine/obj
	gcc machine/io.c $(CFLAGS) -c -o machine/obj/io.o

machine/obj:
	mkdir machine/obj

