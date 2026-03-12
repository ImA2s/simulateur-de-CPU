all: main memoryTest parserTest cpuTest

main: main.o cpuTest.o cpu.o hachageg.o memory.o parser.o
	gcc -g -Wall -o main main.o cpu.o hachageg.o memory.o parser.o

main.o: main.c cpu.h hachageg.h memory.h parser.h
	gcc -c -Wall -o main.o main.c

memoryTest: memoryTest.o hachageg.o memory.o
	gcc -g -Wall -o memoryTest memoryTest.o hachageg.o memory.o

memoryTest.o: memoryTest.c hachageg.h memory.h
	gcc -c -Wall -o memoryTest.o memoryTest.c

parserTest: parserTest.o parser.o hachageg.o
	gcc -g -Wall -o parserTest parserTest.o parser.o hachageg.o

cpuTest: cpuTest.o cpu.o hachageg.o memory.o parser.o
	gcc -g -Wall -o cpuTest cpuTest.o cpu.o hachageg.o memory.o parser.o

parserTest.o: parserTest.c parser.h hachageg.h
	gcc -c -Wall -o parserTest.o parserTest.c

cpuTest.o: cpuTest.c cpu.h hachageg.h memory.h parser.h
	gcc -c -Wall -o cpuTest.o cpuTest.c

hachageg.o: hachageg.h hachageg.c
	gcc -c -Wall -o hachageg.o hachageg.c

memory.o: memory.h memory.c
	gcc -c -Wall -o memory.o memory.c

parser.o: parser.h parser.c
	gcc -c -Wall -o parser.o parser.c

cpu.o: cpu.c cpu.h
	gcc -c -Wall -o cpu.o cpu.c

clean:
	rm -f *.o main parserTest memoryTest cpuTest
