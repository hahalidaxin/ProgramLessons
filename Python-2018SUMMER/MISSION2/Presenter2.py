import nation
import pickle
from collections import defaultdict

def decode(s):
    return ''.join([chr(i) for i in [int(b, 2) for b in s.split(' ')]])

nations = {}
nations_bycontinent = defaultdict(list)


def readData():
    global nations
    nations = pickle.load(open("nationsDict.dat",'rb'))
    for name in nations:
        nations_bycontinent[nations[name].continent].append(name)


def query():
    while(True):
        continent = raw_input("Enter a continent: ")
        ls = nations_bycontinent[continent]
        ls = sorted(ls,key=lambda x: -float(nations[x].population)/float(nations[x].area))
        for i in range(min(len(ls),5)):
            print " ",ls[i]


if __name__=="__main__":
    readData()
    query()