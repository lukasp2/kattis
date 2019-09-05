#!/bin/bash

g++ hehe.cc -o h
g++ hopper.cc

for i in {1..1}; do
	input=$(./h)
	printf "input: "
	echo $input

	output=$(./h | ./a.out)
	printf "output: "
	echo $output

	sleep 1
done
