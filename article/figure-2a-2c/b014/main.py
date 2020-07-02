#!/usr/bin/python

import sys
import networkx as nx

#Parameter initialization with default values
operation = ""
samples = 1
cycles = 1000
numNodes = 100
numEdges = 4
ephBonus = 0.04
threads = 1
saveFile = ""
graphType = "ba"
ephStartRatio = 0.5
ephPopHistory = False
ephTime = 30
graphFile = "graph.txt"

#Parsing arguments
for arg in sys.argv:
	if "--operation"  in arg:
		op = arg.split("=")
		operation = op[1]
	elif "--savefile"  in arg:
		savef = arg.split("=")
		saveFile = savef[1]
	elif "--samples" in arg:
		smp = arg.split("=")
		samples = int(smp[1])
	elif "--cycles" in arg:
		cyc = arg.split("=")
		cycles = int(cyc[1])
	elif "--numNodes" in arg:
		num = arg.split("=")
		numNodes = int(num[1])
	elif "--numEdges" in arg:
		num = arg.split("=")
		numEdges = int(num[1])
	elif "--ephBonus" in arg:
		num = arg.split("=")
		ephBonus = float(num[1])
	elif "--threads" in arg:
		thr = arg.split("=")
		threads = int(thr[1])
	elif "--graphtype" in arg:
		gtype = arg.split("=")
		graphtype = gtype[1]
	elif "--ephStartRatio" in arg:
		esr = arg.split("=")
		ephStartRatio = float(esr[1])
	elif "--ephPopHistory" in arg:
		epop = arg.split("=")
		ephPopHistory = (epop[1].lower() == "true")
	elif "--ephTime" in arg:
		ephtime = arg.split("=")
		ephTime = int(ephtime[1])
	elif "--graphFile" in arg:
		gfile = arg.split("=")
		graphFile = gfile[1]
	
if operation == "newGraph":
	#Generate graph according to type
	if graphtype == "ba":
		newGraph = nx.barabasi_albert_graph(numNodes, numEdges)
	elif graphtype == "complete":
		newGraph = nx.complete_graph(numNodes)
	elif graphtype == "simple":
		newGraph = nx.random_regular_graph(numEdges, numNodes)
	else:
		newGraph = nx.barabasi_albert_graph(numNodes, numEdges)

	#print(newGraph.number_of_nodes())
	
	#Write base graph to file
	nx.write_adjlist(newGraph, graphFile)
	file = open("opts_"+graphFile,"w")
	file.write(str(numNodes))
	file.close()
	
elif operation == "simulation":
	#Load previously generated graph
	G = nx.read_adjlist(graphFile)
	numNodes = G.number_of_nodes()
	
	#Call simulation executable with options	
	epoph_int = 0
	if ephPopHistory:
		epoph_int = 1
	command = "./main samples "+str(samples)+" cycles "+str(cycles)+" numNodes "+str(numNodes)+" ephBonus "+str(ephBonus)+" threads "+str(threads)+" ephStartRatio "+str(ephStartRatio)+" ephPopHistory "+str(epoph_int)+" ephTime "+str(ephTime)
	
	import subprocess
	
	p = subprocess.Popen(command, shell=True)
	p.wait()
	
	print(command)
	