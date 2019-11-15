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

    // if(current_context.get().getLastWorklist() != NULL){
    //   llvm::errs() << "doAnalysis Node being processed: " << current_context.get().getId() << " "  << current_context.get().getLastWorklist()->getParent()->getName() << ":" << current_context.get().getLastWorklist()->getName() << "\n";
    // } else {
    //   llvm::errs() << "doAnalysis Node being processed: " << current_context.get().getId() << " " << current_context.get().getMethod()->getName() << ":NULL" << "\n";
    // }

		if(current_context.get().isEmptyWorklist()) {
      current_context.get().markAnalysed();
      workList.pop_back();
			continue;
		}

		N node = current_context.get().getAndPopWorklist();

    if(node != NULL){
      llvm::errs() << "---------------------doAnalysis Node being processed: " << current_context.get().getId() << " "  << node->getParent()->getName() << ":" << node->getName() << "\n";
    } else {
      llvm::errs() << "---------------------doAnalysis Node being processed: " << current_context.get().getId() << " " << current_context.get().getMethod()->getName() << ":NULL" << "\n";
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

      // DBG(llvm::errs() << "out value before meet:" << "\n";)
      // for(auto &x : out) {
      //   DBG(llvm::errs() << "\t " << x.first << " " << SIGN_toString(x.second) << "\n";)
      // }

			out = meet(out, prevOut);

      DBG(llvm::errs() << "setting afterValue value:" << "\n";)
      for(auto &x : out) {
        DBG(llvm::errs() << "\t " << x.first << " " << SIGN_toString(x.second) << "\n";)
      }

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

			A exit_value = topValue();
      std::vector<N> tails = current_context.get().getTails();
			for(int i=0; i<tails.size(); i++) {
        DBG(llvm::errs() << "setting exit value:" << tails[i]->getName() << "\n";)
				A tail_out = current_context.get().getValueAfter(tails[i]);
        for(auto &x : tail_out) {
          DBG(llvm::errs() << "\t " << x.first << " " << SIGN_toString(x.second) << "\n";)
        }
				exit_value = meet(exit_value, tail_out);
			}

      A prev_exit_value = current_context.get().getExitValue();

      DBG(llvm::errs() << "get exit value:" << "\n";)
      for(auto &x : current_context.get().getExitValue()) {
        DBG(llvm::errs() << "\t " << x.first << " " << SIGN_toString(x.second) << "\n";)
      }
      DBG(llvm::errs() << "set exit value:"<< "\n";)
      for(auto &x : exit_value) {
        DBG(llvm::errs() << "\t " << x.first << " " << SIGN_toString(x.second) << "\n";)
      }

			current_context.get().setExitValue(exit_value);

			current_context.get().markAnalysed();
			std::vector<CallSite<M, N, A>> callers =  context_transitions.getCallers(current_context.get());
			if(!callers.empty() and !isReturnEqual(prev_exit_value, exit_value)) {
        for(auto &call_site: callers) {
					std::reference_wrapper<Context<M, N, A>> calling_context = getContextbyId(call_site.second->getParent(), call_site.first.getId());
					N call_node = call_site.second;
          DBG(llvm::errs() << "call_site: call node " << call_site.first.getId() << ":"  << call_node->getParent()->getName() << ":" << call_node->getName() << "\n";)
          if(calling_context.get().getLastWorklist() == NULL)
            calling_context.get().addToWorklist(call_node);
					workList.push_back(calling_context);
				}
			}
		}
  }
}

// Define the class here so that main file can use
template class ForwardInterProceduralAnalysis<llvm::Function*, llvm::BasicBlock*, Sign>;
