#include "context_transition_table.h"

using namespace std;

template<>
int Context<int, int, int>::count = 0;

int main() {
	ContextTransitionTable<int, int, int> c;
	Context<int, int, int> c1(0);
	cout << "Hello World!" << c1.getId()  << endl;
	c.sayHello();
	return 0;
}
