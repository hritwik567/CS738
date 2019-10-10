// Run using $c++ -g odd_even.cpp `llvm-config --cxxflags --ldflags --libs core`
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
  FunctionType *EvenFTy = FunctionType::get(Type::getInt32Ty(Context),
                                           {Type::getInt32Ty(Context)}, false);
  FunctionType *OddFTy = FunctionType::get(Type::getInt32Ty(Context),
                                           {Type::getInt32Ty(Context)}, false);
  Function *EvenF =
      Function::Create(EvenFTy, Function::ExternalLinkage, "even", M);
  Function *OddF =
      Function::Create(OddFTy, Function::ExternalLinkage, "odd", M);

  BasicBlock *BB1 = BasicBlock::Create(Context, "EntryBlock_even", EvenF);
  BasicBlock *BB2 = BasicBlock::Create(Context, "EntryBlock_odd", OddF);

  Value *Zero = ConstantInt::get(Type::getInt32Ty(Context), 0);
  Value *One = ConstantInt::get(Type::getInt32Ty(Context), 1);

  Argument *ArgE = &*EvenF->arg_begin();
  ArgE->setName("num");

  Argument *ArgO = &*OddF->arg_begin();
  ArgO->setName("num");

  BasicBlock *RetBB1 = BasicBlock::Create(Context, "return_even", EvenF);
  BasicBlock *RetBB2 = BasicBlock::Create(Context, "return_odd", OddF);

  BasicBlock* RecurseBB1 = BasicBlock::Create(Context, "recurse_even", EvenF);
  BasicBlock* RecurseBB2 = BasicBlock::Create(Context, "recurse_odd", OddF);

  Value *CondInst1 = new ICmpInst(*BB1, ICmpInst::ICMP_EQ, ArgE, Zero, "cond_even");
  BranchInst::Create(RetBB1, RecurseBB1, CondInst1, BB1);

  Value *CondInst2 = new ICmpInst(*BB2, ICmpInst::ICMP_EQ, ArgO, Zero, "cond_odd");
  BranchInst::Create(RetBB2, RecurseBB2, CondInst2, BB2);

  ReturnInst::Create(Context, One, RetBB1);
  ReturnInst::Create(Context, Zero, RetBB2);

  Value *Sub1 = BinaryOperator::CreateSub(ArgE, One, "arg_even", RecurseBB1);
  CallInst *CallOddX1 = CallInst::Create(OddF, Sub1, "oddx1", RecurseBB1);
  CallOddX1->setTailCall();

  Value *Sub2 = BinaryOperator::CreateSub(ArgO, One, "arg_odd", RecurseBB2);
  CallInst *CallEvenX1 = CallInst::Create(EvenF, Sub2, "evenx1", RecurseBB2);
  CallEvenX1->setTailCall();

  ReturnInst::Create(Context, CallOddX1, RecurseBB1);
  ReturnInst::Create(Context, CallEvenX1, RecurseBB2);

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
