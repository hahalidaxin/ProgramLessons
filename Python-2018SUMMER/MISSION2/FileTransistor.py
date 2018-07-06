import struct

def encode(s):
    return ' '.join([bin(ord(c)).replace('0b', '') for c in s])

fin = open(r'F:\\桌面\\CodeTraining\\Python-2018SUMMER\\MISSION2\\UN.txt','r')
fout = open(r"F:\\桌面\\CodeTraining\\Python-2018SUMMER\\MISSION2\\nationsDict.dat","w")

while(True):
    line = fin.readline()
    if(len(line)==0) :
        break
    fout.write(encode(line.strip()))
    fout.write('\n')

fin.close()
fout.flush()
fout.close()