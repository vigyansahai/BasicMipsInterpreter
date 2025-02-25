# BasicMipsInterpreter
*CS64 CCS Lab Project, takes in raw binary and runs MIPS instructions*

## How to run
There are two .cpp files, main is the interpreter with all the debugging print statements to show what occurs inside the interpreter, mainNoComments does not have these debugging print statements.\
The file takes binary Big Endian input from the input.txt file. It immidiately starts reading code as intrustions, as such all data must be stored at the end of the file. Sample inputs have been provided in the inputPrep.txt file. \
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; *Writing this code in raw binary is extremely annoying, I recomend using this [website](https://mips-converter.jeffsieu.com/), it does not have all the commands but will help a lot if you plan to write your own binary input:*

## Instructions Implemented

### R Instructions (OPCODE 0, FUNCT: x):

- **addu** 
- **sub** prints overflow if there is overflow
- **add** prints overflow if there is overflow
- **subu** 
- **and**
- **nor**
- **xor**
- **or**
- **slt**
- **sltu**
- **sll**
- **srl**
- **div**
- **divu**
- **mfhi**
- **mflo**
- **mult**
- **multu**

### I Instructions (OPCODE: x):

- **addi** prints overflow if there is overflow
- **addiu**
- **andi**
- **beq** adds the immidiate to the PC, DOES NOT ADD ANY EXTRA 4 BYTES, when I was implementing this, I could not figure out why the MIPS reference card implemented it like this
- **bne** adds the immidiate to the PC, DOES NOT ADD ANY EXTRA 4 BYTES, when I was implementing this, I could not figure out why the MIPS reference card implemented it like this
- **lbu** loads from a binary interpretation
- **lw** loads from a binary interpretation
- **ori**
- **slti**
- **sltiu**
- **sb** stores in a binary interpretation
- **sw** stores in a binary interpretation

### J Instructions (OPCODE x):

- **jr**
- **j** 
- **jal**

### Syscalls (OPCODE 0, FUNCT 0xC):
- **Print int:** input 1 into $v0, it prints what is in $a0
- **Print string:** input 4 into $v0, it prints from the address inputed into $a0 until it hits a null terminator
- **Take input int:** input 5 into $v0, it takes in input from std::cin and stores it in $v0
- **Take input string:** input 8 into $v0, the buffer address to write in $a0, and the length of the string in $a including the null terminator, it takes in input from std::cin and stores it with a null terminator
- **Quit the program:** input 10 into $v0, it quits the program successfully
