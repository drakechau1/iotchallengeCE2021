/*Include libraries*/
#include <SPI.h>
#include <LoRa.h>

#define LORA_BAND 923E6

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial);
  Serial.print("Start to setup LoRa...");
  SetupLora();
}

void loop() {
  // put your main code here, to run repeatedly:
  SendPacket();
  delay(3000);
}

/*Detail functions*/
void SetupLora()
{
  SPI.begin();
  LoRa.setPins(E22_NSS, E22_NRST, E22_DIO1);
  int result = LoRa.begin(LORA_BAND);
  if (result != 1) {
    Serial.println("Fail initialization");
    while (1);
  }
  Serial.println("LoRa initialized");
  delay(500);
}
void SendPacket()
{
  Serial.println("Sending packet: ");
  int humi = 50, temp = 27;
  LoRa.beginPacket();
  LoRa.print(humi);
  LoRa.print(".");
  LoRa.print(temp);
  LoRa.endPacket();
  /*--*/
  Serial.print(humi);
  Serial.print(".");
  Serial.print("temp");
}
//void ReceivePacket();
//void SetupSensor();
//void GetTemperature();
//void GetHumidity();
