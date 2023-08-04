#!/bin/bash

options=$(getopt -o cu -l clean,update: -- "$@")
eval set -- "$options"
while true; do
    case "$1" in
    -c|--clean)
		rm -f cscope.*
		rm -f tags
		rm -f cscope.files
		exit 0
        ;;
    --)
        shift
        break
        ;;
    esac
    shift
done

rm -f cscope.files

# arch
find arch/arm/arm-m/ -type f -iname *.[ch] > cscope.files
find arch/arm/src/ -type f -iname *.[ch] >> cscope.files
# cmsis
echo contrib/cmsis/git/CMSIS/Core/Include/core_cm7.h >> cscope.files
# debugger
find debugger/ -type f -iname *.[ch] >> cscope.files
# framework
find framework/include/ -type f -iname *.[ch] >> cscope.files
find framework/src/ -type f -iname *.[ch] >> cscope.files
# module
find module/pcid -type f -iname *.[ch] >> cscope.files
find module/sid/ -type f -iname *.[ch] >> cscope.files
find module/system_info/ -type f -iname *.[ch] >> cscope.files
find module/pl011/ -type f -iname *.[ch] >> cscope.files
find module/clock/ -type f -iname *.[ch] >> cscope.files
find module/bootloader/ -type f -iname *.[ch] >> cscope.files
find module/isys_rom/ -type f -iname *.[ch] >> cscope.files
# product
find product/rdn2/ -type f -iname *.[ch] >> cscope.files
# toolchain
# find ../tools/gcc/gcc-arm-11.2-2022.02-x86_64-arm-none-eabi/arm-none-eabi/include/ -type f -iname *.h >> cscope.files

# generate index file
cscope -bkqi cscope.files
ctags -L cscope.files
