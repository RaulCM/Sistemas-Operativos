#!/bin/bash
flag_c=false
flag_d=false
flag_arg=false
while test $# -gt 0
do
	case $1 in
		-c)
			flag_c=true
			compilation_options=$2
			shift
			;;
		-d)
			flag_d=true
			directory=$2
			shift
			;;
		-*)
			echo "$1: Incorrect flag. Usage: ./ccall.sh -d [directory] -c [compilation_options] [filter_word] " 1>&2
			exit 1
			;;
		*)
			flag_arg=true
			filter_word=$1
			;;
	esac
	shift
done
if test $flag_d = true
then
	cd $directory
fi
for i in *.c
do
	args="$compilation_options -o `echo $i | sed 's/\.c$//g'` $i"
	if test $flag_arg = true
	then
		gcc $args 2>&1 | grep $filter_word
	else
		gcc $args
	fi
done
