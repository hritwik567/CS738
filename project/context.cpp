#include "context.h"

template<class M, class N, class A>
Context<M, N, A>::Context(): analysed(false), is_null(true) {
	// id = ++count;
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
std::vector<N> Context<M, N, A>::getControlFlowGraph(void) {
	std::vector<N> wl;
	for(N BB: depth_first(&method->getEntryBlock())) {
  	wl.push_back(BB);
  }
	std::reverse(wl.begin(), wl.end());
	return wl;
}

template<class M, class N, class A>
std::vector<N> Context<M, N, A>::getPredsOf(N _node) {
	std::vector<N> wl;
	for(N Pred: predecessors(_node)) {
		wl.push_back(Pred);
	}

	return wl;
}

template<class M, class N, class A>
std::vector<N> Context<M, N, A>::getSuccsOf(N _node) {
	std::vector<N> wl;
	for(N Succ: successors(_node)) {
		wl.push_back(Succ);
	}

	return wl;
}

template<class M, class N, class A>
std::vector<N> Context<M, N, A>::getTails(void) {
	std::vector<N> wl;
	for(N BB: depth_first(&method->getEntryBlock())) {
		for(auto &I: *BB) {
			if(llvm::isa<llvm::ReturnInst>(I)) {
				wl.push_back(BB);
				break;
			}
		}
	}
	return wl;
}

template<class M, class N, class A>
bool Context<M, N, A>::isCall(N _node) {
	for(auto &I: *_node) {
		if(llvm::isa<llvm::CallInst>(I)) {
			return true;
		}
	}
	return false;
}

template<class M, class N, class A>
N Context<M, N, A>::getEntryNode(void) {
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
A Context<M, N, A>::getValueBefore(N _node) {
	return in_values[_node];
}

template<class M, class N, class A>
A Context<M, N, A>::getValueAfter(N _node) {
	return out_values[_node];
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

// TODO: Need to update this based on reverse value
// ERROR: If there is any error look here
template<class M, class N, class A>
void Context<M, N, A>::addToWorklist(N _node) {
	worklist.push_back(_node);
	return;
}

template<class M, class N, class A>
bool Context<M, N, A>::isEmptyWorklist(void) {
	return worklist.empty();
}

template<class M, class N, class A>
std::vector<N> Context<M, N, A>::getWorklist(void) {
	return worklist;
}

template<class M, class N, class A>
N Context<M, N, A>::getAndPopWorklist(void) {
	N _node = worklist.back();
	worklist.pop_back();
	return _node;
}

template<class M, class N, class A>
N Context<M, N, A>::getLastWorklist(void) {
	if(worklist.empty()) return NULL;
	return worklist.back();
}

// Till Here can use pq for worklist as well

template<class M, class N, class A>
int Context<M, N, A>::count = 0;

// Define the class here so that main file can use
template class Context<llvm::Function*, llvm::BasicBlock*, Sign>;
