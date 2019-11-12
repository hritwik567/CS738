#include "forward_inter_procedural_analysis.h"

template<class M, class N, class A>
Context<M, N, A> ForwardInterProceduralAnalysis<M, N, A>::initContextForPhantomMethod(M method, A entry_value) {
  Context<M, N, A> *context = new Context<M, N, A>(method);
  context->setEntryValue(entry_value);
  context->setExitValue(copy(entry_value));
  context->markAnalysed();
  return *context;
}

template<class M, class N, class A>
Context<M, N, A> ForwardInterProceduralAnalysis<M, N, A>::initContext(M method, A entry_value) {
  Context<M, N, A> *context = new Context<M, N, A>(method, false);
  std::vector<N> cfg = context->getControlFlowGraph();
  for(int i=0; i<cfg.size(); i++) {
    context->setValueBefore(cfg[i], topValue()); // This is "in"
    context->setValueAfter(cfg[i], topValue()); // This is "out"
    context->addToWorklist(cfg[i]);
  }

  context->setEntryValue(copy(entry_value));
  context->setValueBefore(context->getEntryNode(), copy(entry_value));
  context->setExitValue(topValue());

  contexts[method].push_back(std::ref(*context));

  workList.push_back(std::ref(*context));

  return *context;
}

template<class M, class N, class A>
void ForwardInterProceduralAnalysis<M, N, A>::doAnalysis(void) {

  M e_method = getEntryMethod();
	initContext(e_method, boundaryValue(e_method));

	while(!workList.empty()) {
    // TODO: Will probably need to make this a pointer
		std::reference_wrapper<Context<M, N, A>> currentContext = workList.back();

		if(currentContext.get().isEmptyWorklist()) {
      currentContext.get().markAnalysed();
      workList.pop_back();
			continue;
		}

		N node = currentContext.get().getAndPopWorklist();

    //TODO: Check this condition
		if(node != NULL) {
			std::vector<N> predecessors = currentContext.get().getPredsOf(node);
			if(predecessors.size() != 0) {
				A in = topValue();
				for(int i=0; i<predecessors.size(); i++) {
					A predOut = currentContext.get().getValueAfter(predecessors[i]);
					in = meet(in, predOut);
				}
				currentContext.get().setValueBefore(node, in);
			}

			A prevOut = currentContext.get().getValueAfter(node);

			A in = currentContext.get().getValueBefore(node);

      A out;

			if(currentContext.get().isCall(node)) {
        out = callCustomFlowFunction(currentContext, node, in);
      } else {
        out = normalFlowFunction(currentContext, node, in);
      }

			out = meet(out, prevOut);

			currentContext.get().setValueAfter(node, out);

			if(isEqual(out, prevOut) == false) {
        std::vector<N> successors = currentContext.get().getSuccsOf(node);
			  for(int i=0; i<successors.size(); i++) {
          currentContext.get().addToWorklist(successors[i]);
				}
			}

      std::vector<N> tails = currentContext.get().getTails();

      // TODO: What if there are mutiple exit nodes and we are pushing this last
			if(tails.find(node) != tails.end()) {
        currentContext.get().addToWorklist(NULL);
			}

    } else {
			assert(currentContext.get().isEmptyWorklist());

			A exitValue = topValue();
      std::vector<N> tails = currentContext.get().getTails();
			for(int i=0; i<tails.size(); i++) {
				A tailOut = currentContext.get().getValueAfter(tails[i]);
				exitValue = meet(exitValue, tailOut);
			}

			currentContext.get().setExitValue(exitValue);

			currentContext.get().markAnalysed();
			std::unordered_set<CallSite<M, N, A>> callers =  context_transitions.getCallers(currentContext.get());
			if(!callers.empty()) {
        for(auto &callSite: callers) {
					Context<M, N, A> callingContext = callSite.first;
					N callNode = callSite.second;

          callingContext.addToWorklist(callNode);
					workList.push_back(std::ref(callingContext));
				}
			}
		}
  }
}

// Define the class here so that main file can use
template class Context<llvm::Function*, llvm::BasicBlock*, Sign>;
