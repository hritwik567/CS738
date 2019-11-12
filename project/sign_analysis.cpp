#include "sign_analysis.h"

namespace llvm {

  std::pair<StringRef, SIGN> SignAnalysis::signOf(Instruction* Insn, Sign dfv) {
    Value *LHS, *op, *op1, *op2;
    if(isa<StoreInst>(*Insn)) {
      LHS = Insn->getOperand(1);
      op = Insn->getOperand(0);

      if(ConstantInt *CI = dyn_cast<ConstantInt>(op)){
        if(CI->isZero()) {
          return std::make_pair(LHS->getName(), SIGN::ZERO);
        } else if(CI->isNegative()) {
          return std::make_pair(LHS->getName(), SIGN::NEGATIVE);
        } else {
          return std::make_pair(LHS->getName(), SIGN::POSITIVE);
        }
      } else {
        if(dfv.find(op->getName()) != dfv.end()) {
          return std::make_pair(LHS->getName(), dfv[op->getName()]);
        } else {
          assert(false);
        }
      }

    } else if(isa<LoadInst>(*Insn)){
      LHS = dyn_cast<Value>(Insn);
      op = Insn->getOperand(0);
      if(ConstantInt *CI = dyn_cast<ConstantInt>(op)){
        if(CI->isZero()) {
          return std::make_pair(LHS->getName(), SIGN::ZERO);
        } else if(CI->isNegative()) {
          return std::make_pair(LHS->getName(), SIGN::NEGATIVE);
        } else {
          return std::make_pair(LHS->getName(), SIGN::POSITIVE);
        }
      } else {
        if(dfv.find(op->getName()) != dfv.end()) {
          return std::make_pair(LHS->getName(), dfv[op->getName()]);
        } else {
          assert(false);
        }
      }

    } else if(isa<BinaryOperator>(*Insn)) {
      LHS = dyn_cast<Value>(Insn);
      op1 = Insn->getOperand(0);
      op2 = Insn->getOperand(1);

      SIGN l, r;
      if(ConstantInt *CI = dyn_cast<ConstantInt>(op1)){
        if(CI->isZero()) {
          l = SIGN::ZERO;
        } else if(CI->isNegative()) {
          l = SIGN::NEGATIVE;
        } else {
          l = SIGN::POSITIVE;
        }
      } else {
        if(dfv.find(op1->getName()) != dfv.end()) {
          l = dfv[op1->getName()];
        } else {
          assert(false);
        }
      }

      if(ConstantInt *CI = dyn_cast<ConstantInt>(op2)){
        if(CI->isZero()) {
          r = SIGN::ZERO;
        } else if(CI->isNegative()) {
          r = SIGN::NEGATIVE;
        } else {
          r = SIGN::POSITIVE;
        }
      } else {
        if(dfv.find(op2->getName()) != dfv.end()) {
          r = dfv[op2->getName()];
        } else {
          assert(false);
        }
      }

      if(Insn->getOpcodeName() == "add") {
        return std::make_pair(LHS->getName(), plus(l, r));
      } else if(Insn->getOpcodeName() == "mul") {
        return std::make_pair(LHS->getName(), mult(l, r));
      } else if(Insn->getOpcodeName() == "sub" and l == SIGN::ZERO) {
        return std::make_pair(LHS->getName(), negate(r));
      } else {
        return std::make_pair(LHS->getName(), SIGN::BOTTOM);
      }

    } else if(isa<ReturnInst>(*Insn)) {
      LHS = Insn->getOperand(0);
      if(ConstantInt *CI = dyn_cast<ConstantInt>(LHS)){
        if(CI->isZero()) {
          return std::make_pair(RETURN, SIGN::ZERO);
        } else if(CI->isNegative()) {
          return std::make_pair(RETURN, SIGN::NEGATIVE);
        } else {
          return std::make_pair(RETURN, SIGN::POSITIVE);
        }
      } else {
        if(dfv.find(op->getName()) != dfv.end()) {
          return std::make_pair(RETURN, dfv[LHS->getName()]);
        } else {
          assert(false);
        }
      }
    } else {
      LHS = dyn_cast<Value>(Insn);
      return std::make_pair(LHS->getName(), SIGN::BOTTOM);
    }
  }

  Sign SignAnalysis::boundaryValue(Function* entryPoint) {
    return topValue();
  }

  Sign SignAnalysis::copy(Sign src) {
    return Sign(src);
  }

  Sign SignAnalysis::meet(Sign op1, Sign op2) {
    Sign result(op1);

    for (auto &x : op2) {
      if (op1.find(x.first) != op1.end()) {
        SIGN sign1 = x.second;
        SIGN sign2 = op2[x.first];
        result[x] = meet(sign1, sign2);
      } else {
        result[x] = op2[x.first];
      }
    }
    return result;
  }

  Sign SignAnalysis::isEqual(Sign op1, Sign op2) {
    return op1 == op2;
  }

  Function* getEntryMethod(void) {
    return M->getFunction("main");
  }

  Sign SignAnalysis::topValue() {
    return Sign();
  }

  std::vector<Function*> getMethods(Function* _method, BasicBlock* _node) {

  }

  Sign SignAnalysis::normalFlowFunction(std::reference_wrapper<Context<Function*, BasicBlock*, Sign>> context, BasicBlock* node, Sign in_value) {
    Sign out_value(in_value);

    for(auto &I: node) {
      Instruction* Insn = &I;
      std::pair<StringRef, SIGN> p = signOf(Insn, out_value);
      out_value[p.first] = p.second;
    }

    return out_value;
  }

  Sign SignAnalysis::callCustomFlowFunction(std::reference_wrapper<Context<Function*, BasicBlock*, Sign>> context, BasicBlock* node, Sign in_value) {
    Sign out_value(in_value);

    for(auto &I: node) {
      Instruction* Insn = &I;
      if(isa<CallInst>(*Insn)) {

        Function* targetMethod = ; // TODO

        Sign entryValue = callEntryFlowFunction(currentContext, targetMethod, node, in);

        CallSite<Function*, BasicBlock*, Sign> callSite(currentContext.get(), node);

        Context<Function*, BasicBlock*, Sign> targetContext = getContext(targetMethod, entryValue);
        if (targetContext.get().is_null) {
          targetContext = initContext(targetMethod, entryValue);
        }

        context_transitions.addTransition(callSite, targetContext.get());

        if(targetContext.get().isAnalysed()) {
          Sign exitValue = targetContext.get().getExitValue();
          Sign returnedValue = callExitFlowFunction(currentContext, targetMethod, node, exitValue);
          out_value = meet(out_value, returnedValue);
        } else {
          Value* LHS = dyn_cast<Value>(Insn);
          out_value[LHS->getName()] = SIGN::BOTTOM;
        }
      } else {
        std::pair<StringRef, SIGN> p = signOf(Insn, out_value);
        out_value[p.first] = p.second;
      }
    }

    return out_value;
  }

  Sign SignAnalysis::callEntryFlowFunction(std::reference_wrapper<Context<Function*, BasicBlock*, Sign>> context, Function* target_method, BasicBlock* node, Sign in_value) {

  }

  Sign SignAnalysis::callExitFlowFunction(std::reference_wrapper<Context<Function*, BasicBlock*, Sign>> context, Function* target_method, BasicBlock* node, Sign exit_value) {

  }

}
