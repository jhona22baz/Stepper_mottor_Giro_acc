#include <Wire.h>

#define MPU6050_ACCEL_XOUT_H       0x3B   // R  
#define MPU6050_PWR_MGMT_1         0x6B   // R/W
#define MPU6050_PWR_MGMT_2         0x6C   // R/W
#define MPU6050_WHO_AM_I           0x75   // R
// Default I2C address for the MPU-6050 is 0x68.
// But only if the AD0 pin is low.
// Some sensor boards have AD0 high, and the
// I2C address thus becomes 0x69.
#define MPU6050_I2C_ADDRESS 0x68

/*
Definir I/O Arduino
*/
#define inDistanceSensor  2   
#define inTrigger         3   
#define outLaser          4   
#define ledRedUpLeft      5   
#define ledGreenUpLeft    6   
#define ledRedUpRight     7   
#define ledGreenUpRight   8   
#define ledRedDownLeft    9   
#define ledGreenDownLeft  10  
#define ledRedDownRight   11  
#define ledGreenDownRight 12  
#define Motor             13
/*********************************/
#define ACTION_START "COMIENZA"
#define ACTION_FINISHED "TERMINA"
#define ERROR "ERROR"

// The AVR chip (on the Arduino board) has the Low Byte 
// at the lower address.
// But the MPU-6050 has a different order: High Byte at
// lower address, so that has to be corrected.
// The register part "reg" is only used internally, 
// and are swapped in code.
typedef union accel_t_gyro_union
{
  struct
  {
    uint8_t x_accel_h;
    uint8_t x_accel_l;
    uint8_t y_accel_h;
    uint8_t y_accel_l;
    uint8_t z_accel_h;
    uint8_t z_accel_l;
    uint8_t t_h;
    uint8_t t_l;
    uint8_t x_gyro_h;
    uint8_t x_gyro_l;
    uint8_t y_gyro_h;
    uint8_t y_gyro_l;
    uint8_t z_gyro_h;
    uint8_t z_gyro_l;
  } reg;
  struct 
  {
    int16_t x_accel;
    int16_t y_accel;
    int16_t z_accel;
    int16_t temperature;
    int16_t x_gyro;
    int16_t y_gyro;
    int16_t z_gyro;
  } value;
};

int inSensor = 2;
int val = 0;
int i = 0;
float ACCEL_XANGLE, ACCEL_YANGLE,yAngle,xAngle;
void setup()
{      
  pinMode(inDistanceSensor,INPUT);
  pinMode(inTrigger,INPUT);
  pinMode(outLaser,OUTPUT);
  pinMode(ledRedUpLeft,OUTPUT);
  pinMode(ledGreenUpLeft,OUTPUT);
  pinMode(ledRedUpRight,OUTPUT);
  pinMode(ledGreenUpRight,OUTPUT);
  pinMode(ledRedDownLeft,OUTPUT);
  pinMode(ledGreenDownLeft,OUTPUT);
  pinMode(ledRedDownRight,OUTPUT);
  pinMode(ledGreenDownRight,OUTPUT);
  pinMode(Motor,OUTPUT);

  int error;
  uint8_t c;

  Serial.begin(9600);
  //Serial.println(F("InvenSense MPU-6050"));

  // Initialize the 'Wire' class for the I2C-bus.
  Wire.begin();

  // default at power-up:
  //    Gyro at 250 degrees second
  //    Acceleration at 2g
  //    Clock source at internal 8MHz
  //    The device is in sleep mode.
  //
  error = MPU6050_read (MPU6050_WHO_AM_I, &c, 1);
/*  Serial.print(F("WHO_AM_I : "));
  Serial.print(c,HEX);
  Serial.print(F(", error = "));
  Serial.println(error,DEC);
*/
  // According to the datasheet, the 'sleep' bit
  // should read a '1'. But I read a '0'.
  // That bit has to be cleared, since the sensor
  // is in sleep mode at power-up. Even if the
  // bit reads '0'.
  error = MPU6050_read (MPU6050_PWR_MGMT_2, &c, 1);
  /*Serial.print(F("PWR_MGMT_2 : "));
  Serial.print(c,HEX);
  Serial.print(F(", error = "));
  Serial.println(error,DEC);
*/

  // Clear the 'sleep' bit to start the sensor.
  MPU6050_write_reg (MPU6050_PWR_MGMT_1, 0);
}


