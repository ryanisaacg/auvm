CFLAGS= -Wall -Wextra -Werror -Wfatal-errors -Iinclude -g -std=c99

all: auvm.out translator.out lc.out asm.out dasm.out

run: auvm.out bios
	./dasm.out bios bios-dasm
	./auvm.out

clean:
	rm machine/obj/*
	rm auvm.out

#Compile the bios
bios: bios-asm asm.out
	./asm.out bios-asm bios
#Disassemble the bios to examine if it has been assembled correctly
bios-dasm: bios dasm.out
	./dasm.out bios bios-dasm

#Compile the assembler and disassembler
asm.out: assembler/assembler.l assembler/assembler.y
	flex -oasmlex.c assembler/assembler.l
	bison -d -basmyacc assembler/assembler.y
	gcc asmlex.c asmyacc.tab.c -Iinclude -Wno-implicit-function-declaration -g -o asm.out
dasm.out: include/instructions.h assembler/disassembler.c
	gcc assembler/disassembler.c $(CFLAGS) -o dasm.out

#Compile the translator tool
translator.out: tools/translator.c
	gcc tools/translator.c $(CFLAGS) -o translator.out

# Compile the VM
auvm.out: machine/obj/io.o machine/obj/machine.o
	gcc machine/obj/* $(CFLAGS) -o auvm.out

machine/obj/machine.o: machine/machine.c machine/machine.h machine/obj machine/obj/io.o include/instructions.h
	gcc machine/machine.c $(CFLAGS) -c -o machine/obj/machine.o

machine/obj/io.o: machine/io.c machine/io.h machine/obj
	gcc machine/io.c $(CFLAGS) -c -o machine/obj/io.o

machine/obj:
	mkdir machine/obj

