#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>

// Define your Firebase credentials and WiFi credentials
#define WIFI_SSID "WiFi Name"
#define WIFI_PASSWORD "WiFi Password"
#define FIREBASE_PROJECT_ID "Firebase Project ID"
#define FIREBASE_API_KEY "Firebse web API key"

FirebaseConfig fconfig;
FirebaseData fdata;
FirebaseAuth fauth;
FirebaseJson json;
bool sendData = true;

void setup() {
  Serial.begin(115200);
  
  connectToWiFi();
  configureFirebase();
}


void loop() {
  String name = "Excptional";
  String phoneNumber = "8621458721";

  if(sendData){
    uploadDocument(name, phoneNumber);
  }
  
}

void connectToWiFi(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi!");
}

void configureFirebase(){
  fconfig.api_key = FIREBASE_API_KEY;
  fauth.user.email = "bikash27dutta@gmail.com";
  fauth.user.password = "8777264639";
  fconfig.token_status_callback = tokenStatusCallback;

  Firebase.begin(&fconfig, &fauth);
  
  if (Firebase.ready()) {
    Serial.println("Connected to Firestore!");
  } else {
    Serial.println("Failed to connect to Firestore");
  }
  
  Firebase.reconnectWiFi(true);
}

void uploadDocument(String name, String phoneNumber) {

  json.set("fields/Name/stringValue", name);
  json.set("fields/Phone/stringValue", phoneNumber);

  // Send the data to Firestore
  if (Firebase.Firestore.patchDocument(&fdata, FIREBASE_PROJECT_ID, "", "users/data", json.raw(), "Name")
  && Firebase.Firestore.patchDocument(&fdata, FIREBASE_PROJECT_ID, "", "users/data", json.raw(), "Phone")) {
    Serial.println("Document uploaded successfully!");  
    sendData = false;
  } else {
    Serial.println("Error uploading document: " + fdata.errorReason());
  }
  
}
