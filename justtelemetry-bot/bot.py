# -*- coding: UTF-8 -*-
import shutil # Para mover los ficheros a otro directorio
import os # Para consultar el contenido de un directorio

import requests # Para descargar la foto
import telebot # API del bot

import random
from random import randint


token = ""
bot = telebot.TeleBot(token)

@bot.message_handler(content_types=['document'])
def handle_docs(m):

	path = "~/JustTelemetry/app/static/data/sesiones/"
	name = ""
	for x in range(0,5):
		name += str(random.randint(0,9))

	if ".json" in m.document.file_name:
		file_info = bot.get_file(m.document.file_id)
		file = bot.download_file(file_info.file_path)

		if not os.path.exists(path + name):
			os.makedirs(path + name)

		with open(path + name + '/trazada.json','wb') as trazada:
			trazada.write(file)

		with open(path + name + '/info.json','wb') as info:
			info.write(file)

		bot.send_message(m.chat.id, "Sesión añadida correctamente")


bot.polling()