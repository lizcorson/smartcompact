/* SmartCompact: a tiny smart mirror by Liz Corson
 * Uses the Weather Underground API. 
 */
 
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
 
const char* ssid     = "...";
const char* password = "...";
 
const char* host = "api.wunderground.com";

//API key from Weather Underground
String wuApiKey = "api_key";

//Place you want weather for
String state = "..";
String city = "...";

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
 
int value = 0;
 
void loop() {
    delay(5000);
  ++value;

  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  getCurrentWeather(client);
  delay(10000);
  getFutureWeather(client);
  delay(10000);
}


void getCurrentWeather(WiFiClient client) {
  // We now create a URI for the request
  String url = "/api/" + wuApiKey + "/conditions/q/" + state + "/" + city + ".json";

  Serial.print("Requesting URL: ");
  Serial.println(url);
  
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
  }
  //The last line contains the JSON object to be parsed
  int str_len = line.length() + 1;
  DynamicJsonBuffer jsonBuffer;
  char json[str_len];
  line.toCharArray(json,str_len);

  //Parse the JSON object
  JsonObject& root = jsonBuffer.parseObject(json);
  // Test if parsing succeeds.
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  
  Serial.println();
  Serial.println("closing connection");

  JsonObject& currentWeather = root["current_observation"];
  const char* temp = currentWeather["temp_f"];
  const char* rh = currentWeather["relative_humidity"];
  const char* description = currentWeather["weather"];
  const char* feelslike = currentWeather["feelslike_f"];
  const char* windmph = currentWeather["wind_mph"];

  Serial.println(description);
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.println(" F");
  Serial.print("Humidity: ");
  Serial.println(rh);
  Serial.print("Feels like ");
  Serial.print(feelslike);
  Serial.println(" F");
  Serial.print("Wind: ");
  Serial.print(windmph);
  Serial.println(" mph");
}

void getFutureWeather(WiFiClient client) {
  // We now create a URI for the request
  String url = "/api/" + wuApiKey + "/forecast/q/" + state + "/" + city + ".json";

  Serial.print("Requesting URL: ");
  Serial.println(url);
  
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
  }
  //Serial.print(line);
  //gross and so hacky but whatevs!!
  
  //get high temperature
  int highTstart = line.indexOf("\"fahrenheit\"");
  String highTfirstcut = line.substring(highTstart+14,highTstart+19);
  int highTend = highTfirstcut.indexOf("\"");
  String highT = highTfirstcut.substring(0,highTend);
  Serial.print("High: ");
  Serial.print(highT);
  Serial.println(" F");
  String restofline = line.substring(highTstart+20);

  //get low temperature
  int lowTstart = restofline.indexOf("\"fahrenheit\"");
  String lowTfirstcut = restofline.substring(lowTstart+14,lowTstart+19);
  int lowTend = lowTfirstcut.indexOf("\"");
  String lowT = lowTfirstcut.substring(0,lowTend);
  Serial.print("Low: ");
  Serial.print(lowT);
  Serial.println(" F");

  //get chance of precip
  int popstart = restofline.indexOf("\"pop\"");
  String popfirstcut = restofline.substring(popstart+6,popstart+9);
  int popend = popfirstcut.indexOf(",");
  String pop = popfirstcut.substring(0,popend);
  Serial.print(pop);
  Serial.println("% chance of precipitation");

  //This JSON stuff causes memory overflows
//  //The last line contains the JSON object to be parsed
//  int str_len = line.length() + 1;
//  DynamicJsonBuffer jsonBuffer;
//  char json[str_len];
//  line.toCharArray(json,str_len);
//
//  //Parse the JSON object
//  JsonObject& root = jsonBuffer.parseObject(json);
//  // Test if parsing succeeds.
//  if (!root.success()) {
//    Serial.println("parseObject() failed");
//    return;
//  }
//  else {
//    Serial.println("successful parse");
//  }
//  
//  Serial.println();
//  Serial.println("closing connection");
//
//  JsonObject& allForecast = root["forecast"];
//  Serial.println("ok");
//  JsonObject& simpleForecast = allForecast["simpleforecast"];
//  Serial.println("ok");
//  JsonObject& fd = simpleForecast["forecastday"];
//  Serial.println("ok");
//  JsonObject& high = fd["forecastday"];
//  const char* highT = high["fahrenheit"];
//  Serial.print("High ");
//  Serial.print(highT);
//  Serial.println(" F");  
}

