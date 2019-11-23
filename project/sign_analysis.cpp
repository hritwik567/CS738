#include "sign_analysis.h"

namespace llvm {

  std::pair<StringRef, SIGN> SignAnalysis::signOf(Instruction* Insn, Sign dfv) {
    // DBG(errs() << "SignOf " << *Insn << "\n";)
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

      if(std::strncmp(Insn->getOpcodeName(), "add", 3) == 0) {
        return std::make_pair(LHS->getName(), SIGN_plus(l, r));
      } else if(std::strncmp(Insn->getOpcodeName(), "mul", 3) == 0) {
        return std::make_pair(LHS->getName(), SIGN_mult(l, r));
      } else if(std::strncmp(Insn->getOpcodeName(), "sub", 3) == 0 and l == SIGN::ZERO) {
        return std::make_pair(LHS->getName(), SIGN_negate(r));
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
        if(dfv.find(LHS->getName()) != dfv.end()) {
          return std::make_pair(RETURN, dfv[LHS->getName()]);
        } else {
          assert(false);
        }
      }
    } else if(isa<BranchInst>(*Insn)) {
      return std::make_pair(BRANCH, SIGN::BOTTOM);
    } else if(isa<AllocaInst>(*Insn)) {
      LHS = dyn_cast<Value>(Insn);
      return std::make_pair(LHS->getName(), SIGN::TOP); 
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

    for(auto &x : op2) {
      if (op1.find(x.first) != op1.end()) {
        SIGN sign1 = x.second;
        SIGN sign2 = op1[x.first];
        result[x.first] = SIGN_meet(sign1, sign2);
      } else {
        result[x.first] = x.second;
      }
    }
    return result;
  }

  bool SignAnalysis::isEqual(Sign op1, Sign op2) {
    return op1 == op2;
  }

  bool SignAnalysis::isReturnEqual(Sign op1, Sign op2) {
    bool in_op1 = op1.find(RETURN) != op1.end();
    bool in_op2 = op2.find(RETURN) != op2.end();
    if(in_op1 and in_op2) return op1[RETURN] == op2[RETURN];
    return isEqual(op1, op2);
  }

  Function* SignAnalysis::getEntryMethod(void) {
    // DBG(errs() << "here" << "\n" << module->getFunction("main")->getName;)
    return module->getFunction("main");
  }

  Sign SignAnalysis::topValue() {
    return Sign();
  }

  std::vector<Function*> getMethods(Function* _method, BasicBlock* _node) {

  }

  Sign SignAnalysis::normalFlowFunction(std::reference_wrapper<Context<Function*, BasicBlock*, Sign>> context, BasicBlock* node, Sign in_value) {
    DBG(errs() << "In normalFlowFunction for function " << node->getParent()->getName()  << ":" << node->getName() << "\n";)
    Sign out_value(in_value);

    for(auto &I: *node) {
      Instruction* Insn = &I;
      std::pair<StringRef, SIGN> p = signOf(Insn, out_value);
      if(p.first != BRANCH)
        out_value[p.first] = p.second;
      for(auto &x : out_value) {
        DBG(errs() << x.first << " " << SIGN_toString(x.second) << "; ";)
      }
      DBG(errs() << "\n\n";)
    }

    DBG(errs() << "\nIn normalFlowFunction out_value: " << "\n";)
    for(auto &x : out_value) {
      DBG(errs() << "\t " << x.first << " " << SIGN_toString(x.second) << "\n";)
    }

    return out_value;
  }

