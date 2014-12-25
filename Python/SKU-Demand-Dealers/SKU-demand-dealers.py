#######################################
# Script to Convert to CSV to CPLEX parseabe data
# Compatible Files : SKU-Demand-Dealers.csv
# Author : github.com/meetshah1995
#######################################

import csv

file_name = open('SKU-Demand-Dealers.csv')
csv_reader = csv.reader(file_name)

output_file = open('SKU-Demand-Dealers.txt', 'w')

row_size = 1040
column_size = 1040

row_margin = 5
column_margin = 1

row_count = 0
column_count = 0

row_list = []

for row in csv_reader:
	output_string = ''
	if row_count >= row_margin: 
		output_string += '['
		print row
		for element in row:
			if column_count >= column_margin:
				output_string = output_string + element + ',' 
			column_count += 1
		column_count = 0
		output_string = output_string[:len(output_string) -1 ]	
		output_string += '],'
		row_list.append(output_string) 
	output_string = ''
	row_count += 1

output_file.write('[')
count = 0
for row in row_list:
	if count != len(row_list) -1 :
		output_file.write(row)
	else:
		output_file.write(row[:len(row) - 1])
	count += 1		
output_file.write(']')  