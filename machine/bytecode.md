#VM Bytecode

All indices are in hexidecimal

##Commands

- mov: Copy memory from one location to another
	- mov R?(SourceIndex) R?(DestinationIndex)
- add: Add two registers and store the result in another register
	- add R(AIndex) R(BIndex) R(ResultIndex)
- sub: Subtract two registers and store the result in another register
	- sub R(AIndex) R(BIndex) R(ResultIndex)
- mul: Multiply two registers and store the result in another register
	- mul R(AIndex) R(BIndex) R(ResultIndex)
- div: Divide two registers and store the result in another register
	- div R(AIndex) R(BIndex) R(ResultIndex)
- mod: Modulo two registers and store the result in another register
	- mod R(AIndex) R(BINdex) R(ResultIndex)
- neg: Negate a value 
	- neg R(SourceIndex) R(ResultIndex)
- rfi: Read from the input device and store the result in a register
	- rfi R(ResultIndex)
- wto: Write to the output device from a register
	- wto R(DataIndex)
- cmp: Compares two source registers and stores it in the comparison register
	- cmp R(AIndex) R(BIndex) R(ResultIndex)
- bie: Branches if the most recent comparison was equal
	- bie R(SourceIndex) (Label)
- big: Branches if the most recent comparison was greater
	- big R(SourceIndex) (Label)
- bil: Branches if the most recent comparison was less
	- bil R(SourceIndex) (Label)
- bne: Branches if the most recent comparison was not equal
	- bne R(SourceIndex) (Label)
- bng: Branches if the most recent comparison was not greater
	- bng R(SourceIndex) (Label)
- bnl: Branches if the most recent comparison was not less
	- bnl R(SourceIndex) (Label)
- pfs: Pushes the current line to the function stack and moves the stack pointer
	- pfs
- pop: Pops the stack pointer, destroying all values on the stack. Returns to the last pfs call
	- pop
- and: Performs a binary and on two registers
	- and R(AIndex) R(BIndex) R(ResultIndex)
- ior: Inclusive binary or
	- ior R(AIndex) R(BIndex) R(ResultIndex)
- xor: Exclusive binary or
	- xor R(AIndex) R(BIndex) R(ResultIndex)
##Constants
Constants can be used in place of source registers, and are denoted with #

##Pointers
Pointers are denoted with a hex string with no R or # prefix.

##Bytecode representation
The commands are in the order listed above, with mov = 0
Values are 5 bytes, with the first byte indicating Register, Value, or Pointer and the next four representing the value
Registers and Pointers are unsigned 32-bit integers, Values are signed 32-bit integers
