#include "main.h" 

// Time
uint32_t mLastTime = 0;
uint32_t mTimeSeconds = 0;

void setup() {
  /////////////////////////////////////////////////////             
               /* SERIAL inizialization */
  /////////////////////////////////////////////////////
  //Serial 1
  Serial.begin(115200);
  while (!Serial) {}
  Serial.println("__ OK __");
  
  //Serial 2
  Serial2.begin(115200, SERIAL_8N1, Serial2RX2, Serial2TX2);
  while (!Serial2) {}
  Serial.println("__ OK2 __");
  
  /////////////////////////////////////////////////////             
               /* WiFi inizialization */
  /////////////////////////////////////////////////////
  Serial.println("NodeMCU is booting");
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, password);
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  /////////////////////////////////////////////////////             
               /* OTA inizialization */
  /////////////////////////////////////////////////////
  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";
      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
  ArduinoOTA.begin();

  /////////////////////////////////////////////////////             
          /* REMOTE DEBUGGER inizialization */
  /////////////////////////////////////////////////////
  String hostNameWifi = HOST_NAME;
  hostNameWifi.concat(".local");
  #ifdef USE_MDNS  // Use the MDNS ?
    if (MDNS.begin(HOST_NAME)) {
        Serial.print("* MDNS responder started. Hostname -> ");
        Serial.println(HOST_NAME);
    }
    MDNS.addService("telnet", "tcp", 23);
  #endif

  Debug.begin(HOST_NAME); // Initialize the WiFi server
  Debug.setResetCmdEnabled(true); // Enable the reset command
	Debug.showProfiler(true); // Profiler (Good to measure times, to optimize codes)
	Debug.showColors(true); // Colors
  // End off setup
  Serial.println("* Arduino RemoteDebug Library");
  Serial.println("*");
  Serial.print("* WiFI connected. IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("*");
  Serial.println("* Please use the telnet client (telnet for Mac/Unix or putty and others for Windows)");
	Serial.println("* or the RemoteDebugApp (in browser: http://joaolopesf.net/remotedebugapp)");
  Serial.println("*");
  Serial.println("* This sample will send messages of debug in all levels.");
  Serial.println("*");
	Serial.println("* Please try change debug level in client (telnet or web app), to see how it works");
  Serial.println("*");

  /////////////////////////////////////////////////////             
              /* IO inizialization */
  /////////////////////////////////////////////////////
  pinMode(Serial2DERE, OUTPUT);
  pinMode(OutStepperEN, OUTPUT);
  pinMode(OutStepperDIR, OUTPUT);
  pinMode(OutStepperSTEP, OUTPUT);
  pinMode(InStepperAlarm, INPUT_PULLUP);
  pinMode(InStepperInPos, INPUT_PULLUP);
  pinMode(PulsStartMotore, INPUT_PULLUP);
  pinMode(PulsStopMotore, INPUT_PULLUP);
  pinMode(PulsStartTaglio, INPUT_PULLUP);
  pinMode(PulsStopTaglio, INPUT_PULLUP);
  pinMode(FcsDestro, INPUT_PULLUP);
  pinMode(FcsSinistro, INPUT_PULLUP);
  pinMode(InVFDErr, INPUT_PULLUP);

  digitalWrite(Serial2DERE, LOW);

  /////////////////////////////////////////////////////             
              /* MODBUS inizialization */
  /////////////////////////////////////////////////////
  node.begin(DRIVE_MODBUS_ID, Serial2);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  /////////////////////////////////////////////////////             
              /* GENERAL inizialization */
  /////////////////////////////////////////////////////
  ESP32Encoder::useInternalWeakPullResistors=UP;
	encoder.attachHalfQuad(InEncoderDT, InEncoderCLK);
	encoder.setCount(1);

  menu.begin(lcd, GVL.MenuEntitys, encoder, EncBtn, vfd);
}

void loop() {
  while(1){
    ArduinoOTA.handle();
    GVL.init();
    menu.update();
    
    /*
    uint8_t Return = node.readHoldingRegisters(0x1000,9);
      if (Return==node.ku8MBSuccess){
        for (int i=0; i<10; i++){
          uint16_t temp = node.getResponseBuffer(i);
          uint16_t k = 1000 + i;
          Serial.printf("\nValore registro %u: ",k);
          Serial.print(temp);
        }
        node.clearResponseBuffer();
      }
      else{
        Serial.printf("\nErrore lettura registro: %u",Return);
        for (int i=0; i<10; i++){
          uint16_t temp = node.getResponseBuffer(i);
          uint16_t k = 1000 + i;
          Serial.printf("\nValore registro %u: ",k);
          Serial.print(temp);
        }
        node.clearResponseBuffer();
      }
    Return = node.writeSingleRegister(0x1000,tempVal);
    */

    // RemoteDebug handle
    Debug.handle();
    // Give a time for ESP
    yield();
  }
}