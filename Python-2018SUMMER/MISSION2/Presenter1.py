import nation
import pickle

nations = None


def readData():
    global nations
    nations = pickle.load(open("nationsDict.dat",'rb'))


def query() :
    while True:
        name = raw_input("Enter a country: ")
        if len(name) == 0: break
        if nations.get(name) is None:
            print("None")
        else:
            print "Continent:", nations[name].continent
            print("Population: {:,.0f}".format(float(nations[name].population)*(10**6)))
            print("Area: {:,.2f} square miles".format(float(nations[name].area)))


if __name__ == "__main__":
    readData()
    query()
