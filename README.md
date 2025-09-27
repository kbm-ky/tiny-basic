# Tiny Basic Interpreter  

An attempt to make an interactive mode [Tiny Basic](https://en.wikipedia.org/wiki/Tiny_BASIC) interpreter in C.  
I'm trying to kill two birds with one stone:  
1. [Challenging projects every programmer should try](https://austinhenley.com/blog/challengingprojects.html)  
2. [Boot.Dev First Personal Project](https://www.boot.dev/courses/build-personal-project-1)  

The user experience and error messages are rough and inconsistent, but it's functional enough as far as I'm willing to test it.  

## Building  

Built and tested on the following configuration:
- Ubuntu 24.04 LTS via Windows 11 WSL2
- GCC 13.3.0

Run the `build.sh` script in the root of the repository.

## Running  

Use the `run.sh` script or the `tinybas.exe` executable yielded by the `build.sh` script.  

```
$ ./run.sh
Welcome to TINY-BASIC!

Type BYE to exit interpreter.
>
```

## The Language  

### Statements

This variant/dialect of Tiny Basic supports the following statements:
- `IF` *expression* *relop* *expression* `THEN` *statement* - a conditional flow control structure
- `GOTO` *expression* - an unconditional jump
- `GOSUB` *expression* - a call to a subroutine that `RETURN`s
- `RETURN` - returns execution to the next location after a `GOSUB`
- `LET` *var* = *expression* - assigns a value to a variable name
- `INPUT` *var* (, *var* ...) - prompts the user for a number for each variable in a comma separated list, e.g `INPUT X, Y, Z`
- `PRINT` *expression* or *string*, ... - prints a comma separated list of expressions or "-enclosed strings, e.g. `PRINT "1 + 1 equals", 1 + 1`

### Expressions  

Expressions are limited to numbers and variable names (but `PRINT` can also print strings).  

Numbers are limited to 16-bit integers.  (Don't be surprised if you overflow/underflow doing math operations.)  

Mathematical operators are `+`, `-`, `*`, and `/` (integer division of course).  

Variable names are limited to the 26 upper case characters `A` through `Z`.  

"Relops" are used to compare values in `IF` statements:
- `=` - equal to
- `<>` or `><` - not equal to
- `<` - less than
- `<=` - less than or equal to
- `>` - greater than
- `>=` - greater than or equal to

## The Interpreter  

The interpreter operates in two modes: `DIRECT` and `INDIRECT`.  It begins in `DIRECT` mode and switches to `INDIRECT` mode when the user runs a program.  

User input is limited to 70 characters (actually 70 + carriage return + 1 null character to terminate a C string).  

There are a few `DIRECT` commands or statements:  
- `BYE` - exits the interpreter
- `LIST` - lists the program in memory
- `CLEAR` - clears the program memory
- `RUN` - runs the program in memory

### Programs

Program memory has enough storage for 256 lines of input in slots 0 through 255.  

The user can enter statements into program memory by typing a number before a statement, e.g. `> 10 PRINT "HELLO"`.  

To clear a statement from the program, the user can type the line number without a statement, e.g. `> 10`.  

To clear the whole program, use the `CLEAR` command.  

#### Examples  

##### Adder  

Adds two numbers input by the user.  

```
> LIST
10 PRINT "ADDER"
15 PRINT "Enter values to add"
20 INPUT A, B
30 PRINT A + B
40 PRINT "Continue? (yes=1)"
50 INPUT Y
60 IF Y = 1 THEN GOTO 15
70 END
```

##### Fibonacci Series  

Computes the Fibonacci series of the number input by the user.  

```
> LIST
5 PRINT "FIBONACCI SERIES"
10 PRINT "Enter a number greater than 0"
15 INPUT X
20 IF X <= 0 THEN GOTO 10
25 GOSUB 100
30 PRINT "Again? (yes=1)"
35 INPUT Y
40 IF Y = 1 THEN GOTO 10
50 END
100 LET C = 1
105 LET P = 0
110 LET G = 0
115 IF X = 0 THEN RETURN
120 PRINT C
125 LET G = P
130 LET P = C
135 LET C = P + G
140 LET X = X - 1
145 GOTO 115
> RUN
FIBONACCI SERIES
Enter a number greater than 0
X = ? 8
1
1
2
3
5
8
13
21
Again? (yes=1)
Y = ? 0
> 
```
