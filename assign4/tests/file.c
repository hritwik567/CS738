#include<stdio.h>

int main(){
	int a = 4; //tmp1
	int b = a; //tmp2
	a = a+1;
	if(b == 0){
		a = b + 1;
	} else {
		b = a + 1;
	}
	return a;
}
