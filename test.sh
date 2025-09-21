./clean.sh
gcc -o scanner_test.exe src/scanner_test.c src/scanner.c -I ./include && ./scanner_test.exe
gcc -o parser_test.exe src/parser_test.c src/scanner.c -I ./include && ./parser_test.exe