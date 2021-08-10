#include <ESP8266WiFi.h>
#include <SPI.h>
#include "my_nodeMCU.h"

#ifndef STASSID
#define STASSID "DESKTOP-chen"
#define STAPSK  "12345678"
#endif
#define clientTimeout 20

const char* ssid = STASSID;
const char* password = STAPSK;

// 创建服务器实例，并监控80端口
WiFiServer server(80);
WiFiServer server_extra(81);
//初始化SPI对象
ESPMaster esp_spi(SS);
//初始化部分
void setup() {
  Serial.begin(115200);
  // 初始化SPI
  SPI.begin();
  esp_spi.begin();
  delay(1000);
  // 初始化LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 0);

  // 打印状态
  Serial.println();
  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(ssid);
  // 初始化WIFI连接
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.println(F("WiFi connected"));

  // 初始化服务器
  server.begin();
  server_extra.begin();
  Serial.println(F("Server started"));
  Serial.println(WiFi.localIP());

  ad9959_init();
}
void loop() {
  // 检测WIFI客户端的在线状态
  WiFiClient client = server.available();
    WiFiClient client_extra = server_extra.available();
  
  if (!client&&!client_extra) {return;}
  // SPI主指令
  if (client){
    Serial.println(F("main client！"));
    client.setTimeout(clientTimeout); 

    // 读取缓冲区TCP指令
    String temp_req = client.readStringUntil('\r');
    const char * req=temp_req.c_str();
    int req_length=temp_req.length();
    // 发送SPI指令
    esp_spi.writeData((uint8_t *)req,req_length);
    Serial.println(F("request: "));
    Serial.write(req);
  }
  
  

// 额外控制指令，用于控制
  if(client_extra){
    Serial.println(F("extra client！"));
    client_extra.setTimeout(clientTimeout);
    String temp_req = client_extra.readStringUntil('\r');
    const uint8_t * req=(uint8_t *)temp_req.c_str();
    int req_length=temp_req.length();

      esp_spi.writeData((uint8_t *)req,req_length);
    switch ((int)req[0])
    {
    case 0x01:
      Serial.println(F("IO_update"));
      ad9959_io_update();

      break;
    case 0x02:
      Serial.println(F("RESET"));
      ad9959_rst();

      break;
    default:
      break;
    }
  }
}
