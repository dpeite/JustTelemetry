import json
import random

with open('ruedas2.json') as json_data:
    d = json.load(json_data)
    # print(d)

for key in d.keys():
    a = random.randint(10,50)
    for element in d[key]:
        b = random.randint(-5,5)
        a += b
        element.append(a)
        element[3], element[1] = element[1], element[3]
        element[2], element[0] = element[0], element[2]
        print element
    
with open('ruedas.json', 'w') as outfile:
        json.dump(d, outfile)
