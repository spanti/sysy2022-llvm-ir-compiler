#!/bin/bash
#批量测试
#generate test.ll and out.o
#clang -emit-llvm -S libsysy.c -o lib.ll
# llvm-link main.ll lib.ll -S -o out.ll
#link with output.o
#无.in 和 存在.in
echo "begin test!"
gcc -c libsysy.c -o libsysy.o

output_file="./out"
test_file_folder="../test/functional"
compiler_path="./c_edu_compiler"
test_file="./test.out"
#tests
for file in "$test_file_folder"/*.out; do
    #generate output_file
    ${compiler_path} "${file%.out}.sy" "test.ll"
    #link two file
    clang -no-pie libsysy.o output.o -o out 
    input_file="${file%.out}.in"
    if [ -e "${input_file}" ]; then
        #exist in
        ${output_file} < ${input_file} > ${test_file}
    else
        #not exist
        ${output_file} > ${test_file}
    fi
    echo $? >> ${test_file}
    #use -diff cammand to compare text
    if diff ${test_file} ${file}; then
        echo "${file} match output"
    else
        echo "${file} not match output"
    fi
done
echo "batch testing complete!"