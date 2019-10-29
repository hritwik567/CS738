#include "forward_inter_procedural_analysis.h"

template<class M, class N, class A>
Context<M, N, A> ForwardInterProceduralAnalysis<M, N, A>::initContextForPhantomMethod(M method, A entry_value) {
  Context<M, N, A>* context = new Context<M, N, A>(method);
  context.setEntryValue(entry_value);
  context.setExitValue(copy(entry_value));
  context.markAnalysed();
  return *context;
}

template<class M, class N, class A>
Context<M, N, A> ForwardInterProceduralAnalysis<M, N, A>::initContext(M method, A entry_value) {
  return Context<M, N, A>(method);
}

template<class M, class N, class A>
void ForwardInterProceduralAnalysis<M, N, A>::doAnalysis(void) {

}