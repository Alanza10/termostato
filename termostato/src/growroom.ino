/*
 * control de growroom 
 * version 0.1 : temperatura am2301
 */

#include <dht.h>

dht DHT;
//para control y protocolo
#define MSG_LEN  2
#define TEMP_HEADER 'T'
#define TEMP_SUP 'S'
#define TERMOSUP 5


void setup()
{
  Serial.begin(115200);
  Serial.println("CONTROL GROWROOM");
  Serial.println("********************");
  Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");
}

void loop()
{
  if(Serial.available())
  {
	processMessage();
  }
  delay(1000);
}

// procesar mensajes serial

void processMessage(){
 
  while(Serial.available() >=  MSG_LEN ){  
	  char cmd = Serial.read() ;  
	  if( cmd == TEMP_HEADER ) { 
		  char c = Serial.read();
		  int pos;
		  if(c==TEMP_SUP){
			  // READ DATA Sup
			  pos = TERMOSUP;
			  Serial.print("Termosuperior, \t");
		  }
		  
		  int chk = DHT.read21(pos);
		  switch (chk)
		  {
		    case DHTLIB_OK:  
				Serial.print("OK,\t"); 
				break;
		    case DHTLIB_ERROR_CHECKSUM: 
				Serial.print("Checksum error,\t"); 
				break;
		    case DHTLIB_ERROR_TIMEOUT: 
				Serial.print("Time out error,\t"); 
				break;
		    default: 
				Serial.print("Unknown error,\t"); 
				break;
		  }
		  // DISPLAY DATA
		  Serial.print(DHT.humidity, 1);
		  Serial.print(",\t");
		  Serial.println(DHT.temperature, 1);
	  }
  }  
}


