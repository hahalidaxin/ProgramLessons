from collections import defaultdict

def readData():
    word_dict = defaultdict(int)
    line = raw_input()
    line = unicode(line,"utf-8")
    for str in line.strip().split(" "):
        word_dict[str] = 1
    return word_dict
def word_seg_match(sent,word_dict):
    ans = []
    sent = unicode(sent,'utf-8')
    end = len(sent)
    begin = 0
    while(end>=1):
        for begin in range(0,end):
            word = sent[begin:end]
            if not(word_dict.get(word) is None) or end==begin+1:
                ans.append(word)
                break
        end = begin
    print " ".join(map(lambda x:x.encode("utf-8"),ans[::-1]))
def runMain(word_dict):
    try:
        while(True):
            line = raw_input()
            if line == '': exit()
            word_seg_match(line,word_dict)
    except:
        exit()

if __name__=="__main__":
    runMain(readData())