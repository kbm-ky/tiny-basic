./clean.sh
gcc -g -o scanner_test.exe src/scanner_test.c src/scanner.c -I ./include && ./scanner_test.exe
gcc -g -o variable_test.exe src/variable_test.c src/variables.c -I ./include && ./variable_test.exe
gcc -g -o interpreter_test.exe src/interpreter_test.c src/interpreter.c src/scanner.c src/editor.c src/variables.c src/stack.c src/program.c -I ./include && ./interpreter_test.exe