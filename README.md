Solo pruebas
=======================

Pruebas para el Giroscopio/acelerometro MPU6050 y el stepper mottor con  easy driver


red and green are for coil A
blue and yellow are for coil B

other order could be red green, blue and yellow

     motor
._____________.
  |	     |
X   X	   X   X
|_B_|      |_A_|
Y   B      G   R


long ta = 3e6;     // acceleration time (microsec)3000000.0

long td = 3e6;     // decelleration time (microsec)3000000.0

long Vm = 1600;    // steady state velocity (pulse/sec)

long Pt = 320880;  // total number of pulses for move (1600 steps per rev)

tamaño del carro 95 cm  aprox 144000 steps is the half of distance

45 cm == 42 


10 cm  1.35

24mm*seg  == 6rev == 9600 pulses


150 dly es lo maximo que lo puedo arrancar


			arduino Valores
_____________________________________________
						LEDS				
			D5	rojo	up lef	
			D6	verde	up lef					                
			D7 	rojo	up right				                
			D8	verde	up right				                
			D9	rojo	down lef				                
			D10	verde	down lef				                
			D11	rojo	down right				              
			D12	verde	down right				              
					SENSOR "DISTANCIA"			              
			D2 									                      
			gnd 								                      
			5v 									                      
					ACELEROMETRO/GYRO			                
			5v									                      
			gnd									                      
			A4	SCL								                    
			A5	SCA								                    
						MOTOR 					                    
			D13									                      
						TRTGGER					                    
			D3 									                                     
						LASER					                      			
			D4									                      



_________________________________________________
					Grados
_________________________________________________
	Pistola		Computadora
	45°				-5°
_________________________________________________


"[2J" + tipoProceso.HighByte + tipoProceso.LowByte + " " + dias(en representación decimal) 
+ "/" + mes(decimal) + "/20" + año(decimal) + " " + idMaquina(decimal) + " " + 
idLlave(decimal) + " " + horas(decimal) + ":" + minutos(decimal) + ":" + segundos(decimal)


.# define led      13

.# define boton    12


void setup()
{
  pinMode(led, OUTPUT);
  pinMode(boton, INPUT);
}

void loop()
{

  int val = 0;
  val = digitalRead(boton);

  if(val == 0)
  {
    digitalWrite(led,LOW);
  }

  if (val == 1)
  {
    digitalWrite(led,HIGH);
  }

}


[DESVIACION_HIGH]
[DESVIACION_LOW]
[DIA]
[MES]
[AÑO]
[ID_MAQUINA_HIGH]
[ID_MAQUINA_LOW]
[ID_LLAVE_HIGH]
[ID_LLAVE_LOW]
[HORAS_HIGH]
[HORAS_LOW]
[MINUTOS_HIGH]
[MINUTOS_LOW]
[SEGUNDOS_HIGH]
[SEGUNDOS_LOW]

int16 Desviacion;
   byte dia;
   byte mes;
   byte anio;
   int16 Id_maquina;
   int16 Id_llave;
   int16 horas;
   int16 minutos;
   int16 segundos;
