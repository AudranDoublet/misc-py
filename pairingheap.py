def merge(head1, head2):
    if head1 == None:
        return head2
    elif head2 == None:
        return head1
    elif head1.key < head2.key:
        head1.addchild(head2)
        return head1
    else:
        head2.addchild(head1)
        return head2

def mergepairs(list):
    result = None

    if len(list) % 2 == 1: # gestion du cas impaire
        result = list[-1]

    for i in range(len(list) // 2):
        result = merge(result, merge(list[i * 2], list[i * 2 + 1]))

    return result

class HeapNode:
    """ Pairing heap node. Use list representation of general trees """
    def __init__(self, key, value):
        self.key = key
        self.value = value
        self.children = []

    def addchild(self, e):
        self.children.append(e)
        
class Heap:
    """ Implementation of pairing heaps """
    def __init__(self, keys = None):
        self.head = None
        self.size = 0
        self.parent = None

    def add(self, key, value):
        self.head = merge(self.head, HeapNode(key, value))
        self.size += 1

    def isempty(self):
        return self.head == None

    def pop(self):
        if self.isempty():
            raise Exception("empty heap")

        value = self.head.value
        self.size -= 1

        self.head = mergepairs(self.head.children)

        return value

    def peek(self):
        if self.isempty():
            raise Exception("empty heap")
        else:
            return self.child.value

    def __iter__(self):
        while not self.isempty():
            yield self.pop()

    def __len__(self):
        return self.size

""" exemple:
>>> heap = Heap()
>>> heap.add(12, "je suis un oiseau")
>>> heap.add(6, "je suis un chat")
>>> heap.pop()
"je suis un chat"
>>> for s in heap: print(s)
"je suis un oiseau"
"""