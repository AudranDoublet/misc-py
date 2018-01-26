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

def __randomAtHead(c, i):
	pos = randint(i, len(c) - 1)
	c[i], c[pos] = c[pos], c[i]

def __randomFromHead(c, i):
	pos = randint(0, i - 1)
	c[i], c[pos] = c[pos], c[i]

def genUnconnectedGraph(count, minV, maxV, directed, multi = True):
	""" Generate a random unconnected graph

	Args:
		count (int): number of connected components
		minV (int): minimal count of vertices for a connected component
		maxV (int): maximal count of vertices for a connected component
		directed (bool): True if the graph is connected, False otherwise
		multi (bool): True if the graph accept multiple connexion between two nodes
	Results:
		GraphMat: the resulting graph
		int list list: list of connected composents, can be useful for tests
	"""
	def addRandomEdges(ga, gb, count):
		for _ in range(count if count > 0 else 1):
			a, b = choice(ga), choice(gb)
			if G.adj[a][b] == 0 or multi:
				if a != b or randint(0, 6) == 0: # moins de boucle sur un noeud
					G.addedge(a, b)

	components, order = __genConnexComponents(count, minV, maxV)
	G =  graphmat.GraphMat(order, directed)

	for component in components:
		# création d'un cycle aléatoire
		le = len(component)

		__randomAtHead(component, 0)

		for i in range(1, le):
			__randomAtHead(component, i)

			if randint(0, 3) and i > 2:
				# ajout de sous cycles; étape inutile mais permet d'avoir
				# des graphes plus variés
				__randomFromHead(component, i - 2)
				a, b = component[i - 1], component[i - 2]
				G.addedge(a, b)
				component[i - 1] = component[i - 2]

			G.addedge(component[i - 1], component[i])

		if directed or randint(0, 6) == 0:
			G.addedge(component[-1], component[0]) # cycle, nécessaire que si orienté
		
		# ajout d'autres arcs aléatoires
		addRandomEdges(component, component, le >> 1)
	
	# ajout d'arcs reliant les différentes composantes fortement connexes
	if directed:
		lists = ([], [])
		for i in range(len(components)):
			lists[i % 2].append(i)

		for i in lists[0]:
			for _ in range(0, randint(0, len(lists[1]) // 2)):
				c1, c2 = components[i], components[choice(lists[1])]
				addRandomEdges(c1, c2, min(len(c2), len(c1)) >> 3)	

	return G, components

def eulerTime(G):
	t = time.time()
	createEulerPath(G, [])
	print(time.time() - t)

"""
createEulerianGraph("graph_example/eulerian5000.gra", 5000, 5000*500)
"""
G = genUnconnectedGraph(5, 3, 5, True, False)[0]
dot = graphmat.todot(G)

fout = open("graph_example/unconnected.dot", mode='w')
fout.write(dot)
fout.close()
"""
G = graphmat.loadgra("graph_example/eulerian" + sys.argv[1] + ".gra")
G2 = graphmat.loadgra("graph_example/eulerian" + sys.argv[1] + ".gra")

print( testEulerPath(G, G2, []) )
"""
"""
G = graphmat.loadgra("graph_example/eulerian" + sys.argv[1] + ".gra")
eulerTime(G)
"""
