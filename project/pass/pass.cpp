#include <algorithm>
#include <cassert>
#include <iterator>
#include <memory>
#include <utility>
#include <vector>
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/DenseSet.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/ADT/iterator_range.h"
#include "llvm/Analysis/ValueTracking.h"
#include "llvm/IR/Argument.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Use.h"
#include "llvm/IR/User.h"
#include "llvm/IR/Value.h"
#include "llvm/Pass.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Transforms/Utils/Local.h"

// DEBUG mode
#undef DEBUG
// #define DEBUG 1

#ifdef DEBUG
#define DBG(a) a
#else
#define DBG(a)
#endif

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
        for (auto &F : M){  
        errs() << "Found global named: " << F.getName() 
                  << "\tType: " << *F.getType() << "\n";
      }
        errs() << M.getFunction("main")->getName() << "\n";
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
