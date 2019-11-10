#include "dataflow.h"

using namespace llvm;

namespace {
  class CustomPass : public FunctionPass {
    public:
      static char ID;
    	CustomPass() : FunctionPass(ID) {}

    	bool runOnFunction(Function &F) override {
    	  errs() << F.getName() << "\n";
    		return false;
    	}
  };

  class CustomModulePass : public ModulePass {
    public:
      static char ID;
    	CustomModulePass() : ModulePass(ID) {}

    	bool runOnModule(Module &M) override {
    	  errs() << M.getName() << "\n";
        for(auto &F: M) {
          errs() << "Found global named: " << F.getName() << "\tType: " << *F.getType() << "\n";
          for(auto &BB: F) {
            errs() << "\t" << "In BB:" << BB.getName() << "\n";
            for(auto &I: BB) {
              Instruction* Insn = &I;
              errs() << "\t\t" << *Insn << "\n" << "\t\t ";
              for(auto &op: Insn->operands()) {
                errs() << op->getName() << " ";
              }
              errs() << "\n";
            }
          }
        }
        // errs() << M.getFunction("main")->getName() << "\n";
    		return false;
    	}
  };
}

char CustomPass::ID = 0;
char CustomModulePass::ID = 0;

static RegisterPass<CustomPass> X("c_pass", "Custom Pass", true, true);
static RegisterPass<CustomModulePass> Y("cm_pass", "Custom Module Pass", true, true);

static RegisterStandardPasses Y1(
    PassManagerBuilder::EP_EarlyAsPossible,
    [](const PassManagerBuilder &Builder,
       legacy::PassManagerBase &PM) { PM.add(new CustomModulePass()); });
