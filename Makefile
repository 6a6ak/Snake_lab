PROG=main.exe
CC=gcc
CFLAGS=-g -Wall -Werror
CFLAGS2=-O3 -Wall -Werror
DEPS = 
OBJ_CODE = 
OBJ = $(OBJ_CODE) main.o 



GTEST = . #gtest-1.7.0/include
#LIBGTEST = /mingw64/lib/libgtest_main.a /mingw64/lib/libgtest.a
LIBGTEST = C:\msys64\mingw64\lib\libgtest_main.a C:\msys64\mingw64\lib\libgtest.a
#TESTDIR = tests
 
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
 
$(PROG): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)