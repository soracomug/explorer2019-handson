#include <PubSubClient.h>     // https://github.com/knolleary/pubsubclient
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
char THING[50];
char TELEMETRY_TOPIC[200];

#define INTERVAL  (5000)

WioCellular Wio;

WioLTEClient WioClient(&Wio);
PubSubClient MqttClient;

GroveBoard Board;
OmronBaro2SMPB02E Sensor(&Board.I2C);

// Wio LTE から SORACOM Air のメタデータサービスにアクセスするコードスニペット@ma2shita
// https://qiita.com/ma2shita/items/44cfe28d76fe0d8aa45a
String get_metadata_by(WioLTE& wio, const char* tag_key, const char* default_value = "") {
    char url[1024];
    sprintf(url, "http://metadata.soracom.io/v1/subscriber.tags.%s", tag_key);
    char buf[1024];
    wio.HttpGet(url, buf, sizeof(buf));
    String content = String(buf);
    content.trim();
    if (content == "Specified key does not exist." || /* == 404 */
        content == "You are not allowed to access Metadata Server.") { /* == 403 */
        content = String(default_value);
        content.trim();
    }
    return content;
}

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

  SerialUSB.println("### Get thing name from meta data service.");
  strcpy(THING, get_metadata_by(Wio, "Thing").c_str());
  sprintf(TELEMETRY_TOPIC, "dt/%s/%s/%s", APPLICATION, CONTEXT, THING);
  SerialUSB.print("Thing = ");
  SerialUSB.println(THING);
  
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
