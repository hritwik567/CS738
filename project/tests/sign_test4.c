#include<stdio.h>

int f(int a, int b);

int g(int u) {
  int v = f(-u, u);
  return v;
}

int f(int a, int b) {
  int c;
  if(1) {
    c = g(10);
  }
  return c;
}

int main() {
	int p = 5;
	int q = f(p, -3);
	int r = g(-q);
	return 0;
}
