from algopy import graphmat
from random import randint, choice, shuffle
from enum import Enum
import euler
import copy

def remlist(l, i):
	l[i], v = l[-1], l[i]
	l.pop()
	return v

class Component:
	def getComponentsVector(G, compList):
		""" Get component vector from Component list
		Returns:
			componentCount (int): count of component
			vector (int list): vector component, from 1 to componentCount
		"""
		res = G.order * [1]
		v = 1

		for i in range(1, len(compList)):
			v += 1
			for w in compList[i].val:
				res[w] = v

		return v, res

	def getArticulations(compList):
		""" Get component articulations
		Return (int, int) list: (node, count), where count is the number of 
		connex components created if we remove the node
		"""
		result = []
		for comp in compList:
			for (node, count) in comp.art.items():
				result.append((node, count))

		return result

	""" represent a connex/biconnex/strongly connex component of a graph
	Attributes:
		len (int): vertices count
		col (int): color count
		artk (int int list): articulations
	"""
	def __init__(self, Gen):
		self.len = randint(Gen.minVertices, Gen.maxVertices)
		self.col = Gen.colorComponents

		if self.col > 1 and self.len > 1 and self.len % self.col == 1:
			self.len += 1

		self.cnt = self.len
		self.val = []
		self.art = {}
		self.artk = [[] for _ in range(self.col)]

	def addVertex(self, i):
		self.val.append(i)
		self.cnt -= 1

		if self.cnt == 0:
			shuffle(self.val)
			return False

		return True

	def color(self, i):
		return i % self.col

	def node(self, i):
		return self.val[i]

	def revert(self, v):
		return self.val.index(v)

	def odds(self, G):
		odd = [[] for _ in range(self.col)]

		for i in range(self.len):
			n = self.node(i)

			if G.gendeg[n] % 2 == 1:
				odd[self.color(i)].append(i)

		return odd

	def toEulerian(self, Gen, G):
		def others(o):
			for o2 in odd:
				if o == o2:
					continue

				for j in range(len(o2)):
					yield (o2, j)

		odd = self.odds(G)

		# cas simple: une seule couleur, on lie les nodes ayant une couleur impaire
		# deux à deux
		if self.col == 1:
			odd = odd[0]
			i, s = 0, False

			while i < len(odd):
				for j in range(i + 1, len(odd)):
					a, b = self.node(odd[i]), self.node(odd[j])
					if Gen.addEdge(G, a, b):
						remlist(odd, j)
						remlist(odd, i)

						s = True
						break
				i += not s
				s = False

			return len(odd) <= 2

		total = 0

		for o in odd:
			i = 0
			# on cherche des noeuds impaires d'une autre couleur
			# pour supprimer les impaires actuels
			while i < len(o):
				s = False
				for (o2, j) in others(o):
					a, b = self.node(o[i]), self.node(o2[j])
					if Gen.addEdge(G, a, b):
						remlist(o, i)
						remlist(o2, j)
						s = True
						break
				i += not s

			# si on ne trouve pas assez d'impaire dans les autres couleurs,
			# on lie deux noeuds de la couleur actuelle à un noeud pair
			# d'une autre couleur
			# FIXME le code ci-dessous ne marchera pas dans tous les cas
			# car il est possible que un des deux addEdge ne fonctionne pas
			# principalement si les multigraphes ne sont pas acceptés
			while len(o) >= 2:
				n = self.randomNode(self.color(o[0]))
				n = self.node(n)

				Gen.addEdge(G, self.node(o.pop()), n)
				Gen.addEdge(G, self.node(o.pop()), n)
			total += len(o)

		return total <= 2

	def randomNodeR(self, a, b, evilColor, evilColor2 = -1):
		if self.col == 1:
			return randint(a, b)

		else: # FIXME il y a probablement mieux à faire
			pos = []

			for i in range(a, b + 1):
				if evilColor >= 0 and i % self.col == evilColor:
					continue

				if evilColor2 >= 0 and i % self.col == evilColor2:
					continue
				pos.append(i)	

			return None if not pos else choice(pos)

	def randomNode(self, evilColor = -1, evilColor2 = -1):
		return self.randomNodeR(0, self.len - 1, evilColor, evilColor2)

	def completeWalk(self, Gen, G):
		for i in range(self.len):
			for j in range(i + 1, self.len):
				if self.col == 1 or self.color(i) != self.color(j):
					Gen.addEdge(G, self.node(i), self.node(j))

	def createWalk(self, Gen, G):
		if Gen.complete:
			return self.completeWalk(Gen, G)

		prev = self.node(0)
		prevc = 0

		for i in range(1, self.len):
			node = self.node(i)
			color = self.color(i)

			if not Gen.tree and randint(0, 3) and not Gen.hamiltonian:
				j = self.randomNodeR(0, i - 1, color, prevc)
				if j == None:
					Gen.addEdge(G, prev, node)

				else:
					j = self.node(j)

					Gen.addEdge(G, prev, j)
					Gen.addEdge(G, j, node)
			else:
				if Gen.type == GraphType.TREE:
					prev = randint(0, i - 1)
					prev = self.node(prev)

				Gen.addEdge(G, prev, node)

			prev, prevc = node, color

		if Gen.tree:
				return

		# cycle forcé si orienté ou biconnexe
		if Gen.directed or Gen.linkComponents != None or randint(0, 6) == 0:
			# il est possible que le noeud soit déjà lié au premier
			if not Gen.addEdge(G, prev, self.node(0)):
				Gen.addEdge(G, prev, self.node(1))

	def addRandomEdges(self, to, Gen, G, count, force = -1, c = 1):
		nodef, nodet = set(), set()

		for _ in range(count):
			a = force if force != - 1 else self.randomNode()
			b = to.randomNode(self.color(a))

			a, b = self.node(a), to.node(b)
			Gen.addEdge(G, a, b, c)

			nodef.add(a)
			nodet.add(b)

		return nodef, nodet

	def randomArtc(self, ec = -1, ec2 = -1, create = False):
		if create or len(self.art) == 0 or randint(0, 4) == 0:
			i = self.randomNode(ec, ec2)

			if i in self.art:
				return i

			self.artk[self.color(i)].append(i)
			self.art[i] = 0

			return i
		else:
			c = self.randomNodeR(0, self.col - 1, ec, ec2)

			if self.artk[c]:
				return choice(self.artk[c])
			else:
				return self.randomArtc(ec, ec2, True)

	def incArtc(self, i):
		if i not in self.art:
			self.artk[self.color(i)].append(i)
			self.art[i] = 2
		elif self.art[i] == 0:
			self.art[i] = 2
		else:
			self.art[i] += 1

