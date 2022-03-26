#include <AllWize.h>
#include <CayenneLPP.h>
#include <TinyGPS++.h>


#define MODULE_SERIAL           SerialWize
#define DEBUG_SERIAL            SerialUSB
#define RESET_PIN               PIN_WIZE_RESET

// -----------------------------------------------------------------------------
// Configuration
// -----------------------------------------------------------------------------

#define WIZE_CHANNEL            CHANNEL_04
#define WIZE_POWER              POWER_20dBm
#define WIZE_DATARATE           DATARATE_2400bps
#define WIZE_MID                0x06FA
#define WIZE_UID                0x20212223
#define WIZE_APP_ID            0xFE
#define WIZE_NETWORK_ID       0x01

#define BATTERY_MONITOR_PIN     A3
#define BATTERY_MONITOR_ENABLE  4
#define BATTERY_RATIO           (3300.0 * 2.0 / 1024.0)

// -----------------------------------------------------------------------------
// Globals
// -----------------------------------------------------------------------------

AllWize allwize(&MODULE_SERIAL, RESET_PIN);


CayenneLPP payload(32);

TinyGPSPlus gps;

void setup()
{

  Serial1.begin(9600);
}

void wizeSetup() {

    DEBUG_SERIAL.println("[WIZE] Initializing radio module");

    // Init AllWize object
    allwize.begin();
    if (!allwize.waitForReady()) {
        DEBUG_SERIAL.println("[WIZE] Error connecting to the module, check your wiring!");
        while (true);
    }

    allwize.slave();
    allwize.setChannel(WIZE_CHANNEL, true);
    allwize.setPower(WIZE_POWER);
    allwize.setDataRate(WIZE_DATARATE);
    allwize.setMID(WIZE_MID);
    allwize.setUID(WIZE_UID);
    allwize.setWizeApplication(WIZE_APP_ID);
    allwize.setWizeNetworkId(WIZE_NETWORK_ID);

    DEBUG_SERIAL.println("[WIZE] Ready...");

}

void wizeSend(uint8_t * payload, size_t len) {

    char buffer[64];
    DEBUG_SERIAL.print("[WIZE] Sending: ");
    for (uint8_t i = 0; i<len; i++) {
        snprintf(buffer, sizeof(buffer), "%02X", payload[i]);
        DEBUG_SERIAL.print(buffer);
    }
    DEBUG_SERIAL.print("\n");

    if (!allwize.send(payload, len)) {
        DEBUG_SERIAL.println("[WIZE] Error sending message");
    }

}


int getVolume()
{
  int muestra = analogRead(A1);
  Serial.println(muestra);

  
  return muestra;
}

float getLatitude
{
    
    gps.encode(Serial1.read());
    float latitude = gps.location.lat();
    gps.location.lng();


 return latitude;
}

float getLongitude
{
    
    gps.encode(Serial1.read());
    float longitude = gps.location.lng();


 return longitude;
}


void loop() {
  
  payload.reset();
  payload.addAnalogInput(1,getVolume());
  payload.addGPS(2,getLatitude(),getLongitude(),0);
  wizeSend(payload.getBuffer(),payload.getSize()); 
  
  delay(200);

}
