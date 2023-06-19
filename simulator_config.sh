#!/bin/bash

git clone https://github.com/ppeetteerrs/gem5-RISC-V-FS-Linux.git ~/development/inesc/
#1. Recommended Setup
#This section recommends a development environment setup. The subsequent sections will assume that this setup is used. Please change according to your personal preferences.
#
#1.1 Folder Structure
#📦~ (or any working directory)
# ┣ +gem5
# ┗ +riscv
#   ┣ +bin: RISC-V tool binaries (e.g. GNU-toolchain, QEMU etc.)
#   ┣ +logs: gem5 simulation logs
#   ┣ +out: build outputs (kernel / bootloader / devicetree / disk image)
#   ┗ +src
#     ┣ +linux: Linux kernel repo
#     ┣ +pk: RISC-V proxy kernel (bbl bootloader)
#     ┣ +qemu: QEMU emulator
#     ┣ +toolchain: RISC-V GNU toolchain
#     ┗ +ucanlinux: UCanLinux disk image
# ┗ +benchmarks
#   ┗ +parsec: PARSEC 3.0 benchmark for RISC-V

#1.2 Bash Environment


source pathsrc

mkdir -p $RISCV/bin
mkdir -p $RISCV/out
mkdir -p $RISCV/src

#######################################################################

#2 Tools and Libraries
#2.1 RISC-V GNU Toolchain

sudo apt-get install -y autoconf automake autotools-dev curl python3 libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev
git clone https://github.com/riscv/riscv-gnu-toolchain $SRC/toolchain
cd $SRC/toolchain
./configure --prefix=$RISCV --enable-multilib
make linux -j$(nproc)
# You will need this to compile things like PARSEC and riscv-tests
make newlib -j$(nproc)

#######################################################################
#2.2 QEMU

sudo apt-get install -y git libglib2.0-dev libfdt-dev libpixman-1-dev zlib1g-dev ninja-build
git clone https://git.qemu.org/git/qemu.git $SRC/qemu
cd $SRC/qemu
./configure --target-list=riscv64-softmmu --prefix=$RISCV
make -j$(nproc)
make install

#######################################################################
#3 gem5 Installation
#3.1 Dependencies

sudo apt install -y build-essential git m4 scons zlib1g zlib1g-dev libprotobuf-dev protobuf-compiler libprotoc-dev libgoogle-perftools-dev python3-dev python3-six python-is-python3 libboost-all-dev pkg-config

#3.2 Building gem5 Binaries

git clone https://gem5.googlesource.com/public/gem5 $G5

# Uncomment if you want to contribute to gem5
# cd $G5 && git checkout develop

# Uncomment if you have anaconda3 installed (or non-deault python interpreter)
# export PATH=/usr/bin:$PATH

cd $G5 && scons build/RISCV/gem5.opt -j$(nproc)

#######################################################################
#4. Linux System
#4.1 Disk Image (UCanLinux)

git clone https://github.com/UCanLinux/riscv64-sample.git $SRC/ucanlinux
cp $SRC/ucanlinux/riscv_disk $OUT


#######################################################################
#4.2 Linux Kernel v5.19 - changed from 5.10 due to compiler and arch extensions issues

git clone https://github.com/torvalds/linux.git $SRC/linux
cd $SRC/linux
git checkout v5.19
cp $SRC/ucanlinux/kernel.config .config
make ARCH=riscv CROSS_COMPILE=riscv64-unknown-linux-gnu- olddefconfig
make ARCH=riscv CROSS_COMPILE=riscv64-unknown-linux-gnu- all -j$(nproc)
cp vmlinux $OUT


#######################################################################
#4.3 Berkeley Bootloader (bbl)

sudo apt-get install -y device-tree-compiler
git clone https://github.com/riscv/riscv-pk.git $SRC/pk
mkdir -p $SRC/pk/build 
cd $SRC/pk/build
#../configure --host=riscv64-unknown-linux-gnu --with-payload=$OUT/vmlinux --prefix=$RISCV
../configure --host=riscv64-unknown-linux-gnu --with-arch=rv64gc_zifencei --with-abi=lp64d --with-payload=$OUT/vmlinux --prefix=$RISCV\n
CFLAGS="-g" make -j$(nproc)
make install
cp bbl $OUT

#######################################################################
#5. Useful Examples
#5.1 Running QEMU FS Linux
