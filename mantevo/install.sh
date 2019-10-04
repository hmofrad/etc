#!/bin/sh
# Copyright (C) Mohammad H. Mofrad - All Rights Reserved
# Author: Mohammad H. Mofrad
# Email: hasanzadeh@cs.pitt.edu
# Date: February 2016; Last revision 02-10-2016


export PATH=/usr/lib64/openmpi/bin/:$PATH
export LD_LIBRARY_PATH=/lib64/openmpi/lib:$LD_LIBRARY_PATH

mkdir /var/log/mantevo/

tar xzvf CloverLeaf-1.1.tar.gz
tar xzvf CoMD_Ref.tgz
tar xzvf miniFE-2.0.1.tgz
tar xzvf miniFE-2.0_ref.tgz
tar xzvf miniFE-2.0_openmp_ref.tgz
rm -rf miniFE-2.0_*.tgz
rm -rf miniFE-2.0.1_openmp_opt.tgz


# CloverLeaf OpenMP
cd CloverLeaf-1.1/CloverLeaf_OpenMP/
make COMPILER=GNU
cp clover_bm16_short.in clover.in

# CloverLeaf MPI
cd ../CloverLeaf_MPI/
make COMPILER=GNU
cp clover_bm16_short.in clover.in

# CoMD OpneMP
cd ../../CoMD_Ref/src-openmp/
cp Makefile.vanilla Makefile
make


# CoMD MPI
cd ../src-mpi/
cp Makefile.vanilla Makefile
make

# miniFE OpenMP
cd ../../miniFE-2.0_openmp_ref/src
make

# miniFE MPI
cd ../../miniFE-2.0_ref/src
make

# Check installation 
cd ../../
R='\033[0;31m'
G='\033[0;32m'
N='\033[0m' # No Color

declare -a FILE=('CloverLeaf-1.1/CloverLeaf_OpenMP/clover_leaf' 'CloverLeaf-1.1/CloverLeaf_MPI/clover_leaf' 'CoMD_Ref/bin/CoMD-openmp-mpi' 'CoMD_Ref/bin/CoMD-mpi' 'miniFE-2.0_openmp_ref/src/miniFE.x' 'miniFE-2.0_ref/src/miniFE.x')

for f in "${FILE[@]}"; do
        if [ -f $f ]; then
            printf "[${G}success${N}] $f\n"
        else
            printf "[${R}failure${N}] $f\n"
        fi
done
