# Assembly Instructions Reference Page
This page is dedicated to the documentation for the low level instructions available in Gigibox.
## Documentation Composition
Each instruction is documented in the following way:
1. Operation description
2. Arguments needed (number of arguments, arguments' description)
3. Usage
4. C++ equivalent code

# Data management instructions
## BMA
- Description: Bulk Memory Allocation. Pushes back [value] numbers in memory
- Arguments: 1
    1. value: the value to push back
- Usage:
    - `bma $8 ; adds 8 numbers to memory`
    - `bma 4 ; adds [value stored at address 4] numbers to memory`
    - `mal $a ; adds [value of A] numbers to memory`
- C++ equivalent:
```cpp
for(int i = 0; i < value; i++)
    memory.push_back(0);
```
## CMP
- Description: Compares register A to the argument and sets the logical flags to their appropriate values
- Arguments: 1
    1. value: The value to compare to
- Usage:
    - `lda $40 ; compares A to 40`
    - `lda 7 ; compares A to the value stored at address 7`
- C++ equivalent:
```cpp
logicalFlag = Compare(a, value);
```
## LDA
- Description: Loads a value into register A
- Arguments: 1
    1. value: The value that will be loaded
- Usage:
    - `lda 9 ; loads the value stored at address 9 into register A`
    - `lda $6 ; loads 6 into register A`
    - `lda $b ; loads the value stored in register B into register A`
    - `lda %$b ; loads the value at the address stored in register B into register A`
- C++ equivalent:
```cpp
a = value;
```
## MAL
- Description: Pushes back the value of the argument in memory
- Arguments: 1
    1. value: the value to push back
- Usage:
    - `mal $8 ; adds 8 to memory`
    - `mal 4 ; adds the value stored at address 4 to memory`
    - `mal $a ; adds the value of A to memory`
- C++ equivalent:
```cpp
memory.push_back(value);
```
## MOV
- Description: Moves the value of the second argument to the first argument
- Arguments: 2
    1. to: Where the data will be moved to
    2. where: The data that will be moved
- Usage:
    - `mov a 10 ; moves the value stored at address 10 to the A register`
    - `mov $a 3 ; moves the value stored at address 3 to the address stored in the A register`
    - `mov $5 $9 ; moves the value 9 to the address 5`
    - `mov 2 $0 ; moves 0 to the address stored in address 2`
- C++ equivalent:
```cpp
*to = where;
```
## STA
- Description: Stores the contents of register A to the address provided
- Arguments: 1
    1. value: The address where register A should be stored
- Usage:
    - `sta $6 ; stores register A at address 6`
    - `sta 5 ; stores register A at the address stored in address 5`
    - `sta $b ; stores register A at the address stored in register B`
    - `sta %$b ; stores register A at the address stored in the address stored in register B`
- C++ equivalent:
```cpp
*value = a;
```
## STR
- Description: Declares a string. Either duplicates a string setting the second argument as the starting index of the string or creates one by setting the second and last argument as quotation marks (") and typing the string in the middle. If the string needs to contain quotation marks, type backslash before quotation marks (\\")
- Arguments:
    - Minimum: 2
    - Maximum: As big of a string your RAM can handle ;)
    1. index: The address where the starting index will be
    2. type: either a number (meaning it's duplicating a string so it's the index of the duplicating string) or quotation marks (meaning it's assigning one)<br>
- Usage:
    - `str $0 " Hello World! " ; stores starting at address 0 the string "Hello World!"`
    - `str $9 $0 ; duplicates the string stored at address 0 to address 9`

- C++ equivalent:
```cpp
string str;
if(isNumber(type))
    str = memory[type];
else
    for(int i = 2; i < arguments.size() - 1; i++)
        str += arguments[i];
memory[index] = str;
```
## <ins>SWP</ins>
- Description: Swaps the values of two addresses
- Arguments: 2
    1. address1: The first address
    2. address2: The second address
- Usage:
    - `swp a b ; swaps the values of registers A and B`
    - `swp $12 $16 ; swaps the values of address 12 and 16`
    - `swp 5 4 ; swaps the values of the addresses stored in address 5 and 4`
- C++ equivalent:
```cpp
int c = *address1;
*address1 = *address2;
*address2 = c;
```

# Mathematic instructions
## ADD
- Description: Adds the value of the argument to register A
- Arguments: 1
    1. value: The value to be added
- Usage:
    - `add $5 ; adds 5 to register A`
    - `add 9 ; adds the value stored in address 9 to register A`
- C++ equivalent:
```cpp
a += value;
```
## DEC
- Description: Decrements register A by one
- Arguments: 0
- Usage:
    - `dec ; self explainatory`
- C++ equivalent:
```cpp
a--;
```
## DIV
- Description: Divides register A by the value of the argument
- Arguments: 1
    1. value: The value to divide by
- Usage:
    - `div $6 ; divides register A by 6`
    - `div 4 ; divides register A by the value stored in address 4`
- C++ equivalent:
```cpp
a /= value;
```
## INC
- Description: Increments register A by one
- Arguments: 0
- Usage:
    - `inc ; literally just increments A. what did you expect?`
- C++ equivalent:
```cpp
a++;
```
## MOD
- Description: Modulo. Sets register A to the remainder of the division between register A and the argument
- Arguments: 1
    1. value: The value to divide by
- Usage:
    - `mod $16 ; divides register A by 16 and sets A to the remainder`
    - `mod 12 ; divides register A by the value stored in address 12 and sets A to the remainder`
- C++ equivalent:
```cpp
a %= value;
```
## MUL
- Description: Multiplies register A by the value of the argument
- Arguments: 1
    1. value: The value to multiply by
- Usage:
    - `mul $3 ; multiplies register A by 3`
    - `mul 2 ; multiplies register A by the value stored in address 2`
- C++ equivalent:
```cpp
a *= value;
```
## SUB
- Description: Subtracts the value of the argument to register A
- Arguments: 1
    1. value: The value to subtract
- Usage:
    - `sub $9 ; subtracts 9 to register A`
    - `sub 5 ; subtracts the value stored in address 5 to register A`
- C++ equivalent:
```cpp
a -= value;
```

# Bitwise operations
## AND
- Description: sets register A to its bitwise and result with the argument
- Arguments: 1
    1. value: The value to use for the operation
- Usage:
    - `and $2 ; bitwise and operation with 2`
    - `add 4 ; you get the idea`
- C++ equivalent:
```cpp
a &= value;
```
## NOT
- Description: sets register A to its bitwise not result with the argument. All of A's ones become zeros and viceversa
- Arguments: 0
- Usage:
    - `not ; I don't have the energies to type that out`
- C++ equivalent:
```cpp
a = ~a;
```
## OR
- Description: sets register A to its bitwise or result with the argument
- Arguments: 1
    1. value: The value to use for the operation
- Usage:
    - `or $2 ; bitwise or operation with 2`
    - `or 4 ; you get the idea`
- C++ equivalent:
```cpp
a |= value;
```
## XOR
- Description: sets register A to its bitwise xor result with the argument
- Arguments: 1
    1. value: The value to use for the operation
- Usage:
    - `xor $2 ; bitwise xor operation with 2`
    - `xor 4 ; you get the idea`
- C++ equivalent:
```cpp
a ^= value;
```

# Code jumps
## JE
- Description: Jumps to the argument if in the last comparison the value was equal
- Arguments: 1
    1. line: The line to jump to
- Usage:
    - `je $21 ; jump to line 21`
- C++ equivalent:
```cpp
line:
    // code...
    // ...
    // ...
if(logicalFlags == EQUAL) goto line;
```
## JG
- Description: Jumps to the argument if in the last comparison the value was grater
- Arguments: 1
    1. line: The line to jump to
- Usage:
    - `jg $21 ; jump to line 21`
- C++ equivalent:
```cpp
line:
    // code...
    // ...
    // ...
if(logicalFlags == GREATER) goto line;
```
## JGE
- Description: Jumps to the argument if in the last comparison the value was grater or equal
- Arguments: 1
    1. line: The line to jump to
- Usage:
    - `jge $21 ; jump to line 21`
- C++ equivalent:
```cpp
line:
    // code...
    // ...
    // ...
if(logicalFlags == GREATEREQUAL) goto line;
```
## JL
- Description: Jumps to the argument if in the last comparison the value was lesser
- Arguments: 1
    1. line: The line to jump to
- Usage:
    - `jl $21 ; jump to line 21`
- C++ equivalent:
```cpp
line:
    // code...
    // ...
    // ...
if(logicalFlags == LESSER) goto line;
```
## JLE
- Description: Jumps to the argument if in the last comparison the value was lesser or equal
- Arguments: 1
    1. line: The line to jump to
- Usage:
    - `jle $21 ; jump to line 21`
- C++ equivalent:
```cpp
line:
    // code...
    // ...
    // ...
if(logicalFlags == LESSEREQUAL) goto line;
```
## JMP
- Description: Jumps unconditionally to the argument
- Arguments: 1
    1. line: The line to jump to
- Usage:
    - `jmp $21 ; jump to line 21`
    - `jmp 3 ; you get the idea`
- C++ equivalent:
```cpp
line:
    // code...
    // ...
    // ...
goto line;
```
## JNE
- Description: Jumps to the argument if in the last comparison the value was not equal
- Arguments: 1
    1. line: The line to jump to
- Usage:
    - `jne $21 ; jump to line 21`
- C++ equivalent:
```cpp
line:
    // code...
    // ...
    // ...
if(logicalFlags == NOTEQUAL) goto line;
```

# Functions related instructions
## CALL
- Description: Calls the function by name given by the argument
- Arguments: 1
    1. name: Name of the function
- Usage:
    - `call foo ; calls foo`
    - `call bar ; calls bar`
- C++ equivalent:
```cpp
name();
```
## FNC
- Description: Declares a function. runs everything until the RET instruction
- Arguments: 1
    1. name: Name of the function
- Usage:
    - `fnc foo ; declares function foo`
    - `fnc bar ; declares function bar`
- C++ equivalent:
```cpp
void name();
```
## RET
- Description: Jump back to the line the function was called
- Arguments: 0
- Usage:
    - `ret ; I'm tired`
- C++ equivalent:
```cpp
void function(){
    // code...
    // ...
    // ...
    return;
}
int main(void){
    function();
    printf("successfully returned\n");
}
```

# Miscellaneous
## INT
- Description: Interrupts code interpretation to give control back to the display loop
- Arguments: 1
    1. code: Depending on the code, the interrupt can run additional code using the registers as parameters
- Usage:
    - `int $0 ; code 0 runs no additional code. control given back to display loop normally`
- C++ equivalent:
```cpp
while(true){
    bool interrupted = false;
    while(!interrupted){
        // code...
        // ...
        // ...
        interrupted = true;
        switch(code){
            case 1:
            // do something
            break;
            case 2:
            // do something else
            break;
        }
    }
    display();
}
```
## LSA
- Description: Sets register A to the index of the last element in the Data memory vector
- Arguments: 0
- Usage:
    - `lsa ; literally nothing else`
- C++ equivalent:
```cpp
a = memory.size() - 1;
```
## TEST
- Description: sets the logical flags to register A's bitwise and result with the argument
- Arguments: 1
    1. value: The value to use for the operation
- Usage:
    - `test $2 ; test with 2`
    - `test 4 ; you get the idea`
- C++ equivalent:
```cpp
logicalFlag = Compare(a, a & value);
```
