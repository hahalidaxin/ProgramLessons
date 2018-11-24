# -*- coding: utf-8 -*-
'''Step one:New Dictionary'''
dict1 = {}
for i in range(100):
    sentence = raw_input()
    word_list = sentence.split()
    for word in word_list:
        if word not in dict1:
            dict1[word] = set()
        dict1[word].add(i+1)

'''Step two:Print Index'''
answer_list = sorted(dict1.iteritems(),key=lambda d:d[0])
for word in answer_list:
    l = list(word[1])
    l.sort()
    print word[0]+': '+', '.join(map(str,l))

def print_answer(answer_set):
    empty_set = set()                                   
    if empty_set == answer_set:
        print 'None'
    else:
        answer_list=list(answer_set)
        answer_list.sort()
        print ', '.join(map(str,answer_list))

'''Step three:Retrieval'''       
while True:
    Query = raw_input() 
    if Query == '': break
    answer_set = set()         
    if 'OR:' in Query:
        Query = Query[3:]
        Q_word_list = Query.split()
        for word in Q_word_list:
            if (word != '')and(word in dict1):
                answer_set = dict1[word]|answer_set
        print_answer(answer_set)
    else:
        if 'AND:' in Query:Query = Query[4:]
        Q_word_list = Query.split()
        if Q_word_list!=[]:
            for i in range(1,101):answer_set.add(i)          
            for word in Q_word_list:
                if (word != ''):
                    if word in dict1:
                        answer_set = dict1[word]&answer_set
                    else:answer_set = set()
            print_answer(answer_set)                         
        else:print 'None'