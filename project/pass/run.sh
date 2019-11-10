#!/bin/bash
bc=`echo -n $1 | rev | cut -d'.' -f 2- | rev`".bc"
clang -c -emit-llvm $1 -o $bc
make
printf "Function Pass----------\n\n"
opt -instnamer -load ./pass.so -c_pass < $bc 1>/dev/null
printf "\n\nModule Pass-----------\n\n"
opt -instnamer -load ./pass.so -cm_pass < $bc 1>/dev/null
