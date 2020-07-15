int Status = 12;  // Digital pin D6

int sensor = 13; // Digital pin D7
int count=0; // no of occupied seats
int vacancies = 50 //assuming bus has 50 seats 

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "YOUJNTU1S12";
const char* password = "YOUJNTU1S12";
#define ORG "znoixf"
#define DEVICE_TYPE "NodeMCU"
#define DEVICE_ID "11111"
#define TOKEN "123456789"
//-------- Customise the above values --------
 //include device credentials
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char publictopic[] = "iot-2/evt/ammonia/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

WiFiClient wifiClient;
PubSubClient client(server, 1883,wifiClient);

void setup() {
  // put your setup code here, to run once:
      Serial.begin(9600);
  pinMode(sensor, INPUT);   // declare sensor as input
  pinMode(Status, OUTPUT);  // declare LED as output
       
       Serial.print(ssid);
       WiFi.begin(ssid, password);
       while (WiFi.status() != WL_CONNECTED) {
          delay(500);
          Serial.print(".");  } 
      Serial.println("");
 
      Serial.print("WiFi connected, IP address: ");
      Serial.println(WiFi.localIP());
}

void loop() {
      long state = digitalRead(sensor);
    if(state == HIGH) {
      count=count+1;
      digitalWrite (Status, HIGH);
      Serial.println("Motion detected!");
      delay(1000);
    }
    else {
      digitalWrite (Status, LOW);
      Serial.println("Motion absent!");
      delay(1000);
      }

      Serial.print("No of seats occupied is = ");
      Serial.println(count);//print the ppm percent value to serial
      delay(1000);
             }
      vacancies=vacancies-count;
      
 
      float h=digitalRead(sensor);
      if(isnan(h))
        {  Serial.println("failed to read from pir sensor");    }
      Serial.println("vacancies are :  "+ String(vacancies)); 
      delay(1000);   

      PublishData(count,vacancies);
}

void PublishData(int count ,int vacancies){
        if (!!!client.connected()) {
          Serial.print("Reconnecting client to ");
          Serial.println(server);
          while (!!!client.connect(clientId, authMethod, token)) {
            Serial.print(".");
            delay(500);
          }
        Serial.println();
      }
  
      String payload = "{\"d\":{\"count\":";
      payload += count;
      payload = "," "\"vacancies\":";
      payload += vacancies;
      payload += "}}";
      Serial.print("Sending payload: ");
      Serial.println(payload);
  
      if (client.publish(publictopic, (char*) payload.c_str())) {
        Serial.println("Publish ok");  }
      else {
      Serial.println("Publish failed");   }
}      
