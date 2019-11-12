#include "sign_analysis.h"

using namespace llvm;

namespace {

  class CustomModulePass : public ModulePass {
    public:
      static char ID;
    	CustomModulePass() : ModulePass(ID) {}

    	bool runOnModule(Module &M) override {
				SignAnalysis S(&M);
				S.doAnalysis();
    		return false;
    	}
  };
}

char CustomModulePass::ID = 0;

static RegisterPass<CustomModulePass> Y("cm_pass", "Custom Module Pass", true, true);

static RegisterStandardPasses Y1(
    PassManagerBuilder::EP_EarlyAsPossible,
    [](const PassManagerBuilder &Builder,
       legacy::PassManagerBase &PM) { PM.add(new CustomModulePass()); });
