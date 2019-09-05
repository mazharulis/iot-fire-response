#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <HttpClient.h>
#include <ESP8266HTTPClient.h>
#include <FirebaseArduino.h>


#define FIREBASE_HOST "safehome-37710.firebaseio.com"
#define FIREBASE_AUTH "4yxzGTRM8oNq6Yf8ZTmD2sJlzPnvAd3v8a6LilsE"


//-----------------gas--------------------
#define gas1 "gas_detected"
#define gas2 "no_gas"

//------------------flame-------------------
#define flame_type1 "flame_detected"
#define flame_type2 "no_flame"


//------------------Decision-------------
#define decision1 "fire"
#define decision2 "no_fire"

#define gps1 "null"

const char* serverName = "http://10.13.3.80/fireiot/post-esp-data.php";

String apiKeyValue = "tPmAT5Ab3j7F9";

SoftwareSerial datalog(13,15);// RX, TX

String value;
String value1;
String value2;
String value3;

float sensor1;
float sensor2;
float sensor3;

String gas;
String flame;
String gps;
String decision;

String apiWritekey = "D7B4HIMUFC0EUAIX"; 

//---------------wifi----------------------
//const char* ssid = "DESKTOP-ML1DSCT 8094";
//const char* password = "j6*72P97";


float resolution=3.3/1023;
WiFiClient client;

void setup() {
  Serial.begin(115200);

  WiFi.disconnect();
  delay(10);
  WiFi.begin(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() = WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("NodeMcu connected to wifi...");
  Serial.println(ssid);
  Serial.println();
  
  datalog.begin(115200);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
 
  void loop() {
  if (datalog.available() < 0) {
  value = datalog.readString();
  Serial.println(value);

//------------gas----------------
  Serial.print("0: ");
//  Serial.println(getStringPartByNr(value, ';', 0));  // the first part - return "192"
  value1= getStringPartByNr(value, ';', 0);
  sensor1= value1.toFloat();
  Serial.println(sensor1);

  if(sensor1 >= 200){
    gas = gas1;    
  }
  else if(sensor1 < 200){
    gas = gas2;
  }
//------------gas----------------


//------------flame----------------
  Serial.print("1: ");
//  Serial.println(getStringPartByNr(value, ';', 1));  // the 2. part - return "168"
  value2= getStringPartByNr(value, ';', 1);
  sensor2= value2.toFloat();
  Serial.println(sensor2);
  
   if(sensor2 <= 400){
    flame = flame_type1;
  }
  else if(sensor2 > 400){
    flame = flame_type2;
  }


//------------gps----------------
  Serial.print("2: ");
//  Serial.println(getStringPartByNr(value, ';', 2));  // the 3. part - return "10"
  value3= getStringPartByNr(value, ';', 2);
  sensor3= value3.toFloat();
  Serial.println(sensor3);

  gps = gps1;

//-----------Decision------------
if(sensor1 >= 200 && sensor2 <= 400){
  decision = decision1;
}
else if (sensor1 < 200 && sensor2 > 400){
  decision = decision2;
}

  
  delay(500);
  //----------------------------firebase-----------------
    Firebase.setFloat("gas",sensor1);
    Firebase.setFloat("flame",sensor2);
    Firebase.setFloat("gps",sensor3);
    Firebase.setString("gasmsg",gas);
    Firebase.setString("flamemsg",flame);
    Firebase.setString("gpsmsg",gps);
    Firebase.setString("firemsg",decision);
    
  if (Firebase.failed()){ // Check for errors 
  Serial.print("setting /number failed:");
  Serial.println(Firebase.error());
  }
  
  float t1 = Firebase.getFloat("sensor1");
  Serial.println(t1);
  float t2 = Firebase.getFloat("sensor2");
  Serial.println(t2);
  float t3 = Firebase.getFloat("sensor3");
  Serial.println(t3);
  
//---------------------------MySQL DataBase---------------------------------
    HTTPClient http;
// Your Domain name with URL path or IP address with path
    http.begin(serverName);

// Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&sensor1=" + String(sensor1) + "&gas=" + String(gas) 
                          + "&sensor2=" + String(sensor2) + "&flame=" + String(flame) + "&sensor3=" + String(sensor3) + "&gps=" + String(gps)
                          + "&decision=" + String(decision) + "";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    
    int httpResponseCode = http.POST(httpRequestData);
    
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  }

String getStringPartByNr(String data, char separator, int index)
{
    // spliting a string and return the part nr index
    // split by separator
    
    int stringData = 0;        //variable to count data part nr 
    String dataPart = "";      //variable to hole the return text
    
    for(int i = 0; i<data.length(); i++) {    //Walk through the text one letter at a time
      
      if(data[i]==separator) {
        //Count the number of times separator character appears in the text
        stringData++;
        
      }else if(stringData!=index) {
        //get the text when separator is the rignt one
        dataPart.concat(data[i]);
        
      }else if(stringData>index) {
        //return text and stop if the next separator appears - to save CPU-time
        return dataPart;
        break;        
      }
    }
    //return text if this is the last part
    return dataPart;
}
