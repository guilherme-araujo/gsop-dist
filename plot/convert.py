#!/usr/bin/python


import math
import sys
import csv

graphFile = sys.argv[1]

rowlist	= []

with open(graphFile) as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=';')

    for row in csv_reader:
        if row[0] == 'partial':
            cyc = int(row[1])
            qta = int(row[2])
            qtb = int(row[3])
            sample = int(row[5])
            rowlist.append([qta, cyc, 'A'])
            rowlist.append([qtb, cyc, 'B'])
        elif int(row[2]) != -1:
            qta = int(row[0])
            qtb = int(row[1])
            cycfix = int(row[2])
            cnext = int(math.ceil(float(cycfix)/500)*500)
            for i in range(cnext, 5001, 500):
                rowlist.append([qta, i, 'A'])
                rowlist.append([qtb, i, 'B'])



with open("split_"+graphFile, "a+") as csv_file:
	writer = csv.writer(csv_file)

	for roww in rowlist:
		writer.writerow(roww)
