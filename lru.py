#! /usr/bin/python
from collections import deque

class lru_cache():
    
    def __init__(self, n = 3):
        self.usable_sz = n
        self.hashmap = {}
        self.llst = deque()
    
    def __detach(self, item):
        self.llst.remove(item)

    def __attach(self, item):
        self.llst.appendleft(item)
    
    def put(self, key, val):
        oldval = self.hashmap.get(key)
        if not oldval:
            # not in cache yet
            if self.usable_sz:
                # cache is not full
                self.__attach({key: val})
                self.usable_sz -= 1
            else:
                # cache is full
                del self.hashmap[self.llst[-1].keys()[0]]
                self.__detach(self.llst[-1])
                self.__attach({key: val})
            self.hashmap[key] = {key: val}
        else:
            # already in cache
            self.__detach({key: oldval.values()[0]})
            self.__attach({key: val})
            self.hashmap[key] = {key: val}
            
    def get(self, key):
        val = self.hashmap.get(key)
        if not val:
            # not found
            return
        else:
            self.__detach({key: val.values()[0]})
            self.__attach({key: val.values()[0]})
            return val.values()[0]
   
    def clear(self):
        self.usable_sz = len(self.hashmap) 
        self.llst = deque()
        self.hashmap = {}

    def traverse_print(self):
        #print 'hashmap: ', self.hashmap
        #print 'llst: ', self.llst
        for item in self.llst:
            print item.values()[0],
        print ''
            
if __name__ == '__main__':
    cache = lru_cache(3)
    cache.put(4, '4')
    cache.traverse_print()
    cache.put(3, '3')
    cache.traverse_print()
    cache.put(4, '4')
    cache.traverse_print()
    cache.put(2, '2')
    cache.traverse_print()
    cache.put(3, '3')
    cache.traverse_print()
    cache.put(1, '1')
    cache.traverse_print()
    cache.put(4, '4')
    cache.traverse_print()
    cache.put(2, '2')
    cache.traverse_print()
   
    cache.clear() 

    if not cache.get(5):
        print 'hello'
    cache.traverse_print()

    cache.put(1, 'one')
    cache.traverse_print()
    print cache.get(1)
    if not cache.get(9): cache.put(9, 'nine')
    cache.traverse_print()
    print cache.get(1)
    print cache.get(2)
    print cache.get(9)
    
