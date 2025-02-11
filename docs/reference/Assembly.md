# Introduction to Gigibox's Assembly code
Gigibox's Assembly inspired low level interpreted programming language has access to <ins>3 registers</ins> and dynamically expanding memory.<br>
All code is **case-insensitive**, so an all-uppercase line of code is the same as it's lowercase version

# Architecture of the system
## General Memory
There are two main types of memory:
1. Program memory. Where all instructions and code are stored in.
2. Data memory. Where all data accessible to the program is stored in.
## Registers
Registers are addresses in Data memory that are used by operations and interrupts.<br>
Registers are reserved addresses in the first indexes of data memory, so all data memory is offsetted by how many registers there are (<ins>currently 3</ins>).<br>
Because of this, these reserved memory addresses are inaccessible through conventional ways, except with their proprietary keyword that accesses memory using their index in a hardcoded array incremented by one and negated (`-(index + 1)` so `a` = `-1`).

# Composition of a line
Each line is composed by 3 components:
1. Instruction
2. Arguments
    - Instructions are the task the program has been assigned to do in that instant. It can be mathematical operations (happening in the `A` register), data or memory manipulation, or other miscellaneous stuff.
    - Arguments are the parameters the instruction gets assigned to do the task it needs to. The amount of arguments depends on the specific instruction that precedes them.
3. Comments
    - Comments are started with a semicolon (;) and last until the end of the line

# Argument and value formatting

- Arguments (typically numbers) when accompanied by a dollar sign (`$`) are treated as absolute values (with the exception of registers that instead get the value stored in them), otherwise they get treated as addresses in memory (es. `$6` = `6`, `6` = `memory[6]`, `$a` = the value of `A`).
- When numbers (es. `$3`, `$a`, etc.) are accompanied by a percent sign (%), they get treated as memory pointers. In that the number refers to an index in memory (`%$9` = `memory[9]`, `%$a` = `memory[a]`).
    - This feature is designed for register values as it's redundant for absolute values, an absolute value without the dollar sign is the same as a memory pointer (`4` == `%$4`).
    - Instructions like `mov` automatically suppose the first argument is referring to an address, so to move data to a predefined address, the dollar sign is needed<br>
    (`mov $4 10` = `memory[4] = memory[10]`. `mov 4 10` = `memory[memory[4]] = memory[10]`)<br>
    (`mov $4 $10` = `memory[4] = 10`).