import nation
import pickle

def readData():
    return pickle.load(open("nationsDict.dat",'rb'))


def query(nations) :
    while True:
        name = raw_input("Enter a country: ").strip()
        if len(name) == 0: break
        if nations.get(name) is None:
            print("None")
        else:
            print "Continent:", nations[name].continent
            print("Population: {:,.0f}".format(float(nations[name].population)*(10**6)))
            print("Area: {:,.2f} square miles".format(float(nations[name].area)))

if __name__ == "__main__":
    query(readData())