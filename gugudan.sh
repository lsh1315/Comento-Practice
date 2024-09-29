#!/bin/sh

gugudan(){
	list="1 2 3 4 5 6 7 8 9"

	for val in $list
	do
		result=$(expr $1 \* $val)
		echo "$1 * $val = $result"
	done
}

if [ $# -ne 1 ]; then
	echo "Input 1 Parameter"
elif [ $1 -lt 2 ] || [ $1 -gt 9 ]; then
	echo "Input 2 ~ 9 Integer"
else
	gugudan $1
fi
