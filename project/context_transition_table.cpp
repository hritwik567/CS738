#include "context_transition_table.h"

template<class M, class N, class A>
void ContextTransitionTable<M, N, A>::addTransition(CallSite<M, N, A> call_site, Context<M, N, A> target_context) {
	if(!target_context.is_null) {
			M target_method = target_context.getMethod();

			if(transitions.find(call_site) != transitions.end() && transitions[call_site].find(target_method) != transitions[call_site].end()) {
				Context<M, N, A> old_target = transitions[call_site][target_method];
				callers[old_target].erase(std::remove(callers[old_target].begin(), callers[old_target].end(), call_site), callers[old_target].end());
			}

			transitions[call_site][target_method] = target_context;

			callers[target_context].push_back(call_site);
	} else {
				if(transitions.find(call_site) != transitions.end()) {
				for (auto e: transitions[call_site]) {
					Context<M, N, A> old_target = transitions[call_site][e.first];
					callers[old_target].erase(std::remove(callers[old_target].begin(), callers[old_target].end(), call_site), callers[old_target].end());
				}
			}
			default_call_sites.push_back(call_site);
	}
}

template<class M, class N, class A>
std::unordered_map<Context<M, N, A>, std::vector<CallSite<M, N, A>>> ContextTransitionTable<M, N, A>::getCallers(void) {
	return callers;
}

template<class M, class N, class A>
void ContextTransitionTable<M, N, A>::sayHello(void) {
	std::cout << "Hello World from ContextTransitionTable!" << '\n';
}

template<class M, class N, class A>
std::vector<CallSite<M, N, A>> ContextTransitionTable<M, N, A>::getCallers(Context<M, N, A> target) {
	return callers[target];
}

template<class M, class N, class A>
std::unordered_map<Context<M, N, A>, std::vector<CallSite<M, N, A>>> ContextTransitionTable<M, N, A>::getCallSitesOfContexts(void) {
	return call_sites_of_contexts;
}

template<class M, class N, class A>
std::vector<CallSite<M, N, A>> ContextTransitionTable<M, N, A>::getDefaultCallSites(void) {
	return default_call_sites;
}

template<class M, class N, class A>
std::unordered_map<M,Context<M, N, A>> ContextTransitionTable<M, N, A>::getTargets(CallSite<M, N, A> call_site) {
	return transitions[call_site];
}

template<class M, class N, class A>
std::unordered_map<CallSite<M, N, A>, std::unordered_map<M, Context<M, N, A>>> ContextTransitionTable<M, N, A>::getTransitions(void) {
	return transitions;
}

// template<class M, class N, class A>
// std::vector<Context<M, N, A>> ContextTransitionTable<M, N, A>::reachableSet(Context<M, N, A> source) {
// 		std::vector<Context<M, N, A>> reachable_contexts;
//
// 		std::stack<Context<M, N, A>> stack;
// 		stack.push(source);
//
// 		while (!stack.empty()) {
// 			source = stack.top();
// 			stack.pop();
// 			for(auto e1: call_sites_of_contexts[source]) {
// 				if (default_call_sites.find(e1) == default_call_sites.end()) {
// 					continue;
// 				}
// 				for(auto e2: transitions[e1]) {
// 					Context<M, N, A> target = e2.second;
// 					if(reachable_contexts.find(target) == reachable_contexts.end()) {
// 						reachable_contexts.insert(target);
// 						stack.push(target);
// 					}
// 				}
// 			}
// 		}
// 		return reachable_contexts;
// }

// Define the class here so that main file can use
template class ContextTransitionTable<llvm::Function*, llvm::BasicBlock*, Sign>;
