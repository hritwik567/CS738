#include "forward_inter_procedural_analysis.h"

template<class M, class N, class A>
Context<M, N, A> ForwardInterProceduralAnalysis<M, N, A>::initContextForPhantomMethod(M method, A entry_value) {
  Context<M, N, A> *context = new Context<M, N, A>(method, false);
  context->setEntryValue(entry_value);
  context->setExitValue(copy(entry_value));
  context->markAnalysed();
  DBG(llvm::errs() << "initContextForPhantomMethod: " << context->getId() << " Fname: " << method->getName() << "\n";)
  return *context;
}

template<class M, class N, class A>
Context<M, N, A> ForwardInterProceduralAnalysis<M, N, A>::initContext(M method, A entry_value) {
  Context<M, N, A> *context = new Context<M, N, A>(method, false);
  std::vector<N> cfg = context->getControlFlowGraph();
  for(int i=0; i<cfg.size(); i++) {
    context->setValueBefore(cfg[i], topValue()); // This is "in"
    context->setValueAfter(cfg[i], topValue()); // This is "out"
    // context->addToWorklist(cfg[i]);
  }
  context->addToWorklist(cfg[cfg.size() - 1]);

  context->setEntryValue(copy(entry_value));
  context->setValueBefore(context->getEntryNode(), copy(entry_value));
  context->setExitValue(topValue());

  contexts[method].push_back(std::ref(*context));

  workList.push_back(std::ref(*context));
  DBG(llvm::errs() << "initContext: " << context->getId() << " Fname: " << method->getName() << "\n";)

  return *context;
}

template<class M, class N, class A>
void ForwardInterProceduralAnalysis<M, N, A>::doAnalysis(void) {

  M e_method = getEntryMethod();
	initContext(e_method, boundaryValue(e_method));

	while(!workList.empty()) {
    // TODO: Will probably need to make this a pointer
		std::reference_wrapper<Context<M, N, A>> current_context = workList.back();

		if(current_context.get().isEmptyWorklist()) {
      current_context.get().markAnalysed();
      workList.pop_back();
			continue;
		}

		N node = current_context.get().getAndPopWorklist();

    if(node != NULL){
      DBG(llvm::errs() << "-------------In doAnalysis Node being processed: " << current_context.get().getId() << " "  << node->getParent()->getName() << ":" << node->getName() << "\n";)
    } else {
      DBG(llvm::errs() << "-------------In doAnalysis Node being processed: " << current_context.get().getId() << " " << current_context.get().getMethod()->getName() << ":NULL" << "\n";)
    }
    for(N &e: current_context.get().getWorklist()) {
      if(e == NULL) {
        DBG(llvm::errs() << current_context.get().getMethod()->getName() << ":NULL" << "\n";)
      } else {
        DBG(llvm::errs() << e->getParent()->getName() << ":" << e->getName() << "\n";)
      }
    }
    //TODO: Check this condition
		if(node != NULL) {
			std::vector<N> predecessors = current_context.get().getPredsOf(node);
			if(predecessors.size() != 0) {
				A in = topValue();
				for(int i=0; i<predecessors.size(); i++) {
					A predOut = current_context.get().getValueAfter(predecessors[i]);
					in = meet(in, predOut);
				}
				current_context.get().setValueBefore(node, in);
			}

			A prevOut = current_context.get().getValueAfter(node);

			A in = current_context.get().getValueBefore(node);

      A out;

			if(current_context.get().isCall(node)) {
        out = callCustomFlowFunction(current_context, node, in);
      } else {
        out = normalFlowFunction(current_context, node, in);
      }

			out = meet(out, prevOut);

			current_context.get().setValueAfter(node, out);

			if(isEqual(out, prevOut) == false) {
        std::vector<N> successors = current_context.get().getSuccsOf(node);
			  for(int i=0; i<successors.size(); i++) {
          DBG(llvm::errs() << "successors:" << successors[i]->getName() << "\n";)
          current_context.get().addToWorklist(successors[i]);
				}
			}

      std::vector<N> tails = current_context.get().getTails();
      for(N &e: tails) {
          DBG(llvm::errs() << "Tails:" << e->getParent()->getName() << ":" << e->getName() << "\n";)
      }
      if(current_context.get().getLastWorklist() != NULL){
        DBG(llvm::errs() << "getLastWorklist " << current_context.get().getLastWorklist()->getName() << "\n";)
      } else {
        DBG(llvm::errs() << "getLastWorklist NULL" << "\n";)
      }
      DBG(llvm::errs() << "Node " << node->getName() << "\n";)
      if(current_context.get().isEmptyWorklist()) {
  			if(std::find(tails.begin(), tails.end(), node) != tails.end()) {
          current_context.get().addToWorklist(NULL);
  			}
      }

    } else {

      for(N &e: current_context.get().getWorklist()) {
        if(e == NULL) {
          DBG(llvm::errs() << current_context.get().getMethod()->getName() << ":NULL" << "\n";)
        } else {
          DBG(llvm::errs() << e->getParent()->getName() << ":" << e->getName() << "\n";)
        }
      }

			assert(current_context.get().isEmptyWorklist());

			A exitValue = topValue();
      std::vector<N> tails = current_context.get().getTails();
			for(int i=0; i<tails.size(); i++) {
				A tailOut = current_context.get().getValueAfter(tails[i]);
				exitValue = meet(exitValue, tailOut);
			}

			current_context.get().setExitValue(exitValue);

			current_context.get().markAnalysed();
			std::vector<CallSite<M, N, A>> callers =  context_transitions.getCallers(current_context.get());
			if(!callers.empty()) {
        for(auto &call_site: callers) {
					std::reference_wrapper<Context<M, N, A>> calling_context = getContextbyId(call_site.second->getParent(), call_site.first.getId());
					N call_node = call_site.second;
          DBG(llvm::errs() << "call_site: call node " << call_site.first.getId() << ":"  << call_node->getParent()->getName() << ":" << call_node->getName() << "\n";)
          calling_context.get().addToWorklist(call_node);
					workList.push_back(calling_context);
				}
			}
		}
  }
}

// Define the class here so that main file can use
template class ForwardInterProceduralAnalysis<llvm::Function*, llvm::BasicBlock*, Sign>;
