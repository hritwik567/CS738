#ifndef _INTER_PROCEDURAL_ANALYSIS_H_
#define _INTER_PROCEDURAL_ANALYSIS_H_
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <iterator>
#include <memory>
#include <iostream>

#include "context_transition_table.h"

/*
<M> the type of a method
<N> the type of a node in the CFG
<A> the type of a data flow value
*/

template<class M, class N, class A>
class InterProceduralAnalysis {
  protected:
    std::vector<Context<M, N, A>> workList;
    std::unordered_map<M, std::vector<Context<M, N, A>>> contexts;
    ContextTransitionTable<M, N, A> context_transitions;
    bool reverse;

  public:
    InterProceduralAnalysis(bool reverse): reverse(reverse) { }

    virtual A boundaryValue(M entryPoint) = 0;
    virtual A copy(A src) = 0;
    virtual void doAnalysis(void) = 0;
    virtual A meet(A op1, A op2) = 0;
    virtual A topValue() = 0;

    std::unordered_set<CallSite<M, N, A>> getCallers(Context<M,N,A> target);
    Context<M, N, A> getContext(M method, A value);
    std::vector<Context<M, N, A>> getContexts(M method);
    ContextTransitionTable<M, N, A> getContextTransitionTable();
    std::unordered_set<M> getMethods();
    std::unordered_map<M, Context<M, N, A>> getTargets(CallSite<M, N, A> call_site);
};
#endif
