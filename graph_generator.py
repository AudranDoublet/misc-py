from algopy import graphmat
from random import randint, choice

class GraphGenerator:
	""" Class used to generate a random graphmat

	Attributes:
		connexComponents (int): strongly connex/connex/biconnex composent count
		minVertices (int): min vertices count by connex components
		maxVertices (int): max vertices count by connex components
		directed = False (bool): True if the graph sould be directed, False otherwise
		allowMultigaph = True (bool): True if the graph is connected, False otherwise
		joinConnexComponents = False (bool): True if the graph sould be biconnex, or
			if strongly connected components should be linked
		joinAllBiconnex = True (bool): True if the generated non-biconnex graph should
			be connex
	"""
	def __init__(self, connexComponents, minVertices, maxVertices):
		""" Init GraphGenerator

		Args:
			connexComponents (int)
			minVertices (int)
			maxVertices (int)
		"""
		self.connexComponents = connexComponents
		self.minVertices = minVertices
		self.maxVertices = maxVertices

		self.directed = False
		self.allowMultigraph = True
		self.joinConnexComponents = False
		self.joinAllBiconnex = True

	def __randomConnexComponents(self):
		lens, nodes, total = [], [], 0

		for i in range(self.connexComponents):
			v = randint(self.minVertices, self.maxVertices)
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

	def __addRandomEdges(self, G, ga, gb, count):
		fromn, ton = [], []
		for _ in range(count if count > 0 else 1):
			a, b = choice(ga), choice(gb)
			if G.adj[a][b] == 0 or self.allowMultigraph:
				if a != b or randint(0, 6) == 0: # moins de boucle sur un noeud
					G.addedge(a, b)
					fromn.append(a)
					ton.append(b)
		return fromn, ton

	def __randomAtHead(self, c, i):
		pos = randint(i, len(c) - 1)
		c[i], c[pos] = c[pos], c[i]

	def __randomFromHead(self, c, i):
		pos = randint(0, i - 1)
		c[i], c[pos] = c[pos], c[i]

	def __randomArtc(self, artic, component):
		if not artic:
			artic.append( (choice(component), 0) )
			return 0
		elif randint(0, 4) == 0: # nouvelle articulation
			c = choice(component)

			#TODO optimisable
			for i in range(len(artic)): # on n'ajoute pas deux fois le même
				if artic[i][0] == c:
					return i

			artic.append( (c, 0) )
			return len(artic) - 1
		else:
			return randint(0, len(artic) - 1)

	def __incArtc(self, artic, i, count = 1):
		(a, b) = artic[i]
		if b == 0:
			b = 1
		artic[i] = (a, b + 1)

	def __incCreate(self, artic, node):
		for i in range(artic):
			if artic[i][0] == node:
				artic[i][0] += 1
				return
		artic.append((node, 2))

	def __allSames(self, l):
		for i in range(1, len(l)):
			if l[i] != l[i - 1]:
				return False
		return True

	def generate(self):
		""" Generate a random graph

		Returns:
			graph			- the resulting graph
			list list int		- connex/biconnex/strongly connex components
			list int (int, int) 	- empty when the graph is directed or when connex
				components should not being connected. Otherwise, returns
				each articulations (first int) and the number of connex components
				created if we remove it. Articulations are grouped by components
		"""
		components, order = self.__randomConnexComponents()
		G =  graphmat.GraphMat(order, self.directed)
		articulations = []

		for component in components:
			# création d'un cycle aléatoire
			le = len(component)

			self.__randomAtHead(component, 0)

			for i in range(1, le):
				self.__randomAtHead(component, i)

				if randint(0, 3) and i > 2:
					# ajout de sous cycles; étape inutile mais permet d'avoir
					# des graphes plus variés
					self.__randomFromHead(component, i - 2)
					a, b = component[i - 1], component[i - 2]
					G.addedge(a, b)
					(component[i - 1], component[i - 2]) = (b, a)

				G.addedge(component[i - 1], component[i])

			if self.directed or self.joinConnexComponents or randint(0, 6) == 0:
				G.addedge(component[-1], component[0])
				 # cycle, nécessaire que si orienté ou biconnexe
			
			# ajout d'autres arcs aléatoires
			self.__addRandomEdges(G, component, component, le >> 1)
		
		# ajout d'arcs reliant les différentes composantes fortement connexes
		if self.directed and self.joinConnexComponents:
			lists = ([], [])
			for i in range(len(components)):
				lists[i % 2].append(i)

			for i in lists[0]:
				for _ in range(0, randint(0, len(lists[1]) // 2)):
					c1, c2 = components[i], components[choice(lists[1])]
					self.__addRandomEdges(G, c1, c2, min(len(c2), len(c1)) >> 3)
		elif not self.directed and self.joinConnexComponents:
			"""
			on cherche à avoir un graphe non biconnexe
			chaque noeud des composantes connexes générées avant est dans un cycle
			les composantes sont donc des composantes biconnexe

			on va chercher à les lier afin de lier les composantes biconnexes et de 
			créer les articulations, sans créer de cycle
			"""
			articulations = [[] for i in range(len(components))]
			a = [i for i in range(len(components))]
			b = []

			while a:
				self.__randomAtHead(a, 0)
				(a[0], a[-1]) = (a[-1], a[0])
				v = a.pop()

				if not self.joinAllBiconnex and randint(0, 6) == 0:
					continue

				self.__randomAtHead(components[v], 0)

				if b:
					i = choice(b)
					ai, av = articulations[i], articulations[v]
					ci, cv = components[i], components[v]

					w = self.__randomArtc(ai, ci)

					# l'autre a une articulation en ce point
					if randint(0, 4) == 0:
						j = self.__randomArtc(av, cv)
						G.addedge(ai[w][0], av[j][0])

						self.__incArtc(ai, w)
						self.__incArtc(av, j)
					else:
						m = max(2, len(cv) >> 3)
						_, to = self.__addRandomEdges(G, [ai[w][0]], cv, m)
						self.__incArtc(ai, w)

						if len(to) == 1 or self.__allSames(to):
							self.__incCreate(av, to[0])

				b.append(v)

		return G, components, articulations

generator = GraphGenerator(5, 3, 6)
generator.joinConnexComponents = True
#generator.joinAllBiconnex = False

G, components, articulations = generator.generate()
dot = graphmat.todot(G)

print(articulations)

fout = open("graph_example/unconnected.dot", mode='w')
fout.write(dot)
fout.close()
