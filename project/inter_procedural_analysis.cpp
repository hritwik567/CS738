#include "inter_procedural_analysis.h"

template<class M, class N, class A>
std::vector<CallSite<M, N, A>> InterProceduralAnalysis<M, N, A>::getCallers(Context<M, N, A> target) {
  return context_transitions.getCallers(target);
}

template<class M, class N, class A>
std::reference_wrapper<Context<M, N, A>> InterProceduralAnalysis<M, N, A>::getContext(M method, A value) {
  if (contexts.find(method) == contexts.end()) {
    return std::ref(*(new Context<M, N, A>()));
  }
  if (reverse) {
    for(auto e: contexts[method]) {
      if(isEqual(value, e.get().getExitValue())) {
        return e;
      }
    }
  } else {
    for(auto e: contexts[method]) {
      if(isEqual(value, e.get().getEntryValue())) {
        return e;
      }
    }
  }
  return std::ref(*(new Context<M, N, A>()));
}

template<class M, class N, class A>
std::reference_wrapper<Context<M, N, A>> InterProceduralAnalysis<M, N, A>::getContextbyId(M method, int id) {
  for(auto e: contexts[method]) {
    if(id == e.get().getId()) {
      return e;
    }
  }
}

template<class M, class N, class A>
std::vector<std::reference_wrapper<Context<M, N, A>>> InterProceduralAnalysis<M, N, A>::getContexts(M method) {
  if(contexts.find(method) == contexts.end()) {
    return std::vector<std::reference_wrapper<Context<M, N, A>>>();
  }
  return contexts[method];
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

// Define the class here so that main file can use
template class InterProceduralAnalysis<llvm::Function*, llvm::BasicBlock*, Sign>;
