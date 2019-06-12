#include <stdio.h>
#include <stdlib.h>

void* operator new(size_t bytes)
{
	printf("allocated %i bytes\n", bytes);
	return malloc(bytes);
}

void* operator new[](size_t bytes)
{
	printf("allocated %i array bytes\n", bytes);
	return malloc(bytes);
}

void operator delete(void* ptr, size_t size)
{
	printf("deleting %p", ptr);
	free(ptr);
}

void operator delete[](void* ptr, size_t size)
{
	printf("deleting array: %p", ptr);
	free(ptr);
}

int main()
{
	int* i = new int;
	int* array = new int[10];

	printf("address %p, = %i\n", i, *i);
	system("PAUSE");

	delete[] array;
	delete i;
}