void loop()
{
  int error;
  double dT;
  float ACCEL_XANGLE, ACCEL_YANGLE,yAngle,xAngle;
  accel_t_gyro_union accel_t_gyro;
   
   char buffer[ 1024 ];
    
   while ( Serial.peek() != -1 ) {
    char c = Serial.read();
             
    if ( c == '\n' ) {
      buffer[ i ] = '\0';
      dispatchAction( buffer );
      i = 0;
    }
    else {
      buffer[ i++ ] = c;
    }
  }
        
     if(!digitalRead(inTrigger))
    {
    val = digitalRead(inSensor);   
      if(val==0){ 
        
  error = MPU6050_read (MPU6050_ACCEL_XOUT_H, (uint8_t *) &accel_t_gyro, sizeof(accel_t_gyro));
  
  uint8_t swap;
  #define SWAP(x,y) swap = x; x = y; y = swap

  SWAP (accel_t_gyro.reg.x_accel_h, accel_t_gyro.reg.x_accel_l);
  SWAP (accel_t_gyro.reg.y_accel_h, accel_t_gyro.reg.y_accel_l);
  SWAP (accel_t_gyro.reg.z_accel_h, accel_t_gyro.reg.z_accel_l);
  SWAP (accel_t_gyro.reg.t_h, accel_t_gyro.reg.t_l);
  SWAP (accel_t_gyro.reg.x_gyro_h, accel_t_gyro.reg.x_gyro_l);
  SWAP (accel_t_gyro.reg.y_gyro_h, accel_t_gyro.reg.y_gyro_l);
  SWAP (accel_t_gyro.reg.z_gyro_h, accel_t_gyro.reg.z_gyro_l);

  //ACCEL_XANGLE = 57.295*atan((float)accel_t_gyro.value.y_accel/ sqrt(pow((float)accel_t_gyro.value.z_accel,2)+pow((float)accel_t_gyro.value.x_accel,2)));
  //ACCEL_YANGLE = 57.295*atan((float)-accel_t_gyro.value.x_accel/ sqrt(pow((float)accel_t_gyro.value.z_accel,2)+pow((float)accel_t_gyro.value.y_accel,2)));     

  yAngle = Angle_O(accel_t_gyro.value.x_accel,accel_t_gyro.value.y_accel,accel_t_gyro.value.z_accel);
  xAngle = Angle_O(accel_t_gyro.value.y_accel,accel_t_gyro.value.x_accel,accel_t_gyro.value.z_accel);
  // Print the raw acceleration values
 revisarAngle(xAngle,yAngle);
 
    }
 } 
}

void printAngle(float angle_x,float angle_y)
{
  Serial.print(F("angle_X-- "));
  Serial.print(angle_x, DEC);
  Serial.print(F(" angle_Y--- "));
  Serial.print(angle_y, DEC);
  Serial.println(F(""));
}
void revisarAngle(float angleX,float angleY)
{
  if(angleY>=30)
  {
    Serial.print(F("ABAJO "));
                printAngle(angleX,angleY);
          delay(300);
  }
  if(angleY<=-30)
  {
    Serial.print(F("ARRIBA "));
                printAngle(angleX,angleY);
        delay(300);
  }
  if(angleX>=30)
  {
    Serial.print("DERECHA ");
            printAngle(angleX,angleY);
        delay(300);
  }
  if(angleX<=-30)
  {
    Serial.print("IZQUIERDA ");
            printAngle(angleX,angleY);
    delay(300);
  }
  if((angleX<=20&&angleX>=-20)&&(angleY<=20&&angleY>=-20))
  {
    Serial.print("OK ");
            printAngle(angleX,angleY);
        delay(300);
        
  }
}

/*
*   
*/
void dispatchAction( const char *action )
{  
  if ( strcmp( action, ACTION_START ) == 0 ) {
    comienza();
  }  
  else if ( strcmp( action, ACTION_FINISHED ) == 0 ) {
    termina();
  }
  else if (strcmp(action,ERROR)==0)
  {
    error();
  }
  else {
    unknownAction( action );
  }
}

void error()
{
  Serial.println("ERROR");
}
void comienza()
{
  Serial.println("Comienza la prueba");
}

void termina()
{
  Serial.println("Termina la prueba");
}
/*
*	For obtaining Y angle needs x_value, y_value, z_value
*	For obtaining X angle needs y_value, x_value, z_value
*/
float Angle_O(int16_t val_1, int16_t val_2,int16_t val_z)
{        
        //57 es la conversion de radianes a angunos 180/3.14159
	return  57.295*atan((float)val_1 / sqrt(pow((float)val_z,2) +pow((float)val_2,2)));
}

void unknownAction(String action)
{
  Serial.print( "Unknown action: " );
  Serial.println( action );
}

int MPU6050_read(int start, uint8_t *buffer, int size)
{
  int i, n, error;

  Wire.beginTransmission(MPU6050_I2C_ADDRESS);
  n = Wire.write(start);
  if (n != 1)
    return (-10);

  n = Wire.endTransmission(false);    // hold the I2C-bus
  if (n != 0)
    return (n);

  // Third parameter is true: relase I2C-bus after data is read.
  Wire.requestFrom(MPU6050_I2C_ADDRESS, size, true);
  i = 0;
  while(Wire.available() && i<size)
  {
    buffer[i++]=Wire.read();
  }
  if ( i != size)
    return (-11);

  return (0);  // return : no error
}


int MPU6050_write(int start, const uint8_t *pData, int size)
{
  int n, error;

  Wire.beginTransmission(MPU6050_I2C_ADDRESS);
  n = Wire.write(start);        // write the start address
  if (n != 1)
    return (-20);

  n = Wire.write(pData, size);  // write data bytes
  if (n != size)
    return (-21);

  error = Wire.endTransmission(true); // release the I2C-bus
  if (error != 0)
    return (error);

  return (0);         // return : no error
}

int MPU6050_write_reg(int reg, uint8_t data)
{
  int error;

  error = MPU6050_write(reg, &data, 1);

  return (error);
}