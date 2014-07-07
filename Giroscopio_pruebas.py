#!/usr/bin/env python
# -*- coding: utf-8 -*- 
import serial
import time

class Enum(object):
    def __init__(self, names, separator=None):
        self.names = names.split(separator)
        for value, name in enumerate(self.names):
            setattr(self, name.upper(), value)
    def tuples(self):
        return tuple(enumerate(self.names))
	
def sentFinishSerial(ser):
	ser.write('TERMINA\n')

def sentStartSerial():
	ser.write('COMIENZA\n')

def position (lang,data):
	def mov_izq():
		ImprimeArchivo(data)		
	def mov_der():
		ImprimeArchivo(data)
	def mov_arr():
		ImprimeArchivo(data)
	def mov_abj():
		ImprimeArchivo(data)
	def mov_ok():		
		ImprimeArchivo(data)
	
	mov_ = {"izq":mov_izq,
			"der":mov_der,
			"arr":mov_arr,
			"abj":mov_abj,
			"ok":mov_ok}
	return mov_[lang]

def recibeData():	
	try:
		data = ser.readline()
		pass
	except Exception, e:
		print 'Failed to read data ', e
		raise
	else:
		pass
	finally:
		if "ERROR"in data:
			ImprimeArchivo("Error")
		elif "OK" in data:
			movimiento,anguloX, anguloy =  separadatos(data)
			position("ok",data)()
			print data, movimiento, anguloX, anguloy
		elif "IZQUIERDA" in data:
			position("izq",data)()
			print data
		elif "DERECHA" in data:
			position("der",data)()
			print data
		elif "ARRIBA" in data:
			position("arr",data)()
			print data
		elif "ABAJO" in data:
			position("abj",data)()
			print data
		elif "" in data:
			r = 0 
		else:
			print 'lo que recibio fue ', data
		pass
	

def separadatos(data):	
	valores = data.split()
	movimiento = valores[0]
	anguloX = valores[2]
	anguloy = valores[4]
	return movimiento, anguloX,anguloy


def ImprimeArchivo(error):	
	document = open('errores','a')
	document.write(error)
	document.close()


ser = serial.Serial('/dev/ttyUSB0',9600,timeout=3)
state = Enum('TERMINA COMIENZA ERROR')

while 1:
	recibeData()
