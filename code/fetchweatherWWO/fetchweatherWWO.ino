/* SmartCompact: a tiny smart mirror by Liz Corson
 * Uses the Weather Underground API. 
 */
 
#include <ESP8266WiFi.h>
 
const char* ssid     = "...";
const char* password = "...";

const char* host = "api.worldweatheronline.com";

//API key from Weather Underground
String wwoApiKey = "...";

//Place you want weather for
String zip = "...";

//Store weather data
String tempF = "";
String description = "";
String RH = "";
String highT = "";
String lowT = "";

long lastUpdateTime = 0;
long lastPrintTime = 0;

long updateWaitTime = 1000 * 60 * 10; //10 minute wait
int printWaitTime = 10000;
bool firstUpdate = false;

void setup() {
  Serial.begin(115200);
  delay(100);
 
  // We start by connecting to a WiFi network
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
 

 
void loop() {

  if ((unsigned long)(millis() - lastUpdateTime) > updateWaitTime || firstUpdate == false) {
    getCurrentWeather();
  }

  if ((unsigned long)(millis() - lastPrintTime) > printWaitTime && firstUpdate) {
    Serial.println("");
    Serial.println(description);
    Serial.println("Current Temp: " + tempF + " F");
    Serial.println("Humidity: " + RH + "%");
    Serial.println("High: " + highT + " F");
    Serial.println("Low: " + lowT + " F");
    Serial.print("Last fetched from WWO ");
    Serial.print((millis() - lastUpdateTime)/1000.0/60);
    Serial.println(" min ago.");
    lastPrintTime = millis();
  }
}


void getCurrentWeather() {
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/free/v2/weather.ashx?q=" + zip + "&format=csv&num_of_days=2&key=" + wwoApiKey;

  //Serial.print("Requesting URL: ");
  //Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(500);
  
  // Read all the lines of the reply from server and print them to Serial
  String line = "";
  while(client.available()){
    line = client.readStringUntil('\r');    
    //Serial.print(line);
    //delay(500);
  }
  //Serial.println(line);
  //Serial.println(line.length());
  //The last line contains data to be parsed
  //Parse CSV
  tempF = getValue(line, ',', 2);
  description = getValue(line, ',', 5);
  RH = getValue(line, ',', 11);
  highT = getValue(line, ',', 16);
  lowT = getValue(line, ',', 18);

  if (line.length() > 1000) {
    lastUpdateTime = millis();
    firstUpdate = true;
  }

}

//found at http://stackoverflow.com/a/18426063
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {
    0, -1  };
  int maxIndex = data.length()-1;
  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
      found++;
      strIndex[0] = strIndex[1]+1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

