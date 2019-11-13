#ifndef _SIGN_H_
#define _SIGN_H_

#include <string>
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/StringRef.h"

#define RETURN "return"
#define BRANCH "branch"

enum SIGN {
  TOP,
  BOTTOM,
  ZERO,
  POSITIVE,
  NEGATIVE,
  INVALID
};

typedef llvm::DenseMap<llvm::StringRef, SIGN> Sign;

SIGN SIGN_meet(SIGN l, SIGN r);

SIGN SIGN_plus(SIGN l, SIGN r);

SIGN SIGN_mult(SIGN l, SIGN r);

SIGN SIGN_negate(SIGN s);

std::string SIGN_toString(SIGN s);

#endif
