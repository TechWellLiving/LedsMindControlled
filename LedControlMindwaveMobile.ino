/*
* Arduino Bluetooth Interface with Mindwave Mobile
* This is example code provided by NeuroSky, Inc. and is provided License Free
* Copyright (c) NeuroSky. All Rights Reserved
*/


/************ THINKGEAR COMMUNICATIONS PROTOCOL ***********/

//ThinkGear Packets:

/*[SYNC] [SYNC] [PLENGTH]  [PAYLOAD...]     [CHKSUM]
  _______________________  _____________  ____________
  ^^^^^^^^(Header)^^^^^^^  ^^(Payload)^^  ^(Checksum)^*/


//ThinkGear Data Values:

//Single-Byte Codes
#define MEDIT_CMD               0x05
#define ATTEN_CMD               0x04
#define POOR_QUAL_CMD           0x02

//Multi-Byte Codes
#define RAW_CMD                 0x80
#define ASIC_EEG_POWER_INT_CMD  0x83

#define SYNC_BYTE               0xAA  



#define GREENLED1  3 
#define GREENLED2  4
#define GREENLED3  5
#define YELLOWLED1 6
#define YELLOWLED2 7
#define YELLOWLED3 8
#define YELLOWLED4 9
#define REDLED1    10
#define REDLED2    11
#define REDLED3    12
#define LED 13
#define BAUDRATE 57600



byte generatedChecksum = 0;
byte checksum = 0; 
int payloadLength = 0;
byte payloadData[64] = {0};
byte poorQuality = 0;
byte attention = 0;
byte meditation = 0;
long lastReceivedPacket = 0;
boolean bigPacket = false;


void setup() {            

  pinMode(GREENLED1, OUTPUT);
  pinMode(GREENLED2, OUTPUT);
  pinMode(GREENLED3, OUTPUT);
  pinMode(YELLOWLED1, OUTPUT);
  pinMode(YELLOWLED2, OUTPUT);
  pinMode(YELLOWLED3, OUTPUT);
  pinMode(YELLOWLED4, OUTPUT);
  pinMode(REDLED1, OUTPUT);
  pinMode(REDLED2, OUTPUT);
  pinMode(REDLED3, OUTPUT);
  pinMode(LED, OUTPUT);
  
  Serial.begin(BAUDRATE);       
                               
}

/*
* Read data from UART 
*/
byte ReadOneByte() { 
  int ByteRead;
                            
  while(!Serial.available());
  ByteRead = Serial.read();


  return ByteRead;
}



