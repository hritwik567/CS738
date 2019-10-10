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
  typedef DenseSet<Value*> LiveVariable;
  class LiveAnalysis {
    private:
  	   DenseMap<Instruction*, LiveVariable> In, Out, OutKill;
       LiveVariable OldIn;

    public:
    	bool run(Function &F) {
        std::vector<BasicBlock*> workList;
        bool change = true;

        while(change) {
          change = false;
          for(BasicBlock* BB : depth_first(&F.getEntryBlock())) {
            workList.push_back(BB);
          }

          while(!workList.empty()) {
      			BasicBlock* BB = workList.back();
            DBG(outs() << "Current BB [IMP]: " << BB->getName() << '\n');
            workList.pop_back();

            //Out = UNION In(S)
      			for(BasicBlock* Succ: successors(BB)) {
      				for(Value* V: In[&(*(Succ->begin()))]) {
      					Out[BB->getTerminator()].insert(V); //For the last instruction of BB
      				}
      			}

            for(Value* V: OutKill[BB->getTerminator()]) {
                Out[BB->getTerminator()].erase(V);
            }

            //OldIn
            auto FI = BB->begin();
      			Instruction* FInsn = &(*FI);
    				for(Value* V: In[FInsn]) {
              OldIn.insert(V);
            }

      			for(auto I = BB->rbegin(); I != BB->rend(); I++) {
      				Instruction* Insn = &(*I);
      				DBG(outs() << "\tFor instruction " << *Insn << "\n");

              LiveVariable gen, kill;
              //Out = In of previous instruction
      				if(I != BB->rbegin()) {
      					auto temp_I = I;
      					temp_I--;
      					Out[Insn] = In[&*temp_I];
      				}

              //Calculating gen and kill
              Value* LHSVal;
              if(isa<StoreInst>(*Insn)) {
      					LHSVal = Insn->getOperand(1);
      					kill.insert(LHSVal);

                Value* op = Insn->getOperand(0);
                if(ConstantInt *CI = dyn_cast<ConstantInt>(op));
                else gen.insert(op);
      				} else {
      					LHSVal = dyn_cast<Value>(Insn);
      					kill.insert(LHSVal);

                for(auto &op: Insn->operands()) {
      						if(isa<Instruction>(&op) || isa<Argument>(&op)) gen.insert(op);
                }

                if(PHINode* phi_insn = dyn_cast<PHINode>(Insn)) {
                  for(BasicBlock* Pred: predecessors(BB)) {
                    for(auto &op: Insn->operands()) {
          						if(isa<Instruction>(&op) || isa<Argument>(&op)) OutKill[Pred->getTerminator()].insert(op);
                    }
                  }
                  for(int i = 0; i < phi_insn->getNumIncomingValues(); i++) {
                    Value* op = phi_insn->getIncomingValue(i);
                    if(isa<Instruction>(op) || isa<Argument>(op)) {
                      BasicBlock* Pred = phi_insn->getIncomingBlock(i);
                      OutKill[Pred->getTerminator()].erase(op);
                    }
                  }
                }
              }

              DBG(outs() << "\t\t LHSVal: " << LHSVal->getName());
              //In = Out - kill
              In[Insn] = Out[Insn];
              if(!kill.empty()) {
      					DBG(outs() << "\t\t Kill set: ");
      					for(auto V: kill) {
      						DBG(outs() << V->getName() << " ");
      						In[Insn].erase(V);
      					}
      				}

              //In = In UNION gen
              if(!gen.empty()) {
      					DBG(outs() << "\t\t Gen set: ");
      					for(auto V: gen) {
      						DBG(outs() << V->getName() << " ");
      						In[Insn].insert(V);
      					}
      				}
              DBG(outs() << '\n');
      				kill.clear();
      				gen.clear();
      			}

    				for(Value* V: In[FInsn]) {
    					if(OldIn.count(V) == 0) {
    						change = true;
    						break;
    					}
    				}
            OldIn.clear();
      		}
        }

        outs() << "1) Live values out of each Basic Block\n\n";
        outs() << "Basic Block \t:\t Live Values\n";
        outs() << "--------------------------------------\n";
        for(Function::iterator b = F.begin(); b != F.end(); b++) {
  				BasicBlock* BB = &*b;
          outs() << BB->getName() << " \t\t:\t ";
          for(auto V: Out[BB->getTerminator()]) {
            outs() << V->getName() << " ";
          }
          outs() << '\n';
        }
        outs() << '\n';

        std::map<int, int> hist;
        int i1 = 0, i2 = 0;
        outs() << "2) Live values at each program point in each Basic Block\n\n";
        outs() << "Program Point \t:\t Live Values\n";
        outs() << "--------------------------------------\n";
  			for(Function::iterator b = F.begin(); b != F.end(); b++) {
  				BasicBlock* BB = &*b;
          i2 = 0;
  				for(auto i = BB->begin(); i != BB->end(); i++) {
  					Instruction* I = &*i;
            hist[Out[I].size()]++;
  					outs() << i1 << " (" << BB->getName() << '.' << i2 << ')' << " \t:\t ";
  					for(auto V: Out[I]) {
  						outs() << V->getName() << " ";
  					}
            DBG(outs() << '\t' << *I);
  					outs() << '\n';
            i1++;i2++;
  				}
        }
        outs() << '\n';

        outs() << "3) Histogram\n\n";
        outs() << "#Live Values \t:\t #Program Points\n";
        for(auto it: hist) {
          outs() << it.first << " \t\t:\t " << it.second << '\n';
        }
    		return false;
    	}
  };

  class LVAPass : public FunctionPass {
    public:
      static char ID;
    	LVAPass() : FunctionPass(ID) {}

    	bool runOnFunction(Function &F) override {
    		LiveAnalysis L;
    		L.run(F);
    		return false;
    	}
  };
}

char LVAPass::ID = 0;

static RegisterPass<LVAPass> X("lva", "Live Variable Analysis", true, true);
