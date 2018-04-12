from algopy import graph, graphmat
import os
import time

from makeMeStronglyConnected import method_fordfulkerson, method_sort, tarjan

def strongly_connected(G):
	p = tarjan.Tarjan(G)
	p.run()

	return p.ccfid == 1

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

def run_verif_strongly(f, f2, dirpath):
	tests = __graphlist(dirpath)
	tests2 = __graphlist(dirpath)
	i = 0

	t1, t2 = 0, 0

	for G in tests:
		G2 = tests2[i]
		i += 1

		s = time.time()
		count = f(G)
		t1 += time.time() - s
		s = time.time()
		count2 = f2(G2)
		t2 += time.time() - s

		r = strongly_connected(G)
		r2 = strongly_connected(G2)

		ok = "[{}|{}]".format("OK" if r else "KO", "OK" if r2 else "KO")
		res = "<" if count < count2 else (">" if count > count2 else "=")

		s = "{} {} {:3d} => {:3d} | {:3d}".format(res, ok, G.order, count, count2)
		print(s)

	print('Times: {:3f}ms vs {:3f}ms'.format(t1 * 1000, t2 * 1000))

if __name__ == '__main__':
	run_verif_strongly(method_sort.makeMeStronglyConnected, method_fordfulkerson.MakeMeStronglyConnected, "graph_example/strongConnectivity")
