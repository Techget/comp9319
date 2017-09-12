#include <stdio.h>

void func ( void (*f)(int) ){
	int i=0;
	for(i=0; i<5; i++){
		(*f)(i);
	}
}
void print ( int x ) {
	printf("%d\n", x);
}

int main(){
	func(print);
}