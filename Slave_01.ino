
/* Upload this sketch into Crowduino and press reset*/
 
#include <SoftwareSerial.h>   //Software Serial Port
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#define RxD 6
#define TxD 7
#define LIGHT 13
 
SoftwareSerial blueToothSerial(RxD,TxD);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() 
{ 
  Serial.begin(9600);
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  pinMode(LIGHT, OUTPUT);
  digitalWrite(LIGHT, HIGH);
  setupBlueToothConnection(); 
  mlx.begin();
} 
 
void loop() 
{   
  char recvChar;
  while(1){
    if(blueToothSerial.available()){//check if there's any data sent from the remote bluetooth shield
      recvChar = blueToothSerial.read();
      if(recvChar == 'y'){
        digitalWrite(LIGHT, HIGH);
        blueToothSerial.println("Teplota objektu");
        int i;
        float sum = 0;
        for(i = 0; i < 5; i++){
          sum += mlx.readObjectTempC();
          delay(500);
        }
        sum = sum/(i+1);  
        blueToothSerial.println(sum);
        digitalWrite(LIGHT, LOW);
      }  
      if(recvChar == 'n'){
        digitalWrite(LIGHT, HIGH);
        blueToothSerial.println("Teplota okolia");
        int i;
        float sum = 0;
        for(i = 0; i < 5; i++){
          sum += mlx.readAmbientTempC();
          delay(500);
        }
        sum = sum/(i+1);  
        blueToothSerial.println(sum);
        digitalWrite(LIGHT, LOW);
      }  
      Serial.print(recvChar);
    }
    if(Serial.available()){//check if there's any data sent from the local serial terminal, you can add the other applications here
      recvChar  = Serial.read();
      blueToothSerial.print(recvChar);
    }
  }
} 
 
void setupBlueToothConnection()
{
  blueToothSerial.begin(38400); //Set BluetoothBee BaudRate to default baud rate 38400
  blueToothSerial.print("\r\n+STWMOD=0\r\n"); //set the bluetooth work in slave mode
  blueToothSerial.print("\r\n+STNA=CrowBTSlave\r\n"); //set the bluetooth name as "CrowBTSlave"
  blueToothSerial.print("\r\n+STPIN=0000\r\n");//Set SLAVE pincode"0000"
  blueToothSerial.print("\r\n+STOAUT=1\r\n"); // Permit Paired device to connect me
  blueToothSerial.print("\r\n+STAUTO=0\r\n"); // Auto-connection should be forbidden here
  delay(2000); // This delay is required.
  blueToothSerial.print("\r\n+INQ=1\r\n"); //make the slave bluetooth inquirable 
  Serial.println("The slave bluetooth is inquirable!");
  delay(2000); // This delay is required.
  blueToothSerial.flush();
}
