#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define snum  4

int A(int x, int y){
	if(x==0){
		y = y+1;
		return y;
	}
	else if (y == 0){
		return A(x-1,1);
	}
	else return A(x-1,A(x,y-1));
}

int main()
{	
	int a = 1;
	int b = 1;
	int out = A(a,b);
	printf("for %i and %i, the Ackermann function returns%i\n",a,b, out);

	a = 2;
	b = 3;
	out = A(a,b);
	printf("for %i and %i, the Ackermann function returns%i\n",a,b, out);
	
	a = 1;
	b = 3;
	out = A(a,b);
	printf("for %i and %i, the Ackermann function returns%i\n",a,b, out);


	a = 3;
	b = 4;
	out = A(a,b);
	printf("for %i and %i, the Ackermann function returns%i\n",a,b, out);



}



