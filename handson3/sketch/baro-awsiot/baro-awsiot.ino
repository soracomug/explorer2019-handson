#include <PubSubClient.h>     // https://github.com/SeeedJP/pubsubclient
#include <WioLTEClient.h>
#include <GroveDriverPack.h>  // https://github.com/SeeedJP/GroveDriverPack

#define APN       "soracom.io"
#define USERNAME  "sora"
#define PASSWORD  "sora"

#define MQTT_SERVER_HOST  "beam.soracom.io"
#define MQTT_SERVER_PORT  (1883)

// Designing MQTT Topics for AWS IoT Core
#define APPLICATION     "soracomug-handson"
#define CONTEXT         "techplayshibuya"
#define THING           "BaroDevice1"
#define TELEMETRY_TOPIC "dt/" APPLICATION "/" CONTEXT "/" THING

#define INTERVAL  (5000)

WioCellular Wio;

WioLTEClient WioClient(&Wio);
PubSubClient MqttClient;

GroveBoard Board;
OmronBaro2SMPB02E Sensor(&Board.I2C);

void setup() {
  delay(200);
  
  SerialUSB.begin(115200);
  SerialUSB.println("");
  SerialUSB.println("--- START ---------------------------------------------------");
  
  SerialUSB.println("### I/O Initialize.");
  Wio.Init();
  
  SerialUSB.println("### Power supply ON.");
  Wio.PowerSupplyCellular(true);
  Wio.PowerSupplyGrove(true);
  delay(500);
  
  SerialUSB.println("### Turn on or reset.");
  if (!Wio.TurnOnOrReset()) {
    SerialUSB.println("### ERROR! ###");
    abort();
  }
  
  SerialUSB.println("### Connecting to \"" APN "\".");
  if (!Wio.Activate(APN, USERNAME, PASSWORD)) {
    SerialUSB.println("### ERROR! ###");
    abort();
  }
  
  SerialUSB.println("### Connecting to MQTT server \"" MQTT_SERVER_HOST "\"");
  MqttClient.setServer(MQTT_SERVER_HOST, MQTT_SERVER_PORT);
  MqttClient.setClient(WioClient);
  if (!MqttClient.connect(THING)) {
    SerialUSB.println("### ERROR! ###");
    abort();
  }
  
  SerialUSB.println("### Sensor Initialize.");
  Board.I2C.Enable();
  Sensor.Init();

  SerialUSB.println("### Setup completed.");
}

void loop() {
  char data[200];
  Sensor.Read();
  SerialUSB.print("Current temperature = ");
  SerialUSB.print(Sensor.Temperature);
  SerialUSB.print("C  ");
  SerialUSB.print("pressure = ");
  SerialUSB.print(Sensor.Pressure);
  SerialUSB.println("Pa");
  sprintf(data, "{\"temp\":%.1f,\"pres\":%.1f}", Sensor.Temperature, Sensor.Pressure);
  
  if (!isnan(Sensor.Temperature) && !isnan(Sensor.Pressure)) {
    MqttClient.publish(TELEMETRY_TOPIC, data);
  }

  unsigned long next = millis() + INTERVAL;
  while (millis() < next) {
    MqttClient.loop();
  }
}