  Sign SignAnalysis::callCustomFlowFunction(std::reference_wrapper<Context<Function*, BasicBlock*, Sign>> context, BasicBlock* node, Sign in_value) {
    DBG(errs() << "In callCustomFlowFunction for function " << node->getParent()->getName() << ":" << node->getName() << "\n";)
    Sign out_value(in_value);
    for(auto &I: *node) {
      Instruction* Insn = &I;
      if(isa<CallInst>(*Insn)) {
        CallInst* call = dyn_cast<CallInst>(Insn);
        Function* target_method = call->getCalledFunction();

        Sign entry_value = callEntryFlowFunction(context, target_method, Insn, out_value);

        // CallSite<Function*, BasicBlock*, Sign> call_site(context.get(), node);

        std::reference_wrapper<Context<Function*, BasicBlock*, Sign>> target_context = getContext(target_method, entry_value);
        if (target_context.get().is_null) {
          target_context.get() = initContext(target_method, entry_value);
          context.get().addToWorklist(node);
          context_transitions.addTransition(std::make_pair(context.get(), node), target_context.get());
          return context.get().getValueAfter(node);
        }

        // DBG(errs() << "Add transitions " << context.get().getId() << ":" << node->getParent()->getName() << ":" << node->getName())
        context_transitions.addTransition(std::make_pair(context.get(), node), target_context.get());
        // context_transitions.addTransition(call_site, target_context.get());

        // if(target_context.get().isAnalysed()) {
          Sign exit_value = target_context.get().getExitValue();
          Sign returned_value = callExitFlowFunction(context, target_method, Insn, exit_value);
          out_value = meet(out_value, returned_value);
        // } else {
          // context.get().addToWorklist(node);
          // return in_value;
          // Value* LHS = dyn_cast<Value>(Insn);
          // out_value[LHS->getName()] = SIGN::BOTTOM;
        // }
      } else {
        std::pair<StringRef, SIGN> p = signOf(Insn, out_value);
        if(p.first != BRANCH)
          out_value[p.first] = p.second;
      }
      // for(auto &x : out_value) {
      //   DBG(errs() << "\t " << x.first << " " << SIGN_toString(x.second) << "; ";)
      // }
    }

    DBG(errs() << "\nIn callCustomFlowFunction out_value: " << "\n";)
    for(auto &x : out_value) {
      DBG(errs() << "\t " << x.first << " " << SIGN_toString(x.second) << "\n";)
    }
    return out_value;
  }

  Sign SignAnalysis::callEntryFlowFunction(std::reference_wrapper<Context<Function*, BasicBlock*, Sign>> context, Function* target_method, Instruction* Insn, Sign in_value) {
    CallInst* call = dyn_cast<CallInst>(Insn);
    unsigned ii = call->getNumArgOperands();
    Sign entry_value;

    for(unsigned i=0;i<ii;i++) {
      Value* op = call->getArgOperand(i);
      Function::arg_iterator AI = target_method->arg_begin();
      if(ConstantInt *CI = dyn_cast<ConstantInt>(op)){
        if(CI->isZero()) {
          entry_value[AI[i].getName()] = SIGN::ZERO;
        } else if(CI->isNegative()) {
          entry_value[AI[i].getName()] = SIGN::NEGATIVE;
        } else {
          entry_value[AI[i].getName()] = SIGN::POSITIVE;
        }
      } else {
        if(in_value.find(op->getName()) != in_value.end()) {
          entry_value[AI[i].getName()] = in_value[op->getName()];
        } else {
          assert(false);
        }
      }
    }
    return entry_value;
  }

  Sign SignAnalysis::callExitFlowFunction(std::reference_wrapper<Context<Function*, BasicBlock*, Sign>> context, Function* target_method, Instruction* Insn, Sign exit_value) {
    Sign return_value;
    Value* LHS = dyn_cast<Value>(Insn);
    return_value[LHS->getName()] = exit_value[RETURN];
    DBG(errs() << "callExitFlowFunction " << SIGN_toString(exit_value[RETURN]) << "\n";)
    return return_value;
  }

  bool sort_func(StringRef a, StringRef b) {
    return std::stoi("0" + a.substr(3).lower()) < std::stoi("0" + b.substr(3).lower());
  }

  void SignAnalysis::printOutput(void) {
    for(auto &e: contexts) {
      errs() << "Function Name: " << e.first->getName() << "\n";
      for(int i=0; i<e.second.size(); i++) {
        Sign entry_value = e.second[i].get().getEntryValue();
        Sign exit_value = e.second[i].get().getExitValue();

        std::vector<StringRef> entry_keys;
        std::vector<StringRef> exit_keys;

        for(auto &x: entry_value) {
          entry_keys.push_back(x.first);
        }

        std::sort(entry_keys.begin(), entry_keys.end(), sort_func);

        for(auto &x: exit_value) {
          if(entry_value.find(x.first) == entry_value.end() and x.first != RETURN)
            exit_keys.push_back(x.first);
        }

        std::sort(exit_keys.begin(), exit_keys.end(), sort_func);

        errs() << " Context: " << e.second[i].get().getId() << "\n";
        errs() << "  Entry Value:\n";

        for(auto &x: entry_keys) {
          errs() << "   " << x << " " << SIGN_toString(entry_value[x]) << "\n";
        }

        errs() << "  Intermediate Values:\n";
        for(auto &x: exit_keys) {
          errs() << "   " << x << " " << SIGN_toString(exit_value[x]) << "\n";
        }

        errs() << "  Return Value:\n";
        if(exit_value.find(RETURN) != exit_value.end()) {
          errs() << "   " << RETURN << " " << SIGN_toString(exit_value[RETURN]) << "\n";
        }
        errs() << "\n";
      }
      errs() << "------------xxxxxxxxxxxx------------\n\n";
    }
  }

}
