#include <algorithm>
#include <cassert>
#include <iterator>
#include <memory>
#include <utility>
#include <vector>

/*
<M> the type of a method
<N> the type of a node in the CFG
<A> the type of a data flow value
*/

template<class M, class N, class A>
class InterProceduralAnalysis {
  protected:
    std::vector<Context<M, N, A>> workList;
    std::map<M, std::vector<Context<M, N, A>>> contexts;
    ContextTransitionTable<M, N, A> contextTransitions;
    bool reverse;

  public:
    InterProceduralAnalysis(bool reverse): reverse(reverse) {}

    virtual A boundaryValue(M entryPoint) = 0;
    virtual A copy(A src) = 0;
    virtual void doAnalysis(void) = 0;
    virtual A meet(A op1, A op2) = 0;
    vitual A topValue() = 0;

    std::set<CallSite<M, N, A>> getCallers(Context<M,N,A> target);
    Context<M, N, A> getContext(M method, A value);
    std::vector<Context<M, N, A>> getContexts(M method);
    ContextTransitionTable<M, N, A> getContextTransitionTable();
    std::set<M> getMethods();
    std::map<M, Context<M, N, A>> getTargets(CallSite<M, N, A> callSite);
};
