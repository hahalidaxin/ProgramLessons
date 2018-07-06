import nation
from collections import defaultdict

def decode(s):
    return ''.join([chr(i) for i in [int(b, 2) for b in s.split(' ')]])

nations = {}
nations_bycontinent = defaultdict(list)
def readData():
    f = open("nationsDict.dat","r")
    while(True) :
        line = f.readline()
        if(len(line)==0): break
        strs = decode(line).strip().split(",")
        nations[strs[0]] = nation.Nation(strs[0],strs[1],strs[2],strs[3])
        nations_bycontinent[strs[1]].append(strs[0])
def query():
    while(True):
        continent = input("Enter a continent: ")
        ls = nations_bycontinent[continent]
        print(ls)
        ls = sorted(ls,key=lambda x: -float(nations[x].population)/float(nations[x].area))
        for i in range(min(len(ls),5)):
            print(" ",ls[i])
if __name__=="__main__":
    readData()
    query()