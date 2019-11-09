#include <GroveDriverPack.h>  // https://github.com/SeeedJP/GroveDriverPack

#define APN       "soracom.io"
#define USERNAME  "sora"
#define PASSWORD  "sora"

#define INTERVAL        (60000)
#define RECEIVE_TIMEOUT (10000)

WioCellular Wio;

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
  if (isnan(Sensor.Temperature) || isnan(Sensor.Pressure)) goto err;

  SerialUSB.println("### Open.");
  int connectId;
  connectId = Wio.SocketOpen("harvest.soracom.io", 8514, WIO_TCP);
  if (connectId < 0) {
    SerialUSB.println("### ERROR! ###");
    goto err;
  }

  SerialUSB.println("### Send.");
  SerialUSB.print("Send:");
  SerialUSB.print(data);
  SerialUSB.println("");
  if (!Wio.SocketSend(connectId, data)) {
    SerialUSB.println("### ERROR! ###");
    goto err_close;
  }

  SerialUSB.println("### Receive.");
  int length;
  length = Wio.SocketReceive(connectId, data, sizeof (data), RECEIVE_TIMEOUT);
  if (length < 0) {
    SerialUSB.println("### ERROR! ###");
    goto err_close;
  }
  if (length == 0) {
    SerialUSB.println("### RECEIVE TIMEOUT! ###");
    goto err_close;
  }
  SerialUSB.print("Receive:");
  SerialUSB.print(data);
  SerialUSB.println("");

err_close:
  SerialUSB.println("### Close.");
  if (!Wio.SocketClose(connectId)) {
    SerialUSB.println("### ERROR! ###");
    goto err;
  }

err:
  delay(INTERVAL);
}

////////////////////////////////////////////////////////////////////////////////////////
