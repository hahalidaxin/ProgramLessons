import nation
def decode(s):
    return ''.join([chr(i) for i in [int(b, 2) for b in s.split(' ')]])

nations = {}
def readData():
    f = open("nationsDict.dat","r")
    while(True) :
        line = f.readline()
        if(len(line)==0): break
        strs = decode(line).strip().split(",")
        nations[strs[0]] = nation.Nation(strs[0],strs[1],strs[2],strs[3])
def query() :
    while(True):
        name = input("Enter a country: ")
        if(len(name)==0):break
        if(nations.get(name) is None):
            print("None")
        else :
            print("Continent: ",nations[name].continent)
            print("Population: {:,.0f}".format(float(nations[name].population)*(10**6)))
            print("Area: {:,.2f} square miles".format(float(nations[name].area)))

if __name__=="__main__":
    readData()
    query()
