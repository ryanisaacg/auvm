CFLAGS= -Wall -Wextra -Werror -Wfatal-errors -Iinclude -g -std=c99

all: auvm.out translator.out lc.out asm.out dasm.out bios bios-dasm

run: all
	./auvm.out

clean:
	rm -r machine/obj
	rm -r compiler/obj
	rm auvm.out
	rm lc.out
	rm asm.out
	rm dasm.out	

#Compile the bios
bios-asm: bios-source lc.out
	./lc.out bios-source bios-asm
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

#Compile the LISP Compiler
compiler/obj:
	mkdir compiler/obj
compiler/obj/ast.o: compiler/obj compiler/ast.h compiler/ast.c
	gcc compiler/ast.c $(CFLAGS) -c -o compiler/obj/ast.o
compiler/obj/ir.o: compiler/obj compiler/ir.c compiler/ir.h
	gcc compiler/ir.c $(CFLAGS) -c -o compiler/obj/ir.o
compiler/obj/emitter.o: compiler/obj compiler/emitter.h compiler/emitter.c
	gcc compiler/emitter.c $(CFLAGS) -c -o compiler/obj/emitter.o
compiler/obj/stack.o: compiler/obj compiler/stack.h compiler/stack.c
	gcc compiler/stack.c $(CFLAGS) -c -o compiler/obj/stack.o
compiler/obj/table.o: compiler/obj compiler/table.h compiler/table.c
	gcc compiler/table.c $(CFLAGS) -c -o compiler/obj/table.o
lex.c: compiler/parser.l
	flex -olex.c compiler/parser.l
yacc.tab.c: compiler/parser.y
	bison -d byacc compiler/parser.y
lc.out: lex.c yacc.tab.c compiler/obj/ast.o compiler/obj/table.o compiler/obj/ir.o compiler/obj/emitter.o compiler/obj/stack.o
	gcc -Icompiler lex.c yacc.tab.c  -Wno-implicit-function-declaration compiler/obj/table.o compiler/obj/ast.o compiler/obj/ir.o  compiler/obj/emitter.o compiler/obj/stack.o -std=c99 -o lc.out

# Compile the VM
auvm.out: machine/obj/io.o machine/obj/machine.o
	gcc machine/obj/* $(CFLAGS) -o auvm.out

machine/obj/machine.o: machine/machine.c machine/machine.h machine/obj machine/obj/io.o include/instructions.h
	gcc machine/machine.c $(CFLAGS) -c -o machine/obj/machine.o

machine/obj/io.o: machine/io.c machine/io.h machine/obj
	gcc machine/io.c $(CFLAGS) -c -o machine/obj/io.o

machine/obj:
	mkdir machine/obj

