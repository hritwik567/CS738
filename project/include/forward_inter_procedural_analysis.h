#ifndef _FORWARD_INTER_PROCEDURAL_ANALYSIS_H_
#define _FORWARD_INTER_PROCEDURAL_ANALYSIS_H_

#include "inter_procedural_analysis.h"

/*
<M> the type of a method
<N> the type of a node in the CFG
<A> the type of a data flow value
*/

template<class M, class N, class A>
class ForwardInterProceduralAnalysis: public InterProceduralAnalysis<M, N, A> {
  protected:
    Context<M, N, A> initContextForPhantomMethod(M method, A entry_value);
    Context<M, N, A> initContext(M method, A entry_value);

  public:
    ForwardInterProceduralAnalysis(): InterProceduralAnalysis<M, N, A>(false) { }

    void doAnalysis(void);

    virtual A boundaryValue(M entryPoint) = 0;
    virtual A copy(A src) = 0;
    virtual A meet(A op1, A op2) = 0;
    virtual A topValue() = 0;

    virtual A normalFlowFunction(Context<M, N, A> context, N node, A in_value) = 0;
    virtual A callEntryFlowFunction(Context<M,N,A> context, M target_method, N node, A in_value) = 0;
    virtual A callExitFlowFunction(Context<M,N,A> context, M target_method, N node, A exit_value) = 0;
    virtual A callLocalFlowFunction(Context<M,N,A> context, N node, A in_value) = 0;
};
#endif
