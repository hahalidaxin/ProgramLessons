from collections import defaultdict

class INVERTED_INDEXOR:
    def __init__(self):
        self.dict = defaultdict(list)

    def buildIndex(self):
        dict = self.dict
        for i in range(1, 101):
            strs = raw_input().strip().split(" ")
            for str in strs:
                if not dict[str].count(i):
                    dict[str].append(i)

    def printIndex(self):
        dict = self.dict
        keys = sorted(dict.keys())
        for s in keys:
            print str(s) + ": " + ", ".join(map(str, dict[s]))

    def queryIndex(self, line, type):
        dict = self.dict
        ans = set()
        flag = 0
        for s in line.strip().split(" "):
            if (flag==0):
                ans = set(dict[s])
                flag = 1
            else:
                if (not type):
                    ans = ans & set(dict[s])
                else:
                    ans = ans | set(dict[s])
        if len(ans) == 0:
            print None
        else:
            ls = sorted(list(ans))
            print ", ".join(map(str, ls))

    def runQuery(self):
        while (True):
            line = raw_input()
            if len(line) == 0:
                break
            else:
                type = 0
                if  line.startswith("AND: "):
                    line = line.strip()[5:]
                elif line.startswith("OR: "):
                    line = line.strip()[4:]
                    type = 1
                self.queryIndex(line, type)


if __name__ == "__main__":
    indexor = INVERTED_INDEXOR()
    indexor.buildIndex()
    indexor.printIndex()
    indexor.runQuery()