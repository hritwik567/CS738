#include "context_transition_table.h"

using namespace std;

template<>
int Context<int, int, int>::count = 0;

int main() {
	ContextTransitionTable<int, int, int> c;
	cout << "Hello World!" << endl;
	return 0;
}
