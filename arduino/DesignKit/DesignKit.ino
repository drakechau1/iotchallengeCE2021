#define REGION R_AS923
#define TX_INTERVAL 5
#define DHT_PIN A8
#define MQ_PIN A3
/*Include libraries*/
#include <LibLacuna.h>
#include <SPI.h>
#include <DHT.h>
#include <DHT_U.h>
DHT dht(DHT_PIN, DHT11);
#include <MQ2.h>
MQ2 mq2(MQ_PIN);

static char payload[255];
const String mytext = "test message";
static lsLoraTxParams txParams;
byte rxenable = 1;
unsigned long currentTime;

void setup() {
  Serial.begin(9600);
   
  pinMode(LS_LED_BLUE, OUTPUT);
  pinMode(LS_GPS_ENABLE, OUTPUT);
  digitalWrite(LS_GPS_ENABLE, LOW);
  pinMode(LS_GPS_V_BCKP, OUTPUT);
  digitalWrite(LS_GPS_V_BCKP, LOW);
  
  while (!Serial);

  delay(2000);
  SetupLora();
  SetupSensor();
  currentTime = millis();
}

void loop() {
  // Send message
  if ((unsigned long)(millis() - currentTime) > TX_INTERVAL*1000)
  {
    digitalWrite(LS_LED_BLUE, HIGH);
    delay(50);
    digitalWrite(LS_LED_BLUE, LOW);
    delay(100);
    digitalWrite(LS_LED_BLUE, HIGH);
    delay(50);
    digitalWrite(LS_LED_BLUE, LOW);
    SendPacket();
    currentTime = millis();
  }
  // Receive message
  ReceivePacket();

  delay(1000);
}

/*Detail functions*/
void SetupLora()
{
  Serial.println("Start to setup LoRa (Asia 923 Mhz)...");
  // SX1262 configuration for lacuna LS200 board
  lsSX126xConfig cfg;
  lsCreateDefaultSX126xConfig(&cfg, BOARD_VERSION);
  // Initialize SX1262
  int result = lsInitSX126x(&cfg, REGION);
  Serial.print("E22/SX1262: ");
  Serial.println(lsErrorToString(result));
  // transmission parameters for terrestrial LoRa
  lsCreateDefaultLoraTxParams(&txParams, REGION);
  txParams.spreadingFactor = lsLoraSpreadingFactor_7;
  txParams.frequency = 923000000;
  Serial.print("Terrestrial Uplink Frequency: ");
  Serial.println(txParams.frequency/1e6);
}
void SendPacket()
{
  float humi = 0, temp = 0, gas = 200.0;
  humi = GetHumidity();
  temp = GetTemperature();
  gas = GetGas();
  String a = String(temp);
  String b = String(humi);
  String c = String(gas);
  String message = String(a + "|" + b + "|" + c);
  Serial.println("Sending packet: " + message);
  message.toCharArray(payload, 255); 
  int lora_result  = lsSendLora(&txParams, (byte *)payload, message.length(), rxenable);
  if (lora_result == LS_OK)
    Serial.println("Transmitted");
  Serial.print("Result: ");
  Serial.println(lsErrorToString(lora_result));
}
void ReceivePacket()
{
    int recv_result = lsReceiveLora(&txParams, (byte *)payload);
    if (recv_result == LS_OK) {
      Serial.println("Downlink received");
      Serial.print("SNR: ");
      Serial.println(txParams.snr);
      Serial.print("RSSI: ");
      Serial.println(txParams.rssi);
      Serial.print("SignalRSSI: ");
      Serial.println(txParams.signalrssi);
      Serial.print("Payload: ");
    
      for (char n = 0; n < 10; n++)
          {
            Serial.print (payload[n],HEX);
            Serial.write (" ");
          }
      Serial.println();
    }
}
void SetupSensor()
{
  // DHT11
  dht.begin();
  // MQ2
  mq2.begin();
}
float GetTemperature()
{
  return dht.readTemperature();
}
float GetHumidity()
{
  return dht.readHumidity(); 
}
int GetGas()
{
  return mq2.readLPG();
}
