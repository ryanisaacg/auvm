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
- rfi: Read from the input device and store the result in a register
	- rfi R(ResultIndex)
- wto: Write to the output device from a register
	- wto R(DataIndex)
- cmp: Compares two values and stores the result in the comparison register
	- cmp R(AIndex) R(BIndex)
- brn: Branches to another location in the code
	- brn (Label)
- gtb: Go to a specific byte in RAM and continue executing
	- gtb R(Position)
- gcb: Get the current byte the CPU is executing and store it
	- gcb R(Position)
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
- rhd: Read from the hard disk at a given index
	- rhd (DiskLocation) R(ResultIndex)
- whd: Write to the hard disk at a given index
	- whd R(SourceIndex) (DiskLocation)
- lbl: Defines a label (converted to index at assemble-time)
	- lbl "(String)"
- dbg: Print debug information (prints registers and dumps the RAM to an output file)
	- dbg
##Constants
Constants can be used in place of source registers, and are denoted with =

##Pointers
Pointers are denoted with a decimal string with a % prefix.

##Registers
Registers are denoted with an 'R'. The value of a dereferenced register is represented by R$.

##Bytecode representation
The commands are in the order listed above, with mov = 0

Values are 5 bytes, with the first byte indicating Register, Register Value, Constant Value, or Pointer and the next four representing the value

Pointers, Registers and Constant Values are signed 32-bit integers

##Branches and labels
At assemble-time, labels are converted into indices at both the location of the branch and the label.

##Conditional Commands
Commands have a byte with a condition under which they execute. The conditions are:

- un: Always execute
- eq: The last comparison was equal
- ne: The last comparison was not equal
- gr: The last comparison was greater
- ng: The last comparison was not greater
- ls: The last comparison was less
- nl: The last comparison was not less

If no conditional is present, un is implied.
