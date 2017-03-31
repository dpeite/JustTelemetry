from itertools import product
import json

def multiradix_product(M):
    return 

valor = 100
M = [valor, valor, valor]
cont = 0
prueba = []
aux = []
for a in product(*(range(x) for x in M)):
    aux.append(a[0])
    aux.append(a[1])
    aux.append(a[2])

    prueba.append(aux)
    cont += 1

    print (aux)
    aux = []

print (prueba)
with open("salida.json", 'w') as outfile:
    json.dump(prueba, outfile)
