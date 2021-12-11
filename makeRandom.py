import firebase_admin
import random
import string
import threading #multi threading
from firebase_admin import credentials as cd
from firebase_admin import db

global flag   #if gor equal value then flag = 1 
flag = 0

cd = cd.Certificate("./embedded-7af04-firebase-adminsdk-dqnk3-648973e6ae.json")
firebase_admin.initialize_app(cd,{
    'databaseURL' : 'https://embedded-7af04-default-rtdb.firebaseio.com/'    
})

global eqaul #use in code
equal = ''

def get(): #get equal valeu from Fireabse RDB
    while True:
        dir = db.reference('game/equal') 
        global equal
        equal = str(dir.get())
        if(equal != ''):
            global flag
            flag = 1
            print('got equal value from RDB')
            break


#make random
string_pool = string.digits
result = ''
for i in range(4):
    result += random.choice(string_pool)
start = 'true'
f = open('random.txt','w')
f.write(result)
f.close()
start = 'true'

dir = db.reference('game')
dir.update({'answer':result})
dir.update({'start':start})

t = threading.Thread(target=get)
t.start()
print('start looking RDB changes')

while True: 
    if(flag == 1):
        print('writting start')
        outlist = [start, result, equal]
        #f = open('answer.txt','w')
        with open('answer.txt','w') as f:
            for i in range(3):
                f.write(outlist[i])
                f.write("\n")
        #f.close()
        print('finish writting')
        break
    else:
        continue
