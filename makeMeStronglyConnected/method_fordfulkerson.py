from algopy import graph
from makeMeStronglyConnected.tarjan import Tarjan

def SourceSinkGraph(G):
	""" process the graph containing only sources and sinks from the reducted graph of G
	This implementation call Tarjan class located in 'tarjan.py'
	Returns:
		graph	The resulting graph
		c2n	A mapping (result graph's node) -> (original graph's node)
		sources	A list of all sources
		sinks	A list of all sinks
	"""
	T = Tarjan(G)
	sources, sinks = T.run()

	return T.Gr, T.c2n, sources, sinks

def MaximumCardinalityMatching(G, sources, sinks):
	"""
	Either E the set of couples (x, y) such that x->y exist in G. In addition, there must be only one couple (x, u) and one couple (u, y), where u is a vertex

	Either F the set of couples (x, x) such that x is a node of G and such that there exist no couple (x, u) in E

	Either G the union between E and F. The purpose of this algorithm is to maximize Card(E).

	The algorithm return G.
	This algorithm is a special case of Ford-Fulkerson algorithm.

	Returns:
		(int, int) list		G
	"""
	seen = None
	match = [None] * G.order

	matchList = []

	def RecFunc(node):
		"""
		Recursive function for Ford-Fulkerson algorithm
		"""
		for child in G.adjlists[node]:
			if not seen[child]:
				seen[child] = True

				if match[child] is None or RecFunc(match[child]):
					match[child] = node
					return True
		return False

	for i in sources:
		seen = [False] * G.order

		if not RecFunc(i):
			matchList.append((i, i)) # i match with himself
			seen[i] = -1

	for i in sinks:
		if match[i] is None:
			matchList.append((i, i))
		elif seen[i] != -1:
			matchList.append((match[i], i))

	return matchList

def MakeMeStronglyConnected(G):
	"""
	Add a set of edges A' to graph G. The algorithm try to minimize Card(A').

	Return:
		int	Card(A')
	"""
	Gr, c2n, sources, sinks		   = SourceSinkGraph(G)

	if Gr.order == 1: # If the graph G is already strongly connected, Card(A') is 0
		return 0

	matching = MaximumCardinalityMatching(Gr, sources, sinks)

	"""
	For each element (x, y) of G
		(x', y') <- Not marked element of G
		Add edge (y, x')
		Set (x', y') as marked
	"""	
	first, last = matching[0]

	for i in range(1, len(matching)):
		x, n = matching[i]
		G.addedge(c2n[last], c2n[x])
		last = n

	G.addedge(c2n[last], c2n[first])

	return len(matching)
