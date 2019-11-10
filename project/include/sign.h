#ifndef _SIGN_H_
#define _SIGN_H_
#include<string>

enum SIGN {
  TOP,
  BOTTOM,
  ZERO,
  POSITIVE,
  NEGATIVE,
  INVALID
};

SIGN meet(SIGN l, SIGN r){
  if(l == SIGN::BOTTOM) {
    return SIGN::BOTTOM;
  } else if(l == SIGN::TOP) {
    return r;
  } else if(l == SIGN::ZERO) {
    if(r == SIGN::TOP || r == SIGN::ZERO) {
      return SIGN::ZERO;
    } else {
      return SIGN::BOTTOM;
    }
  } else if(l == SIGN::POSITIVE) {
    if (r == SIGN::TOP || r == SIGN::POSITIVE) {
      return SIGN::POSITIVE;
    } else {
      return SIGN::BOTTOM;
    }
  } else if(l == SIGN::NEGATIVE) {
    if (r == SIGN::TOP || r == SIGN::NEGATIVE) {
      return SIGN::NEGATIVE;
    } else {
      return SIGN::BOTTOM;
    }
  } else {
    return SIGN::INVALID;
  }
}

SIGN plus(SIGN l, SIGN r){
  if(l == SIGN::BOTTOM) {
    return SIGN::BOTTOM;
  } else if(l == SIGN::TOP) {
    return r;
  } else if(l == SIGN::ZERO) {
    if (r == SIGN::TOP || r == SIGN::ZERO) {
      return SIGN::ZERO;
    } else if (r == SIGN::POSITIVE) {
      return SIGN::POSITIVE;
    } else if (r == SIGN::NEGATIVE) {
      return SIGN::NEGATIVE;
    } else {
      return SIGN::BOTTOM;
    }
  } else if(l == SIGN::POSITIVE) {
    if (r == SIGN::TOP || r == SIGN::POSITIVE || r == SIGN::ZERO) {
      return SIGN::POSITIVE;
    } else if (r == SIGN::NEGATIVE) {
      return SIGN::BOTTOM;
    } else {
      return SIGN::BOTTOM;
    }
  } else if(l == SIGN::NEGATIVE) {
    if (r == SIGN::TOP || r == SIGN::NEGATIVE || r == SIGN::ZERO) {
      return SIGN::NEGATIVE;
    } else if (r == SIGN::POSITIVE) {
      return SIGN::BOTTOM;
    } else {
      return SIGN::BOTTOM;
    }
  } else {
    return SIGN::INVALID;
  }
}

SIGN mult(SIGN l, SIGN r){
  if(l == SIGN::BOTTOM) {
    return SIGN::BOTTOM;
  } else if(l == SIGN::TOP) {
    return r;
  } else if(l == SIGN::ZERO) {
    if (r == SIGN::TOP || r == SIGN::ZERO || r == SIGN::POSITIVE || r == SIGN::NEGATIVE) {
      return SIGN::ZERO;
    } else {
      return SIGN::BOTTOM;
    }
  } else if(l == SIGN::POSITIVE) {
    if (r == SIGN::TOP || r == SIGN::POSITIVE) {
      return SIGN::POSITIVE;
    } else if (r == SIGN::ZERO) {
      return SIGN::ZERO;
    } else if (r == SIGN::NEGATIVE) {
      return SIGN::NEGATIVE;
    } else {
      return SIGN::BOTTOM;
    }
  } else if(l == SIGN::NEGATIVE) {
    if (r == SIGN::TOP || r == SIGN::NEGATIVE) {
      return SIGN::POSITIVE;
    } else if (r == SIGN::ZERO) {
      return SIGN::ZERO;
    } else if (r == SIGN::POSITIVE) {
      return SIGN::NEGATIVE;
    } else {
      return SIGN::BOTTOM;
    }
  } else {
    return SIGN::INVALID;
  }
}

SIGN negate(SIGN s) {
  if(s == SIGN::BOTTOM) {
    return SIGN::BOTTOM;
  } else if(s == SIGN::TOP) {
    return SIGN::TOP;
  } else if(s == SIGN::ZERO) {
    return SIGN::ZERO;
  } else if(s == SIGN::POSITIVE) {
    return SIGN::NEGATIVE;
  } else if(s == SIGN::NEGATIVE) {
    return SIGN::POSITIVE;
  } else {
    return SIGN::INVALID;
  }
}

std::string toString(SIGN s) {
  if(s == SIGN::BOTTOM) {
    return "_|_";
  } else if(s == SIGN::TOP) {
    return "T";
  } else if(s == SIGN::ZERO) {
    return "0";
  } else if(s == SIGN::POSITIVE) {
    return "+";
  } else if(s == SIGN::NEGATIVE) {
    return "-";
  } else {
    return "INVALID";
  }
}

#endif
