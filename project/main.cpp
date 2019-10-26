#include "context_transition_table.h"

using namespace std;

namespace std {
	template<class M, class N, class A>
	struct hash<Context<M, N, A>> {
		size_t operator()(const Context<M, N, A>& c) const {
			return hash<int>()(c.getId());
		}
	};
}

template<>
int Context<int, int, int>::count = 0;

int main() {
	ContextTransitionTable<int, int, int> c;
	cout << "Hello World!" << endl;
	return 0;
}