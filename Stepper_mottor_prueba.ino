/*
	This code wiil control the easyDriver and oviously the stepper Motor
*/

// Givens
long time_acc = 3e6;     // acceleration time (microsec)3000000.0
long time_Deceleration = 3e6;     // decelleration time (microsec)3000000.0
long Vm = 2200;    // steady state velocity (pulse/sec)
long pulses_Total = 320880;    // total number of pulses for move (1600 steps per rev)

// Other variables
long dly;           // stepper pulse delay (microsec)
long t = time_Deceleration/9;      // current time (microsec)  -  You need to seed the initial time with something > 0
                    //                             so you don't calculate to long of a delay
long t12;           // time during constant velocity (microsec)

long count = 0;      // count the number of pulses
int Perr = 0;       // error in position
int mov_to =0;
int i = 0;
// Arduino pins
#define dirPin 2
#define stepPin 3
#define ACTION_MOVE_LEFT "IZQUIERDA"
#define ACTION_MOVE_RIGHT "DERECHA"
#define ACTION_STOP "DETENTE"

void setup() {
  Serial.begin(9600);
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  
  // Calculate the time at constant velocity 
  t12 = (pulses_Total/(Vm/1e6))-0.5*(time_acc+time_Deceleration);
  Serial.println(); Serial.println();
  Serial.println(t12);
  Serial.println("Setup Done");
}
void loop()
{	

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
	MovStepper(mov_to);

}


void MovStepper(int direction, long steps_Number)
{
  digitalWrite(dirPin, direction);  // Set the stepper direction

  //Serial.print("dly: "); Serial.print (dly); Serial.println(" microsec");
  //Serial.print ("Current time: "); Serial.print(t); Serial.println(" microsec"); 
  // Move stepper one pulse using delay just calculated
  for (int i = 0; i<=steps_Number; i++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(160);//160 dly
    digitalWrite(stepPin, LOW);
    delayMicroseconds(160);
  }
  count ++;
 
  
}

void dispatchAction( const char *action )
{  
  if ( strcmp( action, ACTION_MOVE_LEFT ) == 0 ) {
    mov_to = 0;
  }  
  else if ( strcmp( action, ACTION_MOVE_RIGHT ) == 0 ) {
    mov_to = 1;
  }  
  else if (strcmp (action, ACTION_STOP)==0)
  {
    pulses_Total= 0;
  }
  else {
    unknownAction( action );
  }
}

void unknownAction(String action)
{
  Serial.print( "Unknown action: " );
  Serial.println( action );
}
