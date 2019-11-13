#include "sign_analysis.h"

using namespace llvm;

namespace {

  class SignAnalysisModulePass : public ModulePass {
    public:
      static char ID;
    	SignAnalysisModulePass() : ModulePass(ID) {}

    	bool runOnModule(Module &M) override {
				SignAnalysis S(&M);
				S.doAnalysis();
        S.printOutput();
    		return false;
    	}
  };
}

char SignAnalysisModulePass::ID = 0;

static RegisterPass<SignAnalysisModulePass> Y("sign_analysis", "Sign Analysis Module Pass", true, true);

static RegisterStandardPasses Y1(
    PassManagerBuilder::EP_EarlyAsPossible,
    [](const PassManagerBuilder &Builder,
       legacy::PassManagerBase &PM) { PM.add(new SignAnalysisModulePass()); });
