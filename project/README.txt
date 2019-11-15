This is an implementation of VASCO[https://github.com/rohanpadhye/vasco] in LLVM.


Useful Links:

http://www.cs.cmu.edu/afs/cs/academic/class/15745-s15/public/lectures/L6-LLVM2-1up.pdf

https://github.com/mirrorer/afl/blob/master/llvm_mode/afl-llvm-pass.so.cc

https://github.com/sampsyo/llvm-pass-skeleton/issues/7#issuecomment-201438757


TODO
Not Handled phi nodes in Sign Analysis Might break

To compile and run the code:
$ ./run.sh <tests/filename.c>

Ex:
$ ./run.sh tests/sign_test1.c
