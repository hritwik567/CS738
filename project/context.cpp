#include "context.h"

template<class M, class N, class A>
Context<M, N, A>::Context(M method): method(method), analysed(false) {
	id = ++count;
}

template<class M, class N, class A>
Context<M, N, A>::Context(M method, N node): method(method), node(node), analysed(false) {
	id = ++count;
}

template<class M, class N, class A>
bool Context<M, N, A>::operator==(const Context<M, N, A>& c) const {
	return id == c.getId();
}

namespace std {
	template<class M, class N, class A>
	struct hash<Context<M, N, A>> {
		size_t operator()(const Context<M, N, A>& c) const {
			return hash<int>()(c.getId());
		}
	};
}

template<class M, class N, class A>
int Context<M, N, A>::getId(void) {
	return id;
}

template<class M, class N, class A>
M Context<M, N, A>::getMethod(void) {
	return method;
}

template<class M, class N, class A>
N Context<M, N, A>::getNode(void) {
	return node;
}

template<class M, class N, class A>
A Context<M, N, A>::getEntryValue(void) {
	return entry_value;
}

template<class M, class N, class A>
A Context<M, N, A>::getExitValue(void) {
	return exit_value;
}

template<class M, class N, class A>
A Context<M, N, A>::getValueBefore(N node) {
	// TODO: Implement this not sure what exactly is this
	return entry_value;
}

template<class M, class N, class A>
A Context<M, N, A>::getValueAfter(N node) {
	// TODO: Implement this not sure what exactly is this
	return exit_value;
}

template<class M, class N, class A>
bool Context<M, N, A>::isAnalysed(void) {
	return analysed;
}

template<class M, class N, class A>
void Context<M, N, A>::markAnalysed(void) {
	analysed = true;
	return;
}

template<class M, class N, class A>
void Context<M, N, A>::unmarkAnalysed(void) {
	analysed = false;
	return;
}

template<class M, class N, class A>
void Context<M, N, A>::setEntryValue(A _entry_value) {
	entry_value = _entry_value;
	return;
}

template<class M, class N, class A>
void Context<M, N, A>::setExitValue(A _exit_value) {
	exit_value = _exit_value;
	return;
}