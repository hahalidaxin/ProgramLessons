import struct
import pickle
import nation

nations = {}
fin = open(r".\\UN.txt", "r")
while True:
    line = fin.readline()
    if len(line) == 0: break
    strs = line.strip().split(',')
    nations[strs[0]] = nation.Nation(strs[0], strs[1], strs[2], strs[3])
pickle.dump(nations, open(r".\\nationsDict.dat", 'wb'), 0)