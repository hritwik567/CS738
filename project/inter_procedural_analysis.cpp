#include "inter_procedural_analysis.h"

template<class M, class N, class A>
std::unordered_set<CallSite<M, N, A>> InterProceduralAnalysis<M, N, A>::getCallers(Context<M, N, A> target) {
  return context_transitions.getCallers(target);
}

template<class M, class N, class A>
Context<M, N, A> InterProceduralAnalysis<M, N, A>::getContext(M method, A value) {
  if (contexts.find(method) == contexts.end()) {
    return *(new Context<M, N, A>());
  }
  if (reverse) {
    for(auto e: contexts[method]) {
      if (value == e.getExitValue()) {
        return e;
      }
    }
  } else {
    for(auto e: contexts[method]) {
      if (value == e.getEntryValue()) {
        return e;
      }
    }
  }
  return *(new Context<M, N, A>());
}

template<class M, class N, class A>
std::vector<Context<M, N, A>> InterProceduralAnalysis<M, N, A>::getContexts(M method) {
  if(contexts.find(method) == contexts.end()) {
    return std::vector<Context<M, N, A>>();
  }
}

template<class M, class N, class A>
ContextTransitionTable<M, N, A> InterProceduralAnalysis<M, N, A>::getContextTransitionTable() {
  return context_transitions;
}

template<class M, class N, class A>
std::unordered_set<M> InterProceduralAnalysis<M, N, A>::getMethods() {
  std::unordered_set<M> r;
  for(auto e: contexts) {
    r.insert(e.first);
  }
  return r;
}

template<class M, class N, class A>
std::unordered_map<M, Context<M, N, A>> InterProceduralAnalysis<M, N, A>::getTargets(CallSite<M, N, A> call_site) {
  return context_transitions.getTargets(call_site);
}