class GraphType(Enum):
	NORMAL = 1
	EULERIAN = 2 # graph will be eulerian
	TREE = 3 # components will be connex graph with n - 1 edges
	COMPLETE = 4 # all component's nodes will be linked with at least one edge (except nodes
		# of same color

class GraphGenerator:
	""" Simple graph generator
	Useful methods:
		generateGraph

	Attributes:
		connectedComponents (int)
		minVertices (int): minimal vertices count per components
		maxVertices (int): maximal vertices count per components
		directed (bool): True if the graph should be connected. Otherwise, False
		linkComponents (lambda: void->bool): lambda function that should answer
			True if two components should be linked
			if no components should be linked, set linkComponents to None (default)
		colorComponents (int): maximal chromatic number of the graph
			set to 1 (default) if there should be no limit
		loops (lambda: void->bool): lambda function that should answer 
			True if a loop is allowed. For example, to always allow loop use:
				lambda: True
			Default: lambda: False
		multigraph (lambda: void->bool): same as "loops", but for multigraphs
			Default: lambda: False
		planar: WIP
		hamiltonian (bool): if the graph should be hamiltonian, True. Otherwise, False.
		type (GraphType): the graph type
		uselessEdges (lambda: int->int): get count of edges that should be added to a
			connex component of size n
			Default: lambda x: x // 2
		edgesBetweenComponents (lambda: int,int->int): get count of edges that should be
			added to link two connex components of size (x, y)
	"""
	def __init__(self, connectedComponents, minVertices, maxVertices):
		self.connectedComponents = connectedComponents
		self.minVertices = minVertices
		self.maxVertices = maxVertices

		self.directed = False
		self.linkComponents = None
		self.colorComponents = 1

		self.loops = lambda: False
		self.multigraph = lambda: False

		self.planar = False
		self.hamiltonian = False
		self.type = GraphType.NORMAL

		self.uselessEdges = lambda x: x >> 1
		self.edgesBetweenComponents = lambda x, y: max(min(x, y) >> 3, 2)

	def addEdge(self, G, a, b, count = 1):
		if G.adj[a][b] != 0 and not self.multigraph():
			return False

		if a == b and not self.loops():
			return False

		if self.planar:
			#TODO vérifier si le graphe est planaire :'(
			pass

		if self.euler:
			G.gendeg[a] += count
			G.gendeg[b] += count

		for i in range(count):
			G.addedge(a, b)
		
		return True

	def createComponents(self):
		components, tmp, total = [], [], 0

		for _ in range(self.connectedComponents):
			C = Component(self)

			components.append(C)
			tmp.append(C)

			total += C.len

		for i in range(0, total):
			j = randint(0, len(tmp) - 1)

			if not tmp[j].addVertex(i):
				remlist(tmp, j)

		return components, total

	def linkDirected(self, G, a, b):
		v = self.edgesBetweenComponents(a.len, b.len)
		a.addRandomEdges(b, self, G, v)

	def linkNDirected(self, G, a, b):
		v = a.randomArtc()
		c = (self.euler) + 1

		# articulation sur b
		if randint(0, 4) == 0:
			w = b.randomArtc(a.color(v))
			self.addEdge(G, a.node(v), b.node(w), c)
			b.incArtc(w)
		else:
			count = self.edgesBetweenComponents(a.len, b.len)

			_, to = a.addRandomEdges(b, self, G, count, v, c)
	
			if len(to) == 1:
				w = b.revert(next(iter(to)))
				b.incArtc(w)

		a.incArtc(v)

	def generateGraph(self):
		""" Generate a graph :D
		Returns:
			Graphmat: the resulting graph
			Component list: list of components
		"""
		self.tree = GraphType.TREE == self.type
		self.euler = GraphType.EULERIAN == self.type
		self.complete = GraphType.COMPLETE == self.type

		if self.tree:
			self.uselessEdges = lambda x: 0
		
		components, order = self.createComponents()

		G = graphmat.GraphMat(order, self.directed)
		G.gendeg = [0] * G.order

		for comp in components:
			# création des arcs nécessaire à la connexité,
			# forte connexité ou biconnexité
			comp.createWalk(self, G)
			
			# ajouts d'arcs aléatoire 'inutiles'
			comp.addRandomEdges(comp, self, G, self.uselessEdges(comp.len))

		if self.linkComponents != None:
			# on veut relier les composantes biconnexes ou
			# fortemment connexes
			a = components.copy()
			shuffle(a)
			b = [a.pop()]

			if self.directed:
				maxi = max(2, len(components) >> 2)
				f = self.linkDirected
			else:
				maxi = 2
				f = self.linkNDirected

			while len(a) > 0:
				w = a.pop()

				if not self.linkComponents():
					continue
			
				for _ in range(1, maxi):
					v = choice(b)
					f(G, v, w)

				b.append(w)

		if self.euler:
			for comp in components:
				if not comp.toEulerian(self, G):
					raise Exception('Unable to make Eulerian graph')

		return G, components

""" Example: generate K3,3 graph:
generator = GraphGenerator(1, 6, 6)
generator.colorComponents = 2
generator.type = GraphType.COMPLETE
"""


""" Example: bipartite graph with multiple biconnex components, which are eulerian subgraphs """
# Generator configuration
generator = GraphGenerator(20, 5, 10)

generator.directed = True
generator.colorComponents = 2
generator.linkComponents = lambda: randint(0, 3) != 0
generator.uselessEdges = lambda x: (x * x) >> 1
generator.type = GraphType.EULERIAN
generator.multigraph = lambda: True

# Generation

G, comp = generator.generateGraph()

# Check graph

print("Graph order:", G.order)
print("Graph edge count:", G.countEdges())
print("Odd edges:", G.oddEdges())
print("Bipartite:", G.isBipartite())
print("Components vector:", Component.getComponentsVector(G, comp))
print("Articulations:", Component.getArticulations(comp))

# Saving

dot = graphmat.todot(G)
fout = open("graph_example/unconnected.dot", mode='w')
fout.write(dot)
fout.close()
