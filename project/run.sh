#!/bin/bash
bc=`echo -n $1 | rev | cut -d'.' -f 2- | rev`".bc"
clang -c -emit-llvm $1 -o $bc
make
opt -instnamer -load ./main.so -sign_analysis < $bc 1>/dev/null
