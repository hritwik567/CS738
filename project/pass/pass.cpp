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
            Value *LHS, *op, *op1, *op2;
            for(auto &I: BB) {
              Instruction* Insn = &I;
              errs() << "\t---------" << *Insn << " ------------\n" << "\t\t ";
              if (isa<CallInst>(*Insn)) {
                LHS = dyn_cast<Value>(Insn);
                CallInst* call = dyn_cast<CallInst>(Insn);
                Function* tar = call->getCalledFunction();
                Function::arg_iterator AI = tar->arg_begin();
                errs() << "Call " << LHS->getName() << " --  " << tar->getName() << " ";
                unsigned ii = call->getNumArgOperands();
                for(unsigned i=0;i<ii;i++) {
                  op = call->getArgOperand(i);
                  errs() << "Arg -- " <<  AI[i].getName() << " --- ";
                  if(ConstantInt *CI = dyn_cast<ConstantInt>(op)){
                    errs() << " -- " << CI->isNegative();
                  } else {
                    errs() << " -- " << op->getName();
                  }
                }
                errs() << "\n";
              } else if(isa<StoreInst>(*Insn)) {
                LHS = Insn->getOperand(1);
                op = Insn->getOperand(0);
                errs() << "Store " << std::stoi("0" + LHS->getName().substr(3).lower());
                if(ConstantInt *CI = dyn_cast<ConstantInt>(op)){
                  errs() << " -- " << CI->isNegative() << "\n";
                } else {
                  errs() << " -- " << op->getName() << "\n";
                }
              } else if(isa<LoadInst>(*Insn)){
                LHS = dyn_cast<Value>(Insn);
                op = Insn->getOperand(0);
                errs() << "Load " << LHS->getName();
                if(ConstantInt *CI = dyn_cast<ConstantInt>(op)){
                  errs() << " -- " << CI->isNegative() << "\n";
                } else {
                  errs() << " -- " << op->getName() << "\n";
                }

              } else if(isa<BinaryOperator>(*Insn)) {
                LHS = dyn_cast<Value>(Insn);
                op1 = Insn->getOperand(0);
                op2 = Insn->getOperand(1);
                errs() << "Binary " << Insn->getOpcodeName() << " " << LHS->getName();
                if(ConstantInt *CI = dyn_cast<ConstantInt>(op1)){
                  errs() << " -- " << CI->isNegative() << " ";
                } else {
                  errs() << " -- " << op1->getName() << " ";
                }

                if(ConstantInt *CI = dyn_cast<ConstantInt>(op2)){
                  errs() << "-- " << CI->isNegative() << "\n";
                } else {
                  errs() << "-- " << op2->getName() << "\n";
                }

              } else if(isa<ReturnInst>(*Insn)){
                LHS = Insn->getOperand(0);
                errs() << "Return ";
                if(ConstantInt *CI = dyn_cast<ConstantInt>(LHS)){
                  errs() << " -- " << CI->isNegative() << " " << CI->isZero() << "\n";
                } else {
                  errs() << " -- " << LHS->getName() << "\n";
                }
              } else {
                LHS = dyn_cast<Value>(Insn);
                errs() << "Others " << LHS->getName() << "\n";
              }
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
