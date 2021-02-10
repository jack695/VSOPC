llvm-as $1.ll -o $1.bc
llc -filetype=obj $1.bc
clang $1.o -o $1
rm $1.ll
rm $1.bc
rm $1.o