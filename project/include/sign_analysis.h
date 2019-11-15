#ifndef _SIGN_ANALYSIS_H_
#define _SIGN_ANALYSIS_H_

#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/ADT/SmallSet.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/ValueMap.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/StringRef.h"
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
#include "llvm/Transforms/IPO/PassManagerBuilder.h"


#include <cstdio>
#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cassert>
#include <iterator>
#include <memory>
#include <utility>

#include "forward_inter_procedural_analysis.h"

namespace llvm {
  class SignAnalysis: public ForwardInterProceduralAnalysis<Function*, BasicBlock*, Sign> {
    public:
      Module* module;
      SignAnalysis(Module* module): module(module) {}

      Sign boundaryValue(Function* entryPoint);
      Sign copy(Sign src);
      Sign meet(Sign op1, Sign op2);
      Sign topValue();
      SIGN signOf(Value* value, Sign dfv);
      Function* getEntryMethod(void);
      bool isEqual(Sign op1, Sign op2);
      bool isReturnEqual(Sign op1, Sign op2);
      std::pair<StringRef, SIGN> signOf(Instruction* Insn, Sign dfv);

      Sign normalFlowFunction(std::reference_wrapper<Context<Function*, BasicBlock*, Sign>> context, BasicBlock* node, Sign in_value);
      Sign callEntryFlowFunction(std::reference_wrapper<Context<Function*, BasicBlock*, Sign>> context, Function* target_method, Instruction* Insn, Sign in_value);
      Sign callExitFlowFunction(std::reference_wrapper<Context<Function*, BasicBlock*, Sign>> context, Function* target_method, Instruction* Insn, Sign exit_value);
      Sign callCustomFlowFunction(std::reference_wrapper<Context<Function*, BasicBlock*, Sign>> context, BasicBlock* node, Sign in_value);
      void printOutput(void);
  };
}

#endif
