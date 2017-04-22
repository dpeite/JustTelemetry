import json
import random

with open('ruedas2.json') as json_data:
    d = json.load(json_data)
    # print(d)
first = True
array_dist = []
for key in d.keys():
    a = random.randint(10,50)
    dist = 0
    for elements in d[key]:
        if "data" in elements:
            for idx, element in enumerate(d[key][elements]):
                b = random.randint(-5,5)
                a += b
                element.append(a)
                element[3], element[1] = element[1], element[3]
                element[2], element[0] = element[0], element[2]
                if first:
                    c = random.randint(0,5)
                    dist += c
                    element.append(dist)
                    array_dist.append(dist)
                else:
                    element.append(array_dist[idx])
                # print element
            first = False

    
with open('ruedas.json', 'w') as outfile:
        json.dump(d, outfile)
