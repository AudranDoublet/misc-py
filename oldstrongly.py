from algopy import graph, graphmat

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
	tests = __graphlist(dirpath)

	for G in tests:
		if G.order == 812:
			Gr, _, _, _ = makeMeToutPiti(G)
			graph.savedot(Gr, "gr.dot")
		count = f(G)
		a, ccf, _ = tarjan_algorithm(G)

		print(G.order, ':', count, '(', ccf, ')')

def lst(i):
	return [[] for i in range(i)]

def diff(j):
	return 0 if j != 0 else 1

def tarjan_algorithm(G):
	""" Implémentation de trajan, retournant:
	int list: noeud -> ccf
	int: nombre de composantes fortemment connexe
	int list: ccf -> 1 noeud

	La dernière valeur sert pour l'algorithme rendant un graphe fo-connexe
	"""
	stack = []
	res, p = [None] * G.order, [0] * G.order
	res2 = [] # pour avoir un noeud de chaque ccf sans chercher

	global pref, ccf # les pythonneries, c'est la vie
	ccf, pref =  0, 0

	def rec(v):
		global pref, ccf
		p[v], msuf = pref, pref
		stack.append(v)

		pref += 1

		for w in G.adjlists[v]:	
			if not p[w]:
				msuf = min(msuf, rec(w))
			elif res[w] == None:
				msuf = min(msuf, p[w])

		if msuf == p[v]:
			res2.append(v)

			while stack:
				w = stack.pop()
				res[w] = ccf

				if w == v:
					break

			ccf += 1

		return msuf

	for i in range(G.order):
		if not p[i]:
			rec(i)

	return res, ccf, res2

def connex_algorithm(G, GI):
	""" Récupère les composantes connexes et les demi-degrés internes/externes
		d'un graphe fortemment connexe, en utilisant un graphe et son graphe
		inverse (afin d'optimisé)
	Returns:
		int list: noeud -> cc
		int: nombre de cc
		int list: demi-degrés internes des noeuds
		int list: demi-degrés externes des noeuds
	"""
	cc, res = 0, [None] * G.order
	di, de = [0] * G.order, [0] * G.order

	def rec(v):
		res[v] = cc

		for i in G.adjlists[v]:
			de[v] += 1
			di[i] += 1

			if res[i] == None:
				rec(i)

		for i in GI.adjlists[v]:
			di[v] += 1
			de[i] += 1

			if res[i] == None:
				rec(i)

	for i in range(G.order):
		if res[i] == None:
			rec(i)
			cc += 1

	return res, cc, di, de

def makeMeToutPiti(G):
	""" Returns:
	Graph: le graphe réduit
	Graph: le graphe inverse du graphe réduit
	int list: le vecteur qui à un noeud du graphe d'origine associe sa composante fo-connexe
	int list: le vecteur qui à une composante fo-connexe associe un noeud qui la contient
	"""
	ccf, k, res2 = tarjan_algorithm(G)
	Gr = graph.Graph(k, directed = True)
	GrI = graph.Graph(k, directed = True)

	for i in range(G.order):
		k1 = ccf[i]
		l = Gr.adjlists[k1]

		for j in G.adjlists[i]:
			k2 = ccf[j]
			if k1 != k2 and k2 not in l:
				Gr.addedge(k1, k2)
				GrI.addedge(k2, k1)

	return Gr, GrI, ccf, res2

def sinksAndSources(Gr, GrI):
	""" Permet d'obtenir les puits/sources. Renvoit:
	int list list: les sources par rapport à leur cc
	int list list: les puits par rapport à leur cc
	"""

	res, cc, di, de = connex_algorithm(Gr, GrI)
	sources, sinks = lst(cc), lst(cc)

	for i in range(Gr.order):
		if di[i] == 0:
			sources[res[i]].append(i)

		if de[i] == 0:
			sinks[res[i]].append(i)

	return sources, sinks

def makeMeStronglyConnected0(G, Gr, pccf, pccf_v):
	Gr, GrI, ccf_v, ccf = makeMeToutPiti(Gr)

	if Gr.order == 1:
		return 0

	""" ccf[i] contient un noeud de la composante fortement connexe i
		on fait en sorte que lorsqu'on réduit un graphe réduit, on ai
		toujours affaire au noeud d'origine
	"""
	if pccf:
		for i in range(len(ccf)):
			ccf[i] = pccf[ccf[i]]

	""" ccf_v contient le mappage noeud -> composaantes
		néanmoins, quand on applique Tarjan sur le graphe réduit, on perd le mappage
		d'origine, c'est pourquoi on le recrée ici

		pas très opti, voir si mieux (mais tjrs mieux que re-trajan tout le graphe)
	"""
	if pccf_v:
		for i in range(len(pccf_v)):
			pccf_v[i] = ccf_v[pccf_v[i]]
		ccf_v = pccf_v

	sources, sinks = sinksAndSources(Gr, GrI)
	cck = len(sources)

	count = [0]

	def addedge(i, j):
		i = ccf[i] if i != None else diff(ccf[j])
		j = ccf[j] if j != None else diff(i)

		count[0] += 1

		Gr.addedge(ccf_v[i], ccf_v[j])
		G.addedge(i, j)

	for i in range(cck):
		nxt = (i + 1) % cck
		addedge(sinks[i].pop(), sources[nxt].pop())

	a, b = 0, 0

	while a < cck and b < cck:
		if not sinks[a]:
			a += 1
		elif not sources[b]:
			b += 1
		else:
			addedge(sinks[a].pop(), sources[b].pop())

	while a < cck:
		if not sinks[a]:
			a += 1
		else:
			addedge(sinks[a].pop(), None)

	while b < cck:
		if not sinks[b]:
			b += 1
		else:
			addedge(None, sources[b].pop())

	return count[0] + makeMeStronglyConnected0(G, Gr, ccf, ccf_v)

def makeMeStronglyConnected(G):
	return makeMeStronglyConnected0(G, G, None, None)

run_verif_coloration(makeMeStronglyConnected, "graph_example/strongConnectivity")
