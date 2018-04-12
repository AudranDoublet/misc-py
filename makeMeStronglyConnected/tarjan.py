from algopy import graph

class Tarjan:
	""" On définit le graphe fréduit de G comme le graphe réduit de G, auquel on a enlevé
		les noeuds n'étant pas sources ou puits
	Cette classe implémente le calcul de ce graphe avec une variante de l'algorithme de Tarjan,
		et un parcours en O(n) ou n est le nombre de composantes fortement connexes de G
	"""
	def __init__(self, G):
		self.G = G
		self.stack, self.reachable = [], set()
		self.c2n, self.c2r, self.c2s = [], [], []
		self.ccf, self.p = [None] * G.order, [0] * G.order
		self.pref, self.ccfid = 0, 0

	def __rec(self, v):
		reach = set()
		self.p[v], msuf = self.pref, self.pref

		self.stack.append(v)
		self.pref += 1

		for w in self.G.adjlists[v]:
			if not self.p[w]:
				a, r = self.__rec(w)
				reach = reach.union(r)
				msuf = min(msuf, a)
			elif self.ccf[w] == None:
				msuf = min(msuf, self.p[w])
			if self.ccf[w] != None and self.ccf[v] != self.ccf[w]:
				reach.add(self.ccf[w])
				self.c2s[self.ccf[w]] = False

		if msuf != self.p[v]:
			return msuf, reach

		self.c2n.append(v)
		self.c2s.append(True)

		while self.stack[-1] != v:
			self.ccf[self.stack.pop()] = self.ccfid

		self.ccf[self.stack.pop()] = self.ccfid
		self.ccfid += 1

		res = set()

		for i in reach:
			if not self.c2r[i]:
				res.add(i)
			else:
				res = res.union(self.c2r[i])

		self.c2r.append(res)
		return msuf, {self.ccfid - 1}

	def run(self):
		for i in range(self.G.order):
			if not self.p[i]:
				self.__rec(i)

		mp = [None] * self.ccfid
		c2n, j = [], 0

		self.Gr = graph.Graph(0, True)
		self.GrI = graph.Graph(0, True)

		src, sin = [], []

		for i in range(self.ccfid):
			if not self.c2s[i]:
				continue

			src.append(j)
			mp[i], j = j, j + 1
			c2n.append(self.c2n[i])

			self.Gr.addvertex()
			self.GrI.addvertex()

			for c in self.c2r[i]:
				if not mp[c]:
					sin.append(j)
					mp[c], j = j, j + 1
					c2n.append(self.c2n[c])
					self.Gr.addvertex()
					self.GrI.addvertex()

				self.Gr.addedge(mp[i], mp[c])
				self.GrI.addedge(mp[c], mp[i])

		self.c2n = c2n
		return src, sin
