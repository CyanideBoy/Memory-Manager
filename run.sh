# commands to compile and run custom memory allocator

# for test case 1
#g++ -c test_alloc1.c
#g++ -c alloc.cpp
#g++ test_alloc1.o alloc.o -o alloc
#./alloc

# for test case 2
if [ -f alloc.c ]
then
gcc -c test_alloc2.c
gcc -c alloc.c
gcc test_alloc2.o alloc.o -o alloc
else
g++ -c test_alloc2.cpp
g++ -c alloc.cpp
g++ test_alloc2.o alloc.o -o alloc
fi
./alloc



rm *.o alloc
