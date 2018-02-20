#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include<ESP8266WebServer.h>

//Enter your WiFi Credentials
const char* ssid = "NITK-NET";
const char* password = "2K16NITK";

unsigned long update_db_interval = 1000;
//database update interval - 1 second

uint8_t bin_ID = 2;//ID of bin
uint8_t bin_status = 0;
uint8_t bin_location = 1;
uint8_t bin_level = 22;

//session variables

unsigned long present_ms = 0, last_ms = 0, update_db, time_ms = 0;

String server = "http://10.50.38.33/"; //Ip address of server
int port = 80; //default http port
String response;
String ard_data;

HTTPClient http;//http client object to communicate with website

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  present_ms = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  req_server();
}

void loop() {

  present_ms = millis();

  // Requesting Website every 1second
  if (present_ms - update_db > update_db_interval)
  {
    req_server();
    update_db = present_ms;
  }
}

void req_server()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    String url = "bin_IOT/update_bin.php/?bin_id=";
    url += String(bin_ID);
    url += "&bin_status=";
    url += String(bin_status);
    url += "&bin_level=";
    url += String(bin_level);
    url += "&bin_location=";
    url += String(bin_location);

    String request = server + url;
    //Serial.priintln(request);
    http.begin(request);
    Serial.println(request);
    int response_code = http.GET();
    if (response_code == HTTP_CODE_OK)
    {
      response = http.getString();
      //parse_response();
    }

    else {
      //error
      //Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(response_code).c_str());
    }
    http.end();
    delay(10);
  }
}



void parse_response()
{
  // Parsing the response
  // Not needed for bin code because there is no response from server
  int l = response.length(), k = 0;
  int limits[100] = {0};
  for (int i = 0; i < l ; i++)
  {
    if (response[i] == '#')
    {
      limits[k] = i + 1;
      k++;
    }
  }
  // Serial.println(k);
}


