./clean.sh
gcc -g -o scanner_test.exe src/scanner_test.c src/scanner.c -I ./include && ./scanner_test.exe
gcc -g -o parser_test.exe src/parser_test.c src/parser.c src/scanner.c -I ./include && ./parser_test.exe
gcc -g -o variable_test.exe src/variable_test.c src/variables.c -I ./include && ./variable_test.exe
gcc -g -o node_pool_test.exe src/node_pool_test.c src/node_pool.c -I ./include && ./node_pool_test.exe