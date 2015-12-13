#! /bin/bash
# script to run ldd on a list of executables 
# stdin: list of executables in /usr/bin
# stdout: writes all used libraries to file, which we will then sort and sed

filename=$1

# to generate the list of libraries for sorting
while read -r line
do
	# if file is executable
	if [[ -x "/usr/bin/$line" ]]
	then 
		ldd "/usr/bin/$line"
	fi

done < "$filename"

# to print human readable output
# to diagnose errors for lab report
# while read -r line
# do
# 	echo "$line"
# 	ldd "/usr/bin/$line"
# 	echo ' '

# done < "$filename"