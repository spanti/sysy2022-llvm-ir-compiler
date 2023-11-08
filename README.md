# sysy2022-llvm-ir-compiler

# introduction
The purpose of writing this project is to become familiar with the LLVM framework.

At present, a compiler has been implemented to translate the SysY2022 language into its equivalent LLVM IR form.
# QuickStart
git clone https://github.com/spanti/sysy2022-llvm-ir-compiler.git

cd build

cmake ..
# Build
cmake --build /mnt/f/test/build --config Debug --target c_edu_compiler -j 10 --
# How to use
/mnt/f/test/build/c_edu_compiler ../test/00_main.sy test.ll

llvm-link-16 test.ll lib.ll -S -o out.ll

lli out.ll
# batch testing
bash test.sh
