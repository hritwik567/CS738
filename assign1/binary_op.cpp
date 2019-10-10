// Run using $c++ -g binary_op.cpp `llvm-config --cxxflags --ldflags --libs core`
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
  FunctionType *BinFTy = FunctionType::get(Type::getInt32Ty(Context),
                                           {Type::getInt32Ty(Context),
                              					   Type::getInt32Ty(Context),
                              					   Type::getInt32Ty(Context)}, false);
  Function *BinF =
      Function::Create(BinFTy, Function::ExternalLinkage, "bin", M);

  BasicBlock *BB = BasicBlock::Create(Context, "EntryBlock", BinF);

  Value *One = ConstantInt::get(Type::getInt32Ty(Context), 1);

  Argument *ArgX = &*BinF->arg_begin()+0;
  ArgX->setName("x");
  Argument *ArgY = &*BinF->arg_begin()+1;
  ArgY->setName("y");
  Argument *ArgZ = &*BinF->arg_begin()+2;
  ArgZ->setName("z");

  Value *Ret1 = BinaryOperator::CreateMul(ArgX, ArgY, "ret1", BB);
  Value *Ret2 = BinaryOperator::CreateAdd(Ret1, ArgZ, "ret2", BB);

  ReturnInst::Create(Context, Ret2, BB);

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
