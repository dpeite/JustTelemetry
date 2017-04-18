import json

with open('trazada.json') as json_data:
    trazada = json.load(json_data)
with open('ruedas.json') as json_data:
    ruedas = json.load(json_data)
with open('amortiguadores.json') as json_data:
    amortiguadores = json.load(json_data)
with open('volante.json') as json_data:
    volante = json.load(json_data)
with open('acelerador.json') as json_data:
    acelerador = json.load(json_data)

    
sensores = [["ruedas",ruedas] , ["amortiguadores",amortiguadores], ["volante",volante], ["acelerador", acelerador]]
for index, element in enumerate(trazada):
    for name, sensor in sensores:
        ruedas2 = {}
        ruedas2["label"] = name
        ruedas2["data"] = {}
        for elements in sensor.keys():
            # print elements
            # print sensor[elements]["label"]
            # print sensor[elements]["data"][index][1]
            ruedas2["data"].update({sensor[elements]["label"] : sensor[elements]["data"][index][1]})

        element.append(ruedas2)

    # print element
print trazada

with open('trazada.json', 'w') as outfile:
    json.dump(trazada, outfile)
