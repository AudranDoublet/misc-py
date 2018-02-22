# -*- coding: utf-8 -*-
"""Graph module.

Provide an implementation of graphs with adjacency matrix.
This can also be called static implementation.

In a graph, vertices are considered numbered from 0 to the order of the graph
minus one. The vertex key, or number, can then be used to access its
neighbour list.

"""

class GraphMat:
    """ Simple class for static graph.

    Attributes:
        order (int): Number of vertices.
        directed (bool): True if the graph is directed. False otherwise.
        adj (List[List[int]]): Adjacency matrix
    """

    def __init__(self, order, directed=False):
        """
        Args:
            order (int): Number of nodes.
            directed (bool): True if the graph is directed. False otherwise.
        """
        
        self.order = order
        self.directed = directed
        self.adj = [[0 for j in range(order)] for i in range(order)]
        self.degrees = [0] * order

    def addedge(self, src, dst):
        """Add egde to graph.
    
        Args:
            src (int): Source vertex.
            dst (int): Destination vertex.
    
        Raises:
            IndexError: If any vertex index is invalid.
    
        """

        if src >= self.order or src < 0:
            raise IndexError("Invalid src index")
        if dst >= self.order or dst < 0:
            raise IndexError("Invalid dst index")
        
        self.adj[src][dst] += 1
        self.degrees[src] += 1
        self.degrees[dst] += 1

        if not self.directed and dst != src:
            self.adj[dst][src] += 1

    def __isBipartite(self, i, v):
        for j in range(self.order):
            if self.adj[i][j]:
                if v[j] == None:
                   v[j] = 1 - v[i][0], v[i][1]

                   if not self.__isBipartite(j, v):
                         return False
                elif v[j] == v[i] and i != j:
                   print(j, i, self.adj[i][j])
                   return False
        return True

    def isBipartite(self):
        v = [None] * self.order
        a = 0
        for i in range(self.order):
            if v[i] == None:
               v[i] = 0, a
               a += 1
               if not self.__isBipartite(i, v):
                  return False
        return True

    def countEdges(self):
         count = 0
         for i in range(self.order):
             for j in range(self.order if self.directed else i + 1):
                 count += self.adj[i][j]
         return count

    def oddEdges(self):
         return [i for i in range(self.order) if self.degrees[i] % 2 == 1]

def todot(ref):
    """Write down dot format of graph.

    Args:
        ref (GraphMat).

    Returns:
        str: String storing dot format of graph.

    """

    (s, link) = ("digraph", " -> ") if ref.directed else ("graph", " -- ")
    s += " G {\n"
    s += "node [shape = circle]\n"
    for src in range(ref.order):
        for dst in range(ref.order):
            if ref.directed or src >= dst:
                for i in range(ref.adj[src][dst]):
                    s += "  " + str(src) + link + str(dst) + '\n'
    s += '}'
    return s

def savedot(ref, filename):
    r = todot(ref)
    f = open(filename, mode='w')
    f.write(r)
    f.close()

# load / save gra format    

def loadgra(filename):
    """Build a new graph from a GRA file.

    Args:
        filename (str): File to load.

    Returns:
        Graph: New graph.

    Raises:
        FileNotFoundError: If file does not exist.

    """

    f = open(filename)
    directed = bool(int(f.readline()))
    order = int(f.readline())
    g = GraphMat(order, directed)
    for line in f.readlines():
        edge = line.strip().split(' ')
        (src, dst) = (int(edge[0]), int(edge[1]))
        g.addedge(src, dst)
    f.close()
    return g


def savegra(G, fileOut):
    gra = str(int(G.directed)) + '\n'
    gra += str(G.order) + '\n'
    for s in range(G.order):
        if G.directed:
            n = G.order
        else:
            n = s
        for adj in range(n):    
            for i in range(G.adj[s][adj]):
                gra += str(s) + " " + str(adj) + '\n'
    fout = open(fileOut, mode='w')
    fout.write(gra)
    fout.close()
    


