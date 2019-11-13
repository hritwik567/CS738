#include<stdio.h>

int five() {
  return 5;
}

int main() {
	int p = five();
  int q = p - 100;
  int r;
  if(q < 0) {
    r = p + five();
  } else {
    r = -p;
  }
	return r;
}
