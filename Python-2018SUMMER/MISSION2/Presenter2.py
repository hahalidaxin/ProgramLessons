import nation
import pickle
from collections import defaultdict

def readData():
    nations_bycontinent = defaultdict(list)
    nations = pickle.load(open("nationsDict.dat",'rb'))
    for name in nations:
        nations_bycontinent[nations[name].continent].append(name)
    return nations,nations_bycontinent


def query(nations,nations_bycontinent):
    while(True):
        continent = raw_input("Enter a continent: ").strip()
        if len(continent)==0: break
        if nations_bycontinent.get(continent) is None:
            print "None"
        ls = nations_bycontinent[continent]
        ls = sorted(ls,key=lambda x: -nations[x].popDensity())
        for i in range(min(len(ls),5)):
            print " ",ls[i]


if __name__=="__main__":
    data = readData()
    query(data[0],data[1])