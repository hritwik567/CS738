#include<stdio.h>

int foo(int a) {
  return a;
}

int foo1(int a) {
  return a;
}
int foo2(int a) {
  return a;
}
int foo3(int a) {
  return a;
}
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
