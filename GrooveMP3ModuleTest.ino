#include <M5StickC.h>

// EspSoftwareSerial
#include <SoftwareSerial.h>

SoftwareSerial mySerial;
byte paramBuffer[16];

void setup() {
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Axp.ScreenBreath(12);
  M5.Lcd.fillScreen(BLACK);

  M5.Lcd.setTextFont(2);
  M5.Lcd.setTextColor(GREEN);
  M5.Lcd.setCursor(0, 10);

  M5.Lcd.printf("GroveMP3ModuleTest");

  Serial.begin(115200);

  mySerial.begin(9600, SWSERIAL_8N1, /* rx */33, /* tx */32 , false, 256);

  delay(100);
  GroveMP3V3_SetVolume(20); // 1-31
}

void loop() {
  M5.update();
  if ( M5.BtnA.wasPressed() ) {
    Serial.println("BtnA presswd");
    GroveMP3V3_PlaySDRootSong(1);
  }
}

// see also... 
// https://github.com/Seeed-Studio/Seeed_Serial_MP3_Player/blob/master/src/WT2003S_Player.h
// https://github.com/Seeed-Studio/Seeed_Serial_MP3_Player/blob/master/src/WT2003S_Player.cpp
// https://uepon.hatenadiary.com/entry/2019/12/29/164445
void GroveMP3V3_SetVolume(uint val)
{
  paramBuffer[0] = val;
  GroveMP3V3_WriteCommand(0xAE, paramBuffer, 1);
}

void GroveMP3V3_PlaySDRootSong(uint16_t val)
{
  paramBuffer[0] = (byte)((val >> 8) & 0xFF);
  paramBuffer[1] = (byte)(0xFF & val);
  GroveMP3V3_WriteCommand(0xA2, paramBuffer, 2);
}

// see also...
// https://uepon.hatenadiary.com/entry/2019/12/27/223818
void GroveMP3V3_WriteCommand(const byte &commandCode, const byte *parameter, const int parameterSize)
{
  byte length = 1 + 1 + (byte)parameterSize + 1;
  byte sum = 0;

  sum += length;
  sum += commandCode;
  for (int i = 0; i < parameterSize; i++) sum += parameter[i];

  mySerial.write(0x7E);
  mySerial.write(length);
  mySerial.write(commandCode);
  mySerial.write(parameter, parameterSize); 
  mySerial.write(sum);
  mySerial.write(0xEF);
}
