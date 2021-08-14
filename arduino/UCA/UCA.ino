#include <SPI.h>
#include <LoRa.h>

#define LORA_BAND 923E6
#define NSS 10
#define NRST 8
#define DIO 6

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Initializing lora receiver");
  
}

void loop() {
  // put your main code here, to run repeatedly:
  ReceivePacket();
}

void SetupLora()
{
  SPI.begin();
  LoRa.setPins(NSS, NRST, DIO);
  int result = LoRa.begin(LORA_BAND);
  if (result != 1) {
    Serial.println("Fail initialization");
    while (1);
  }
  Serial.println("LoRa initialized");
  delay(500);
}
void ReceivePacket()
{
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");
    // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }
    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}
