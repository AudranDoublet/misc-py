from algopy import graph
from algopy import queue

import os

"""
Created on Sat Feb  3 13:49:12 2018

@author: Nathalie
"""
from algopy import graph
import os

def __graphlist(dirpath):
    """builds a list of graphs from a given directory
    Args: 
        dirpath (str): path to the graph directory (".gra" format)
    Returns:
        Graph list
    """
    
    files = os.listdir(dirpath)
    L = []
    for f in files:
        L.append(dirpath + "/" + f)
    
    L = sorted(L)
    return [graph.loadgra(f) for f in L]
    
# pour les adeptes des "list comprehensions"

def __graphlist2(dirpath):
    return [graph.loadgra(dirpath + "/" + f) for f in os.listdir(dirpath)]
    

#without verification!
def run_coloration(f, dirpath):
    return [f(G)[0] for G in __graphlist(dirpath)]

# with verification

def __testcolors(G, colors):
    for s in range(G.order):
        for adj in G.adjlists[s]:
            if colors[s] == colors[adj]:
                print(s, adj, colors)
                return False
    return True

def run_verif_coloration(f, dirpath):
    """tests coloration function on a list of graphs
    Args: 
        f (function): the coloration function (returns (nbcol, color list)) 
        dirpath (str): path to the graph directory (".gra" format)
    Returns:
        the result list: list of color numbers
    """
    tests = __graphlist(dirpath)
    results = []
    for G in tests:
        (nb, colors) = f(G)
        if not __testcolors(G, colors):
            a = graph.todot(G)
            f2 = open("test.dot", 'w')
            f2.write(a)
            f2.close()

            print("wrong coloration")
        results.append(nb)
    return results

""" @author: audran doublet """

# Fix_color et Add_color sont totalement à revoir, car absolument pas optimisés que ce soit au
# niveau de la mémoire comme de la complexité...
def __fix_color(col, precolors):
	if not precolors:
		return col + 1

	precolors = [-1] + sorted(precolors)

	for i in range(len(precolors) - 1):
		if precolors[i +  1] - precolors[i] > 1:
			return precolors[i] + 1

	return precolors[-1] + 1

def __add_color(col, precolors):
	if col not in precolors:
		precolors.append(col)

	return precolors

def __colorize_graph(G, result, precolors, vec, cur):
	maxcol = 0
	q = queue.Queue()

	q.enqueue(cur)
	vec[cur] = 1

	while not q.isempty():
		cur = q.dequeue()
		col = result[cur]
		maxcol = max(maxcol, col)

		for child in G.adjlists[cur]:
			if not vec[child]:
				q.enqueue(child)
				vec[child] = 1

			childcol = result[child]

			if col == childcol:
				__add_color(col, precolors[child])
				result[child] = __fix_color(col, precolors[child])
			elif col > childcol:
				__add_color(col, precolors[child])

	return maxcol

def colorize_graph(G):
	result = G.order * [0]
	precolors = [[] for _ in range(G.order)]
	vec = G.order * [0]
	maxcol = 0

	for i in range(G.order):
		if not vec[i]:
			col = __colorize_graph(G, result, precolors, vec, i)
			maxcol = max(maxcol, col)

	return maxcol + 1, result

print(run_verif_coloration(colorize_graph, "graph_example/coloration"))
