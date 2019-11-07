#include "context.h"

template<class M, class N, class A>
Context<M, N, A>::Context(): analysed(false), is_null(true) {
	id = ++count;
}

template<class M, class N, class A>
Context<M, N, A>::Context(M method, bool reverse): method(method), analysed(false), is_null(false), reverse(reverse) {
	id = ++count;
}

template<class M, class N, class A>
bool Context<M, N, A>::operator==(const Context<M, N, A>& c) const {
	return id == c.getId();
}

template<class M, class N, class A>
int Context<M, N, A>::getId(void) const {
	return id;
}

template<class M, class N, class A>
M Context<M, N, A>::getMethod(void) {
	return method;
}

template<class M, class N, class A>
N Context<M, N, A>::getControlFlowGraph(void) {
	// TODO: Need to update this to return an iterator or a vector
	return node;
}

template<class M, class N, class A>
N Context<M, N, A>::getEntryNode(void) {
	// TODO: Need to update this to return the entry node based on the method
	return &method->getEntryBlock();
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
	return in_values[node];
}

template<class M, class N, class A>
A Context<M, N, A>::getValueAfter(N node) {
	return out_values[node];
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

template<class M, class N, class A>
void Context<M, N, A>::setValueBefore(N _node, A _value) {
	in_values[_node] = _value;
	return;
}

template<class M, class N, class A>
void Context<M, N, A>::setValueAfter(N _node, A _value) {
	out_values[_node] = _value;
	return;
}

template<class M, class N, class A>
void Context<M, N, A>::addToWorklist(N _node) {
	// TODO: Need to update this based on reverse value
	worklist.push_back(_node);
	return;
}

// Define the class here so that main file can use
template class Context<llvm::Function*, llvm::BasicBlock*, int>;
