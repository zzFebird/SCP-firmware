#!/usr/bin/env bash

set -e

# Parameters
SCP_PATH=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
PRODUCT=rdn2
TOOLCHAIN=GNU
MODE=debug
PLATFORM_VARIANT=2
MODEL="$HOME/FVP_RD_N2/models/Linux64_GCC-9.3/FVP_RD_N2_Cfg2"

# Building the images
make -f Makefile.cmake PRODUCT=$PRODUCT MODE=$MODE PLATFORM_VARIANT=$PLATFORM_VARIANT

if [ -z $PLATFORM_VARIANT ]; then
    PRODUCT_BUILD_PATH=${SCP_PATH}/build/$PRODUCT/$TOOLCHAIN/$MODE
else
    PRODUCT_BUILD_PATH=${SCP_PATH}/build/$PRODUCT/platform_variant_$PLATFORM_VARIANT/$TOOLCHAIN/$MODE
fi

SCP_ROM_PATH=${PRODUCT_BUILD_PATH}/firmware-scp_romfw/bin/$PRODUCT-bl1.bin
SCP_RAM_PATH=${PRODUCT_BUILD_PATH}/firmware-scp_ramfw/bin/$PRODUCT-bl2.bin
MCP_ROM_PATH=${PRODUCT_BUILD_PATH}/firmware-mcp_romfw/bin/$PRODUCT-mcp-bl1.bin

# Creating the NOR flash image
NOR_PATH=/tmp/nor.bin
# dd if=/dev/zero of=${NOR_PATH} bs=1024 count=62976 && \
# cat ${SCP_RAM_PATH} >> ${NOR_PATH}

# Booting the firmware
$MODEL \
    -C css0.scp.ROMloader.fname=${SCP_ROM_PATH} \
    -C css0.mcp.ROMloader.fname=${MCP_ROM_PATH} \
    -C board0.flashloader0.fname=${NOR_PATH}
