import json

lat = 42.175746
lon = -8.713508
vuelta = []
vueltas = []

with open("trazada.json") as data_file:
    info = json.load(data_file)

for coord in info:
    if (lat-0.000010) <= coord[0] <= (lat+0.000010) and (lon-0.000010) <= coord[1] <= (lon+0.000010):
        if coord[0] == lat and coord[1] == lon:
            pass
        else:
            vueltas.append(vuelta[:])
            del vuelta[:]
            # print coord
    vuelta.append(coord)
vueltas.append(vuelta[:])
#print vueltas
print len(vueltas)

for vuelta, coords in enumerate(vueltas):
    with open('vuelta_'+str(vuelta)+'.json', 'w') as outfile:
        json.dump(coords, outfile)

#42.233547 -8.743652
#42.233562 -8.743610
