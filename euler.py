#!/usr/bin/env python
# -*- coding: utf-8 -*-

from algopy import graphmat
from random import randint, choice
import time
import sys

class LinkedList:
	""" Simple class for LinkedList

	Attributes:
		value (int):  value of the node
		next (LinkedList): next node
	"""
	def __init__(self, value, prev):
		""" Init LinkedList

		Args:
			value (int): value of the node
			prev (LinkedList): node into which this node will be inserted
		"""
		if prev:
			self.next = prev.next
			prev.next = self
		else:
			self.next = None

		self.value = value

def graphDeg(G):
	""" Retrieve the degree of each vertices and vertices with an odd value of a graph

	Args:
		G (GraphMat)

	Results:
		int list: degree of each vertices
		int list: odd vertices
	"""
	deg = [0] * G.order
	odd = []

	for i in range(G.order):
		deg[i] = G.adj[i][i]
		ln = -1

		for j in range(G.order):
			deg[i] += G.adj[i][j]

		if deg[i] % 2 == 1:
			odd.append(i)

	return (deg, odd)

def isEulerianPath(path, G):
	""" Check if the given path is a valid eulerian path for the given graph
	This version destructs the provided graph

	Args:
		path (LinkedList)
		G (GraphMat)
	Results:
		bool
	"""
	while path.next:
		a, b = path.next.value, path.value
		if G.adj[b][a] == 0:
			return False

		G.adj[b][a] -= 1
		if a != b:
			G.adj[a][b] -= 1
		path = path.next

	for i in range(G.order):
		for j in range(i):
			if G.adj[i][j] != 0:
				return False

	return True

def __hasSuccessor(G, i, s = 1):
	l = G.adj[i]

	if l == None:
		return 0
	l0 = l[0]	

	if l0 < 0:
		if l[-l0] == 0:
			s = -l0 + 1
			l0 = 0
		else:
			return 1
	if l0 == 0:
		for j in range(s, G.order):
			if l[j]:
				l[0] = -j
				return 1

		G.adj[i] = None
		return 0
	else:
		return 1

def __popSuccessor(G, i):
	l = G.adj[i]
	j = -l[0] if l[0] < 0 else 0
	
	if i != j:
		G.adj[j][i] -= 1

	if l[j] == 1:
		l[0] = 0
		__hasSuccessor(G, i, s = j + 1)
	else:
		l[j] -= 1
	return j

def createEulerPath(G, odd):
	"""
	Create an euler path/cycle for the given graph. The graph will be destroyed during
	the process.

	Args:
		G (graphmat): Eulerian graph
		odd (int list): List of graph nodes that have an odd degree

	Returns:
		LinkedList: An eulerian path/cycle
	"""
	stack = [LinkedList(odd[0] if odd else 0, None)]

	while stack:
		n = stack[-1]
		
		if not __hasSuccessor(G, n.value):
			stack.pop()
			continue

		n2 = LinkedList(__popSuccessor(G, n.value), n)
		if not G.adj[n.value]:
			stack.pop()
		stack.append(n2)

	return n

def testEulerPath(G, G2, odd):
	""" Test function for createEulerPath
	
	Args:
		G (GraphMat): the graph which will be used to create the path
		G2 (GraphMat): a copy of the graph
		odd (int list): odd nodes
	Results:
		bool
	"""
	path = createEulerPath(G, odd)
	return isEulerianPath(path, G2)

def createEulerianGraph(dst, verticesCount, minEdgeCount):
	""" Create an eulerian graph and store it in a file (.gra)

	Args:
		dst (string): path for the resulting file
		verticesCount (int): vertices count for the resulting graph
		minEdgeCount (int): minimal edge count for the resulting graph
	Result:
		GraphMat
	"""
	G = graphmat.GraphMat(verticesCount)

	for i in range(minEdgeCount):
		a, b = randint(0, verticesCount - 1), randint(0, verticesCount - 1)

		G.adj[a][b] += 1
		if a != b:
			G.adj[b][a] += 1

	_, odd = graphDeg(G) # TODO à changer

	for i in range(0, len(odd) - 2 * randint(0, 1), 2): # 1 chance sur 2 d'avoir des impairs
		G.adj[odd[i]][odd[i+1]] += 1
		G.adj[odd[i+1]][odd[i]] += 1

	graphmat.savegra(G, dst)
	return G

def __genConnexComponents(connexComponentCount, minVertices, maxVertices):
	lens, nodes, total = [], [], 0

	for i in range(connexComponentCount):
		v = randint(minVertices, maxVertices)
		total += v
		lens.append((i, v))
		nodes.append([])

	for i in range(0, total):
		j = randint(0, len(lens) - 1)
		n, c = lens[j]
		nodes[n].append(i)

		if c == 1:
			lens[j] = lens[-1]
			lens.pop()
		else:
			lens[j] = (n, c - 1)

	return nodes, total

def eulerTime(G):
	t = time.time()
	createEulerPath(G, [])
	print(time.time() - t)

"""
createEulerianGraph("graph_example/eulerian5000.gra", 5000, 5000*500)
"""
"""
G = graphmat.loadgra("graph_example/eulerian" + sys.argv[1] + ".gra")
G2 = graphmat.loadgra("graph_example/eulerian" + sys.argv[1] + ".gra")

print( testEulerPath(G, G2, []) )
"""
"""
G = graphmat.loadgra("graph_example/eulerian" + sys.argv[1] + ".gra")
eulerTime(G)
"""
