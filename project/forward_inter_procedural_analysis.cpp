#include "forward_inter_procedural_analysis.h"

template<class M, class N, class A>
Context<M, N, A> ForwardInterProceduralAnalysis<M, N, A>::initContextForPhantomMethod(M method, A entry_value) {
  Context<M, N, A> *context = new Context<M, N, A>(method);
  context.setEntryValue(entry_value);
  context.setExitValue(copy(entry_value));
  context.markAnalysed();
  return *context;
}

template<class M, class N, class A>
Context<M, N, A> ForwardInterProceduralAnalysis<M, N, A>::initContext(M method, A entry_value) {
  Context<M, N, A> *context = new Context<M, N, A>(method, false);

  for (N unit : context.getControlFlowGraph()) {
    context.setValueBefore(unit, topValue()); // This is "in"
    context.setValueAfter(unit, topValue()); // This is "out"
    context.getWorkList().add(unit);
  }

  context.setEntryValue(copy(entry_value));
  context.setValueBefore(context.getEntryNode(), copy(entry_value));
  context.setExitValue(topValue());

  contexts[method].push_back(*context);

  worklist.push_back(*context);

  return *context;
}

template<class M, class N, class A>
void ForwardInterProceduralAnalysis<M, N, A>::doAnalysis(void) {

}
