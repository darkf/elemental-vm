evm:	bios.o cpu.o main.o op.o vm.o
	cc *.o -o evm
clean:
	rm -f *.o evm

