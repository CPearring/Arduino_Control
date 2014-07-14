// demo: CAN-BUS Shield, receive data
#include <mcp_can.h>
#include <SPI.h>
#include <Bridge.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char buf[64] = " ";
char l_Buf[6];
char r_Buf[6];
signed short r_RPM = 0;
signed short l_RPM = 0;

void setup()
{
  //Serial.begin(115200);
  //while( !Serial ){ ; }
  Bridge.begin();
  CAN.begin(CAN_1000KBPS); // init can bus : baudrate = 1M
  pinMode(2, INPUT); // Setting pin 2 for /INT input
  Bridge.put("RPM_STATUS", "0:0");
}

void loop()
{
    if(!digitalRead(2)) // If pin 2 is low, can message has been recieved. read receive buffer
    {
      CAN.readMsgBuf(&len, rxBuf); // Read data: len = data length, buf = data byte(s)
      rxId = CAN.getCanId(); // Get message ID
      if( rxId == 0x213 ) {//Left side can bus message recieved
        r_RPM = 0;//Magic. See GRDSControlAPI file for details
        r_RPM = rxBuf[2];
        r_RPM = r_RPM | ((long)(rxBuf[3]))<<8;
        //Serial.println(String(l_RPM)+":"+String(r_RPM));
        (String(l_RPM) + ":" + String(r_RPM)).toCharArray(buf, 64);
        Bridge.put("RPM_STATUS", buf);
      }
      else if( rxId == 0x212 ) {//Right side can bus message recieved
        l_RPM = 0;//Magic. See GRDSControlAPI file for details
        l_RPM = l_RPM | rxBuf[2];
        l_RPM = l_RPM | ((long)(rxBuf[3]))<<8;
        //Serial.println(String(l_RPM)+":"+String(r_RPM));
        (String(l_RPM) + ":" + String(r_RPM)).toCharArray(buf, 64);
        Bridge.put("RPM_STATUS", buf);
      }
    }
    unsigned char stmp[6] = {0, 0, 0, 0, 0, 0};//Raw CAN message
    
    Bridge.get("SET_L_RPM", l_Buf, 6);//Get a string off the bridge (Left)
    short l = atoi(l_Buf); //Convert to an int
    Bridge.get("SET_R_RPM", r_Buf, 6);//Get a string off the bridge (Right)
    short r = atoi(r_Buf); //Convert to an int
    
    //Compose CAN message
    stmp[0] = l%256;
    stmp[1] = l>>8;
    stmp[2] = r%256;
    stmp[3] = r>>8;
    
    CAN.sendMsgBuf(0x112, 0, 6, stmp);//Send message
    /*
    l_Buf[0] = 0;
    l_Buf[1] = 0;
    l_Buf[2] = 0;
    Bridge.get("SET_L_RPM", l_Buf, 3);
    short l = atoi(l_Buf);
    Serial.print("Left:");
    Serial.println(l);
    Serial.println(l_Buf);
    //Bridge.put("LEFT_RPM", buf);
    unsigned char stmp[6] = {0, 0, 0, 0, 0, 0};
    
    r_Buf[0] = 0;
    r_Buf[1] = 0;
    r_Buf[2] = 0;
    Bridge.get("SET_R_RPM", r_Buf, 3);
    short r = atoi(r_Buf);
    //Serial.println(r_Buf);
    stmp[3] = r%256;
    stmp[2] = r<<8;
    
    CAN.sendMsgBuf(0x112, 0, 6, stmp);*/
    //delay(50);
}


