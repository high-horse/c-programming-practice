#include <stdio.h>


// static in function scope make it initialized once 
// the program starts and never changes 
// even it is called multiple times
void counter(void) {
	static int counter = 1;
	printf("the cointer has been called %d times yet.\n", counter);
	counter++;
}

// whereas static function or static outside function
// or file level global static are only accessable by the file 
// and cannot be accessed through outside the file
static void counter_again(void) {
	int count = 1;
	printf("the counter again envoked %d \n", count);
	count++;
}

int main(void) {
	counter();
	counter();
	counter();

	counter_again();
	counter_again();
	counter_again();
}
