#ifndef _FORWARD_INTER_PROCEDURAL_ANALYSIS_H_
#define _FORWARD_INTER_PROCEDURAL_ANALYSIS_H_

#include "inter_procedural_analysis.h"
#include <cassert>

/*
<M> the type of a method
<N> the type of a node in the CFG
<A> the type of a data flow value
*/

template<class M, class N, class A>
class ForwardInterProceduralAnalysis: public InterProceduralAnalysis<M, N, A> {
  protected:
    using InterProceduralAnalysis<M, N, A>::workList;
    using InterProceduralAnalysis<M, N, A>::contexts;
    using InterProceduralAnalysis<M, N, A>::context_transitions;
    using InterProceduralAnalysis<M, N, A>::getContextbyId;

    Context<M, N, A> initContextForPhantomMethod(M method, A entry_value);
    Context<M, N, A> initContext(M method, A entry_value);

  public:
    ForwardInterProceduralAnalysis(): InterProceduralAnalysis<M, N, A>(false) { }

    void doAnalysis(void);

    virtual A boundaryValue(M entryPoint) = 0;
    virtual A copy(A src) = 0;
    virtual A meet(A op1, A op2) = 0;
    virtual A topValue() = 0;
    virtual M getEntryMethod() = 0;
    virtual bool isEqual(A op1, A op2) = 0;
    virtual bool isReturnEqual(A op1, A op2) = 0;


    virtual A normalFlowFunction(std::reference_wrapper<Context<M, N, A>> context, N node, A in_value) = 0;
    virtual A callCustomFlowFunction(std::reference_wrapper<Context<M, N, A>> context, N node, A in_value) = 0;
};
#endif
