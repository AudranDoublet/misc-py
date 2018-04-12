from algopy import graph, graphmat
from makeMeStronglyConnected.tarjan import Tarjan
import os

def add_edge(G, c2n, l, f, i):
	if f is None:
		return i

	G.addedge(c2n[l], c2n[i])
	return f

def stronglyRec(G, Gr, GrI, c2n, i, p, l, f):
	p[i] = True
	adj = [(len(GrI.adjlists[c]), c) for c in Gr.adjlists[i]]
	adj.sort()

	if len(adj) == 0:
		return add_edge(G, c2n, l, f, i), i, 1

	tot = 1

	for j in range(len(adj) - 1):
		c = adj[j][1]
		if p[c]:
			continue

		for d in GrI.adjlists[c]:
			if not p[d]:
				f, l, tot1 = stronglyRec(G, Gr, GrI, c2n, d, p, l, f)
				tot += tot1
	
		if not p[c]:
			p[c] = True
			return add_edge(G, c2n, l, f, i), c, tot

	c = adj.pop()[1]
	p[c] = True
	return add_edge(G, c2n, l, f, i), c, tot

def makeMeStronglyConnected(G):
	piti = Tarjan(G)
	src, sin = piti.run()

	Gr, GrI, c2n = piti.Gr, piti.GrI, piti.c2n

	if Gr.order == 1:
		return 0

	src = [(len(Gr.adjlists[c]), c) for c in src]
	src.sort()

	p = [False] * Gr.order
	f, l = None, None
	tot = 0

	for _, i in src:
		if not p[i]:
			f, l, tot1 = stronglyRec(G, Gr, GrI, c2n, i, p, l, f)
			tot += tot1

	for i in sin:
		if not p[i]:
			G.addedge(c2n[i], c2n[GrI.adjlists[i][0]])
			tot += 1

	G.addedge(c2n[l], c2n[f])

	return tot