void loop() {

  // Wait for 2 sync bytes
  if(ReadOneByte() == SYNC_BYTE) {  
    if(ReadOneByte() == SYNC_BYTE) {
      payloadLength = ReadOneByte();
	  //Payload length can not be greater than 169	  
      if(payloadLength > 169) 
          return;

      generatedChecksum = 0;        
      for(int i = 0; i < payloadLength; i++) {  
        payloadData[i] = ReadOneByte();            //Get payload Data
        generatedChecksum += payloadData[i];
      }   

      checksum = ReadOneByte();                      //Read checksum byte   
      generatedChecksum = 255 - generatedChecksum;   //Take one's compliment of generated checksum

        if(checksum == generatedChecksum) {    

        poorQuality = 200;
        attention = 0;
        meditation = 0;

		//Parse the payload
        for(int i = 0; i < payloadLength; i++) {    
          switch (payloadData[i]) {
          case POOR_QUAL_CMD:
            i++;            
            poorQuality = payloadData[i];
            bigPacket = true;            
            break;
          case ATTEN_CMD:
            i++;
            attention = payloadData[i];                        
            break;
          case MEDIT_CMD:
            i++;
            meditation = payloadData[i];
            break;
          case RAW_CMD:
            i = i + 3;  //Not used in this example 
            break;
          case ASIC_EEG_POWER_INT_CMD:
            i = i + 25; //Not used in this example
                             
            break;
          default:
            break;
          } // switch
        } // for loop
       
        if(bigPacket) { 
          if(poorQuality == 0)
            digitalWrite(LED, HIGH);//Good quality signal measured by Mindwave Mobile (ThinkGear electrodes)
       
          Serial.print("PoorQuality: ");
          Serial.print(poorQuality, DEC);
          Serial.print(" Attention: ");
          Serial.print(attention, DEC);
          Serial.print(" Time since last packet: ");
          Serial.print(millis() - lastReceivedPacket, DEC);
          lastReceivedPacket = millis();//Time since program excecution 
          Serial.print("\n");

          switch(attention / 10) { 
                                  
          case 0:
            digitalWrite(GREENLED1, HIGH);
            digitalWrite(GREENLED2, LOW);
            digitalWrite(GREENLED3, LOW);
            digitalWrite(YELLOWLED1, LOW);
            digitalWrite(YELLOWLED2, LOW);
            digitalWrite(YELLOWLED3, LOW);
            digitalWrite(YELLOWLED4, LOW);
            digitalWrite(REDLED1, LOW);
            digitalWrite(REDLED2, LOW);
            digitalWrite(REDLED3, LOW);           
            break;
          case 1:
            digitalWrite(GREENLED1, HIGH);
            digitalWrite(GREENLED2, HIGH);
            digitalWrite(GREENLED3, LOW);
            digitalWrite(YELLOWLED1, LOW);
            digitalWrite(YELLOWLED2, LOW);
            digitalWrite(YELLOWLED3, LOW);
            digitalWrite(YELLOWLED4, LOW);
            digitalWrite(REDLED1, LOW);
            digitalWrite(REDLED2, LOW);
            digitalWrite(REDLED3, LOW);
            break;
          case 2:
            digitalWrite(GREENLED1, HIGH);
            digitalWrite(GREENLED2, HIGH);
            digitalWrite(GREENLED3, HIGH);
            digitalWrite(YELLOWLED1, LOW);
            digitalWrite(YELLOWLED2, LOW);
            digitalWrite(YELLOWLED3, LOW);
            digitalWrite(YELLOWLED4, LOW);
            digitalWrite(REDLED1, LOW);
            digitalWrite(REDLED2, LOW);
            digitalWrite(REDLED3, LOW);
            break;
          case 3:              
            digitalWrite(GREENLED1, HIGH);
            digitalWrite(GREENLED2, HIGH);
            digitalWrite(GREENLED3, HIGH);              
            digitalWrite(YELLOWLED1, HIGH);
            digitalWrite(YELLOWLED2, LOW);
            digitalWrite(YELLOWLED3, LOW);
            digitalWrite(YELLOWLED4, LOW);
            digitalWrite(REDLED1, LOW);
            digitalWrite(REDLED2, LOW);
            digitalWrite(REDLED3, LOW);             
            break;
          case 4:
            digitalWrite(GREENLED1, HIGH);
            digitalWrite(GREENLED2, HIGH);
            digitalWrite(GREENLED3, HIGH);              
            digitalWrite(YELLOWLED1, HIGH);
            digitalWrite(YELLOWLED2, HIGH);
            digitalWrite(YELLOWLED3, LOW);
            digitalWrite(YELLOWLED4, LOW);
            digitalWrite(REDLED1, LOW);
            digitalWrite(REDLED2, LOW);
            digitalWrite(REDLED3, LOW);              
            break;
          case 5:
            digitalWrite(GREENLED1, HIGH);
            digitalWrite(GREENLED2, HIGH);
            digitalWrite(GREENLED3, HIGH);              
            digitalWrite(YELLOWLED1, HIGH);
            digitalWrite(YELLOWLED2, HIGH);
            digitalWrite(YELLOWLED3, HIGH);
            digitalWrite(YELLOWLED4, LOW);
            digitalWrite(REDLED1, LOW);
            digitalWrite(REDLED2, LOW);
            digitalWrite(REDLED3, LOW);               
            break;
          case 6:              
            digitalWrite(GREENLED1, HIGH);
            digitalWrite(GREENLED2, HIGH);
            digitalWrite(GREENLED3, HIGH);              
            digitalWrite(YELLOWLED1, HIGH);
            digitalWrite(YELLOWLED2, HIGH);
            digitalWrite(YELLOWLED3, HIGH);
            digitalWrite(YELLOWLED4, HIGH);
            digitalWrite(REDLED1, LOW);
            digitalWrite(REDLED2, LOW);
            digitalWrite(REDLED3, LOW);              
            break;
          case 7:
            digitalWrite(GREENLED1, HIGH);
            digitalWrite(GREENLED2, HIGH);
            digitalWrite(GREENLED3, HIGH);              
            digitalWrite(YELLOWLED1, HIGH);
            digitalWrite(YELLOWLED2, HIGH);
            digitalWrite(YELLOWLED3, HIGH);
            digitalWrite(YELLOWLED4, HIGH);
            digitalWrite(REDLED1, HIGH);
            digitalWrite(REDLED2, LOW);
            digitalWrite(REDLED3, LOW);              
            break;    
          case 8:
            digitalWrite(GREENLED1, HIGH);
            digitalWrite(GREENLED2, HIGH);
            digitalWrite(GREENLED3, HIGH);              
            digitalWrite(YELLOWLED1, HIGH);
            digitalWrite(YELLOWLED2, HIGH);
            digitalWrite(YELLOWLED3, HIGH);
            digitalWrite(YELLOWLED4, HIGH);
            digitalWrite(REDLED1, HIGH);
            digitalWrite(REDLED2, HIGH);
            digitalWrite(REDLED3, LOW);
            break;
          case 9:
            digitalWrite(GREENLED1, HIGH);
            digitalWrite(GREENLED2, HIGH);
            digitalWrite(GREENLED3, HIGH);              
            digitalWrite(YELLOWLED1, HIGH);
            digitalWrite(YELLOWLED2, HIGH);
            digitalWrite(YELLOWLED3, HIGH);
            digitalWrite(YELLOWLED4, HIGH);
            digitalWrite(REDLED1, HIGH);
            digitalWrite(REDLED2, HIGH); 
            digitalWrite(REDLED3, HIGH);
            break;
          case 10:
            digitalWrite(GREENLED1, HIGH);
            digitalWrite(GREENLED2, HIGH);
            digitalWrite(GREENLED3, HIGH);              
            digitalWrite(YELLOWLED1, HIGH);
            digitalWrite(YELLOWLED2, HIGH);
            digitalWrite(YELLOWLED3, HIGH);
            digitalWrite(YELLOWLED4, HIGH);
            digitalWrite(REDLED1, HIGH);
            digitalWrite(REDLED2, HIGH); 
            digitalWrite(REDLED3, HIGH);
            break;           
          }                     
        }     
        bigPacket = false;        
      }
      else {
        // Checksum Error
      }  // end if else for checksum
    } // end if read 0xAA byte
  } // end if read 0xAA byte
}



