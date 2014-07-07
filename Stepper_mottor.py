!/usr/bin/env python
# -*- coding: utf-8 -*- 
import serial
import time

def moverIzquierdaTope(direction):
	def mov_left():
		print 'izquierda'
		ser.write('IZQUIERDA;\n')
	def mov_right():		
		print 'derecha'
		ser.write('DERECHA;\n')
	def mov_stop():
		print'stop'
		ser.write('DETENTE;\n')
	def apaga():
		return 0

	moviment_ = { '0':apaga,
				  '1':mov_left,
				  '2':mov_right,
				  '3':mov_stop
				}
	return moviment_[direction]
	
ser = serial.Serial('/dev/ttyUSB0',9600,timeout=3)
#state = Enum('TERMINA COMIENZA ERROR')

nombre = ''
while nombre != 0:
	nombre = raw_input('\tgive me a number from: 0 to 2  \n0 - close program \n1 - mov_left \n2 - mov_right \n3 - mov_stop ')
	try:
		nombre = moverIzquierdaTope(nombre)()
	except KeyError:
		#nombre = 0
		ser.close()
		pass
