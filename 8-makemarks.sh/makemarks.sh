#!/bin/bash

marks(){
	n=`cat $1 | awk '$1 ~ /'$2'/ {print $2}'`
	if ! test $n
	then
		echo -n "		-"
	else
		echo -n "		$n"
	fi
}

if test $# -ne 4 ; then
	echo "Usage: $0 '[file1] [file2] [file3] [file4]'" 1>&2
	exit 1
fi

names=`cat $* | awk '{print $1}' | egrep '^([A-Z]|[a-z])+' | sort | uniq`

echo "#Nombre		Ejer1	Ejer2	Ejer3	Ejer4	Final" > notasfinales.txt
echo >> notasfinales.txt

for i in $names ; do
	echo -n $i
	for j in $@;
	do
		marks $j $i
	done
	echo -n "		"
	cat $* |  awk '$1 ~ /'$i'/{sum=sum+$2;n+=1} END {if (n == 4) {printf("%.2f\n", sum/4)} else {printf "NP\n"}}'
done >> notasfinales.txt
