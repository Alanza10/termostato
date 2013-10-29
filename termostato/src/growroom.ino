/*
 * control de growroom 
 * pin usados digitales 3 (relay termostato) 5 (temp superior)
 * version 0.1 : temperatura am2301 1 termometro superior
 * version 0.2 : control relay 
 * version 0.3 : control relay termostato configurar maximo y margen 
 */

#include <dht.h>
#include <Rele.h>

dht DHT;
//para control y protocolo
#define MSG_LEN  6
#define TEMP_HEADER 'T'
#define TEMP_SUP 'S'
#define TERMOSUP 5
#define RELAY_HEADER  'R'
#define RELAY_ON  'E'  
#define RELAY_OFF  'A'
#define RELAY_TERM 3
#define CONFIG_HEADER  'C'
#define CONFIG_MAX 'M'
#define CONFIG_MARGEN 'R'
#define DEF_MARGEN_INIT 10
#define DEF_TEMP_MAX 35

Rele termrele;
int temp_max=DEF_TEMP_MAX,margen=DEF_MARGEN_INIT;

void setup()
{
  Serial.begin(115200);
  Serial.println("CONTROL GROWROOM");
  Serial.println("********************");
  Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");
  //setReles
  termrele.setRele(1);
  termrele.setPin(RELAY_TERM);
  termrele.setOutput();
  termrele.releOff();
 
}

void loop()
{
  if(Serial.available())
  {
	processMessage();
  }
  termostato();
  delay(1000);
}

// procesar mensajes serial

void processMessage(){
 
  while(Serial.available() >=  MSG_LEN ){ 
	  char command[MSG_LEN-1]; 
	  char cmd = Serial.read() ; 
	  
	  if( cmd == TEMP_HEADER ) { 
		  char c = Serial.read();
		  //resto chars comando
	       for(int i=0; i < MSG_LEN -2; i++){   
	        command[i] = Serial.read();           
	      }
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
	  
	  if( cmd == RELAY_HEADER ) { 
		  char c = Serial.read();
		  //resto chars comando
	       for(int i=0; i < MSG_LEN -2; i++){   
	        command[i] = Serial.read();           
	      }
		  int pos;
		  if(c==RELAY_ON){
			  termrele.releOn();
		  }else if(c==RELAY_OFF){
			  termrele.releOff();
		  }
	  } 
	  if( cmd == CONFIG_HEADER ) { 
		  char c = Serial.read();
		  char c2 = Serial.read();
		  //resto chars comando
	       for(int i=0; i < MSG_LEN -3; i++){   
	        command[i] = Serial.read();           
	      }
		  int pos;
		  if(c==CONFIG_MAX && c2==CONFIG_MARGEN){
			  Serial.print("config max temp");
			  Serial.print(temp_max);
			  Serial.print(" margen ");
			  Serial.println(margen);
		  }
	  }
  }
}

void termostato(){
	int chk = DHT.read21(TERMOSUP);
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
	  if(DHT.temperature>=temp_max && !termrele.isOn())termrele.releOn();
	  else if(DHT.temperature<=temp_max-margen && termrele.isOn()) termrele.releOff();
	
}
