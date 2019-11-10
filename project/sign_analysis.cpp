#include "sign_analysis.h"

namespace llvm {

  SIGN SignAnalysis::signOf(Value* value, Sign dfv) {
    
  }

  Sign SignAnalysis::boundaryValue(Function* entryPoint) {
    return topValue();
  }

  Sign SignAnalysis::copy(Sign src) {
    return Sign(src);
  }

  Sign SignAnalysis::meet(Sign op1, Sign op2) {

  }

  Sign SignAnalysis::topValue() {
    return Sign();
  }


  Sign SignAnalysis::normalFlowFunction(Context<Function*, BasicBlock*, Sign> context, BasicBlock* node, Sign in_value) {

  }

  Sign SignAnalysis::callEntryFlowFunction(Context<Function*, BasicBlock*, Sign> context, Function* target_method, BasicBlock* node, Sign in_value) {

  }

  Sign SignAnalysis::callExitFlowFunction(Context<Function*, BasicBlock*, Sign> context, Function* target_method, BasicBlock* node, Sign exit_value) {

  }

  Sign SignAnalysis::callLocalFlowFunction(Context<Function*, BasicBlock*, Sign> context, BasicBlock* node, Sign in_value) {

  }

}
