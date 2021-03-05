#!/bin/bash
g++ -fopenmp main.cpp
for i in 1 2 3 4 5 6 7 8 9 10
do
	echo "OMP_NUM_THREADS=$i"
	export OMP_NUM_THREADS=$i;
	./a.out
done