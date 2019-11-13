#ifndef _CONTEXT_TRANSITION_TABLE_H_
#define _CONTEXT_TRANSITION_TABLE_H_
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <iterator>
#include <memory>
#include <iostream>
#include <stack>

#include "context.h"

template<class M, class N, class A>
using CallSite = std::pair<Context<M, N, A>, N>;

namespace std {
	template<class M, class N, class A>
	struct hash<CallSite<M, N, A>> {
		size_t operator()(const CallSite<M, N, A>& c) const {
			return hash<Context<M, N, A>>()(c.first) ^ hash<N>()(c.second);
		}
	};
}

template<class M, class N, class A>
class ContextTransitionTable {
	protected:
		std::unordered_map<Context<M, N, A>, std::vector<CallSite<M, N, A>>> callers;
		std::unordered_map<CallSite<M, N, A>, std::unordered_map<M, Context<M, N, A>>> transitions;
		std::unordered_map<Context<M, N, A>, std::vector<CallSite<M, N, A>>> call_sites_of_contexts;
		std::vector<CallSite<M, N, A>> default_call_sites;

  public:
    ContextTransitionTable() { }

		void sayHello(void);
    void addTransition(CallSite<M, N, A> call_site, Context<M, N, A> target_context);
    std::unordered_map<Context<M, N, A>, std::vector<CallSite<M, N, A>>> getCallers(void);
    std::vector<CallSite<M, N, A>> getCallers(Context<M,N,A> target);
		std::unordered_map<Context<M, N, A>, std::vector<CallSite<M, N, A>>> getCallSitesOfContexts(void);
		std::vector<CallSite<M, N, A>> getDefaultCallSites(void);
		std::unordered_map<M, Context<M, N, A>> getTargets(CallSite<M, N, A> call_site);
		std::unordered_map<CallSite<M, N, A>, std::unordered_map<M, Context<M, N, A>>> getTransitions(void);
		// std::vector<Context<M, N, A>> reachableSet(Context<M, N, A> source);
};
#endif
