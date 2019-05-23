// connect;kereon;camerappelleunevieillecopine
// init-firebase;cat-feeder-66c26.firebaseio.com;1OMWIJPzgentPSNAEoTYPZTzLwrlfCWI8ht0MqtL
// get-string;/feeders/0/name
// get;/feeders/0
// get-time;Europe/Paris
// begin-stream;/feeders/0
// begin-stream;/feeders/0/name
// begin-stream;/feeders/0/alarms/0/name
// stop-stream;
// get-event-stream;

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <FirebaseArduino.h>

#define FIREBASE_EVENT_OUTPUT_PIN 2

#define DEBUG true

#define CONNECTING_TIMEOUT_LIMIT 20

#define WORLD_TIME_API "http://worldtimeapi.org/api/timezone/"

#define COMMAND_SEPARATOR           ';'
#define COMMAND_CONNECT             "connect"
#define COMMAND_ID_CONNECTED        "id-connected"
#define COMMAND_INIT_FIREBASE       "init-firebase"
#define COMMAND_GET_TIME            "get-time"
#define COMMAND_GET                 "get"
#define COMMAND_GET_STRING          "get-string"
#define COMMAND_BEGIN_STREAM        "begin-stream"
#define COMMAND_STOP_STREAM         "stop-stream"
#define COMMAND_GET_EVENT_STREAM    "get-event-stream"
#define COMMAND_STATUS_OK           "OK"
#define COMMAND_STATUS_KO           "KO"

HTTPClient http;
bool connected;
bool firebaseConnected;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("Launch module");

  pinMode(FIREBASE_EVENT_OUTPUT_PIN, OUTPUT);
  digitalWrite(FIREBASE_EVENT_OUTPUT_PIN, LOW);

  connected = false;
  firebaseConnected = false;
}

void loop() {
  String cmd = Serial.readStringUntil(COMMAND_SEPARATOR);
  if (cmd == "") {
    return;
  }

  if (cmd == COMMAND_CONNECT) connected = connect();
  if (cmd == COMMAND_ID_CONNECTED) isConnected();

  if (!connected) return;

  if (cmd == COMMAND_INIT_FIREBASE) firebaseConnected = initFirebase();
  if (cmd == COMMAND_GET_TIME) getTime();
  
  if (!firebaseConnected) return;

  if (cmd == COMMAND_GET) get();
  if (cmd == COMMAND_GET_STRING) getString();
  if (cmd == COMMAND_BEGIN_STREAM) beginStream();
  if (cmd == COMMAND_STOP_STREAM) stopStream();
  if (cmd == COMMAND_GET_EVENT_STREAM) getEventStream();

  if (Firebase.available()) {
    digitalWrite(FIREBASE_EVENT_OUTPUT_PIN, HIGH);
  }
}

#ifdef DEBUG
void debug(String cmd, int argsCount, String args[]) {
  Serial.print("Launch '" + cmd + "' command with ");
  Serial.print(argsCount);
  Serial.println(" arguments : ");
  for (int i = 0; i < argsCount; i++) {
    Serial.print(i);
    Serial.print(" : ");
    Serial.println(args[i]);
  }
}
#endif

bool connect() {
  String wifiSSID     = Serial.readStringUntil(COMMAND_SEPARATOR);
  String wifiPassword = Serial.readString();
  wifiPassword.trim();

  WiFi.begin(wifiSSID, wifiPassword);
  int timeout = 0;
  while (WiFi.status() != WL_CONNECTED && timeout < CONNECTING_TIMEOUT_LIMIT) {
    timeout++;
    delay(500);
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(COMMAND_STATUS_OK);
    
    return true;
  } else {
    Serial.println(COMMAND_STATUS_KO);
    
    return false;
  }
}

void isConnected() {
  Serial.println(WiFi.status() != WL_CONNECTED ? COMMAND_STATUS_OK : COMMAND_STATUS_KO);
}

bool initFirebase() {
  String host  = Serial.readStringUntil(COMMAND_SEPARATOR);
  String token = Serial.readString();
  token.trim();
  
  Firebase.begin(host, token);

  Serial.println(COMMAND_STATUS_OK);
    
  return true;
}

void getTime() {
  String timeZone = Serial.readString();
  timeZone.trim();

  http.begin(WORLD_TIME_API + timeZone);
  int httpCode = http.GET();

  if (httpCode != 200) {
    Serial.println(COMMAND_STATUS_KO);
  } else {
    Serial.println(http.getString());
    Serial.println(COMMAND_STATUS_OK);
  }

  http.end();
}

void get() {
  String path = Serial.readString();
  path.trim();

  FirebaseObject data = Firebase.get(path);
  if (Firebase.failed()) {
    Serial.println(COMMAND_STATUS_KO);

    return;
  }
  
  JsonVariant jsonDocument = data.getJsonVariant();
  jsonDocument.printTo(Serial);
  Serial.println();
}

void getString() {
  String path = Serial.readString();
  path.trim();

  String data = Firebase.getString(path);
  if (Firebase.failed()) {
    Serial.println(COMMAND_STATUS_KO);
    
    return;
  }
  
  Serial.println(data);
}

bool beginStream() {
  String path = Serial.readString();
  path.trim();

  Firebase.stream(path);
  if (Firebase.failed()) {
    Serial.println(COMMAND_STATUS_KO);

    return false;
  }
  Serial.println(COMMAND_STATUS_OK);
  
  return true;
}

bool stopStream() {
  Serial.println(COMMAND_STATUS_OK);
  
  return true;
}

void getEventStream() {
    digitalWrite(FIREBASE_EVENT_OUTPUT_PIN, LOW);
    
    FirebaseObject data = Firebase.readEvent();
    if (Firebase.failed()) {
      Serial.println(COMMAND_STATUS_KO);
  
      return;
    }
    
    JsonVariant jsonDocument = data.getJsonVariant();
    jsonDocument.printTo(Serial);
    Serial.println();
}
