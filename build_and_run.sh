mkdir -p ./build
gcc -O3 -mbmi2 -madx main.c -o ./build/main.o
./build/main.o