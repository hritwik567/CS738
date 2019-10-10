// Run using $c++ -g factorial.cpp `llvm-config --cxxflags --ldflags --libs core`
#include "llvm/ADT/APInt.h"
#include "llvm/IR/Verifier.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/MCJIT.h"
#include "llvm/IR/Argument.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include <algorithm>
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

using namespace llvm;

static void CreateFunctions(Module *M, LLVMContext &Context) {
  FunctionType *FactFTy = FunctionType::get(Type::getInt32Ty(Context),
                                           {Type::getInt32Ty(Context)}, false);
  Function *FactF =
      Function::Create(FactFTy, Function::ExternalLinkage, "fact", M);

  BasicBlock *BB = BasicBlock::Create(Context, "EntryBlock", FactF);

  Value *One = ConstantInt::get(Type::getInt32Ty(Context), 1);

  Argument *ArgN = &*FactF->arg_begin();
  ArgN->setName("n");

  BasicBlock *RetBB = BasicBlock::Create(Context, "return", FactF);

  BasicBlock* RecurseBB = BasicBlock::Create(Context, "recurse", FactF);

  Value *CondInst = new ICmpInst(*BB, ICmpInst::ICMP_SLE, ArgN, One, "cond");
  BranchInst::Create(RetBB, RecurseBB, CondInst, BB);

  ReturnInst::Create(Context, One, RetBB);

  Value *Sub = BinaryOperator::CreateSub(ArgN, One, "arg", RecurseBB);
  CallInst *CallFactX1 = CallInst::Create(FactF, Sub, "factx1", RecurseBB);
  CallFactX1->setTailCall();

  Value *Ret = BinaryOperator::CreateMul(ArgN, CallFactX1, "ret", RecurseBB);

  ReturnInst::Create(Context, Ret, RecurseBB);

  return;
}

int main(int argc, char **argv) {
  int n = argc > 1 ? atol(argv[1]) : 24;

  InitializeNativeTarget();
  InitializeNativeTargetAsmPrinter();
  LLVMContext Context;

  std::unique_ptr<Module> Owner(new Module("test", Context));
  Module *M = Owner.get();

  CreateFunctions(M, Context);

  errs() << "verifying... ";
  if (verifyModule(*M)) {
    errs() << argv[0] << ": Error constructing function!\n";
    return 1;
  }

  errs() << "OK\n";
  errs() << "We just constructed this LLVM module:\n\n---------\n" << *M;
  return 0;
}
