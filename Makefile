iap: main.o				#Add here depandent object file
	gcc -o iap main.o

main.o: main.c
	gcc -c -o main.o main.c

clean:
	rm *.o iap
