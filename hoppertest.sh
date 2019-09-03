#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

function test () {
	nDM=$1
	nodes=$2
	expected_res=$3

	echo $nDM
	echo $nodes

	res=$(echo $nDM $nodes | ./a.out)
	printf "result: $res\n"

	if [[ $res == $expected_res ]]; then
		printf "${GREEN}PASS${NC}\n\n"
	else
		printf "${RED}FAIL${NC}\n"
		printf "expected: $expected_res\n\n"
	fi
}

g++ hopper.cc

echo "test case 1 : sample input 1"
test "8 3 1" "1 7 8 2 6 4 3 5" "8"

echo "test case 2 : sample input 2"
test "8 2 1" "1 7 8 2 6 4 3 5" "3"

echo "test case 3 : sample input 3"
test "8 1 1" "1 7 8 2 6 4 3 5" "2"

echo "test case 4 : branching graph 1"
test "4 2 1" "1 2 3 1" "3"

echo "test case 5 : branching graph 2"
test "8 2 1" "5 4 3 2 4 1 5 6" "6"

echo "test case 6 : loop 1"
test "3 3 3" "1 2 3" "3"

echo "test case 7 : complete graph (high branch factor) 1"
test "4 4 4" "1 2 3 4" "4"

echo "test case 8 : complete graph (high branch factor) 2"
test "7 7 7" "1 2 3 4 5 6 7" "7"

echo "test case 9 : tree nodes"
test "3 1 1" "1 2 3" "3"

echo "test case 10 : backtracking 1"
test "8 8 1" "1 6 3 5 4 4 8 2" "7"

echo "test case 11 : backtracking 2, multiple steps"
test "7 2 1" "1 2 1 3 0 4 5" "7"

echo "test case 12 : seperated networks 1"
test "6 4 1" "1 2 4 4 4 4" "4"

echo "test case 13 : seperated networks 2 : three nodes -two connected"
test "3 2 1" "1 2 4" "2"

echo "test case 14 : seperated networks 3 : two nodes not connected"
test "2 1 1" "1 3" "1"












