# Name : Ryan Peterman
# UID : 704269982
# Section : Lab 3

# Script that has functionality of linux command comm

import sys, locale
from optparse import OptionParser

def main():
	version_msg = "%prog 2.0"
	usage_msg = """%prog [OPTION]... FILE1 FILE2 
Outputs lines common to two files ."""

	# Instantiate parser
	parser = OptionParser(version=version_msg,
						  usage=usage_msg)

	# Set locale for the sort
	locale.setlocale(locale.LC_ALL, '')

	# Parse and add options
	parser.add_option("-1", action="store_true", dest="one", default=False,
					  help="suppress lines unique to FILE1")
	parser.add_option("-2", action="store_true", dest="two", default=False,
					  help="suppress lines unique to FILE2")
	parser.add_option("-3", action="store_true", dest="three", default=False,
					  help="suppress lines that appear in both files")
	parser.add_option("-u", action="store_true", dest="unsorted", default=False,
					  help="prints out columns even if inputs are not sorted")
	
	# stores list of options and their associated booleans
	# stores file names in args
	options, args = parser.parse_args(sys.argv[1:])

	# Number of arguments test case
	if len(args) != 2:
		parser.error("Error: Two file arguments must be passed in for comparison")
	if args[0] == '-' and args[1] == '-':
		parser.error("Error: Both files cannot read from stdin (Undefined)")

	file1_data = None
	file2_data = None

	# handle file arguments

	if args[0] == '-':
		# read from stdin for file 1
		file1_data = set(sys.stdin.readlines())

	if args[1] == '-':
		#read from stdin for file 2
		file2_data = set(sys.stdin.readlines())

	if file1_data == None:
		# read from file for file 1
		file1 = open(args[0], 'r')
		file1_data = set(file1.readlines())
		file1.close()

	if file2_data == None:
		# read from file for file 2
		file2 = open(args[1], 'r')
		file2_data = set(file2.readlines())
		file2.close()

	# lines unique to file 1
	col1 = file1_data.difference(file2_data)
	# lines unique to file 2
	col2 = file2_data.difference(file1_data)
	# lines shared among both files
	col3 = file1_data.intersection(file2_data)

	to_print = [];
	# append col 1 to what we will print
	if not options.one:
		col_num = 0
		for line in col1: 
			# stores tuple containing lines in col1
			to_print.append((col_num,line))

	if not options.two:
		col_num = 1
		for line in col2:
			#stores tuple containing lines in col2
			to_print.append((col_num, line))

	if not options.three:
		col_num = 2
		for line in col3:
			# stores tuple containing lines in col3
			to_print.append((col_num, line))

	# sort if no -u option
	if not options.unsorted:
		# transform to lowercase before sorting
		to_print = sorted(to_print, key=lambda x: x[1].lower(), reverse=False)

	for line in to_print:
		# tabing based on which column
		sys.stdout.write('\t'*line[0])
		# add newline for readability if not already included
		if '\n' not in line[1]:
			sys.stdout.write(line[1]+'\n')
		else:
			sys.stdout.write(line[1])

# Will call main only if this function is being run as a script
if __name__ == "__main__":
	main()