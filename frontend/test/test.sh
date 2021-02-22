#!/bin/bash

RED="\033[0;31m"
GREEN="\033[0;32m"
RESET="\033[0m"

TEST_FOLDER="./tig_files"

# each of this folder needs two folders inside: pass & fail
TESTS=(${TEST_FOLDER}/princeton_tests ${TEST_FOLDER}/local_tests)

executable="../parsertest" # todo change to entire frontend

make_error() {
    echo -e "File $1:"
    echo -e "\t$2"
}

# compile tests
cd ../ && make tests && cd test && clear

for folder in ${TESTS[@]}; do
    pass_errors=()
    fail_errors=()
    for file in ${folder}/pass/*; do
        exec_print_result=$(./${executable} ${file})
        exec_return=$(echo $?)
        if [ $exec_return -ne 0 ]; then
            pass_errors+=$file
        fi
    done

    for file in ${folder}/fail/*; do
        ./${executable} ${file} >/dev/null
        exec_return=$(echo $?)
        if [ $exec_return -ne 1 ]; then
            fail_errors+=$file
        fi
    done

    echo
    if [ ${#pass_errors[@]} -eq 0 ]; then
        echo -e "$folder/pass ... ${GREEN}ok${RESET}"
    else 
        echo -e "$folder/pass ... ${RED}expected all to pass, but got fail in:${RESET}"
        for err in $pass_errors; do
            echo -e "  " $err
        done
    fi

    if [ ${#fail_errors[@]} -eq 0 ]; then
        echo -e "$folder/fail ... ${GREEN}ok${RESET}"
    else 
        echo -e "$folder/fail ... ${RED}expected all to fail, but got success in:${RESET}"
        for err in $fail_errors; do
            echo -e "  " $err
        done
    fi
    echo "================================================================================="
done