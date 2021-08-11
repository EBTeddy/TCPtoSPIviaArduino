#include <ESP8266WiFi.h>
#include <SPI.h>
#include "my_nodeMCU.h"

#ifndef STASSID
#define STASSID "DESKTOP-chen"
#define STAPSK  "12345678"
#endif
#define clientTimeout 15

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
  // if (!client&&!client_extra) {return;}
  // SPI主指令
  if (client){
    Serial.println(F("main client！"));
    client.setTimeout(clientTimeout); 

    String temp_req = client.readStringUntil('\r');
    // String cmd_req=temp_req.substring(0,CMD_length);
    const char * temp_req_char=temp_req.c_str();
    // const char * cmd_char=cmd_req.c_str();
    switch (temp_req_char[0])
    {
    case 0x31:
      Serial.println(F("IO_update"));
      ad9959_io_update();
      break;
    case 0x32:
      ad9959_cs(true);
      Serial.println(F("CS_HIGH"));
      break;
    case 0x33:
      ad9959_cs(false);
      Serial.println(F("CS_LOW"));
      break;
    case 0x30:
      Serial.println(F("RESET"));
      ad9959_rst();
      break;
    case 0xFF:
      Serial.println(F("NULL CMD"));
      break;
    default:
      break;
    }
    Serial.println(F("length:"));
    Serial.println(temp_req.length());;
    if(temp_req.length()>1){
      int spi_req_length=temp_req.length()-CMD_length;
      // TODO:结尾有个0x00，所以要+1；
      // String spi_req=temp_req.substring(CMD_length,spi_req_length);
      // const char * req = spi_req.c_str();
      const char * req = temp_req_char+1;
      // 发送SPI指令
      esp_spi.writeData((uint8_t *)req,spi_req_length);
      Serial.println(F("request: "));
      Serial.write(req);
      Serial.println();
    }      
    // // 读取缓冲区TCP指令
    // while(client.available()){
    //   String temp_req = client.readStringUntil('\r');
    //   const char * req=temp_req.c_str();
    //   int req_length=temp_req.length();
    //   // 发送SPI指令
    //   esp_spi.writeData((uint8_t *)req,req_length);
    //   Serial.println(F("main request: "));
    //   Serial.write(req);
    //   Serial.println();
    // }
  }
//  WiFiClient client_extra = server_extra.available();
// // 额外控制指令，用于控制
//   if(client_extra){
//     Serial.println(F("extra client！"));
//     client_extra.setTimeout(clientTimeout);

//     String temp_req = client_extra.readStringUntil('\r');
//     const uint8_t * req=(uint8_t *)temp_req.c_str();
//     int req_length=temp_req.length();
//     Serial.println(F("extra request: "));
//     Serial.println(temp_req);
//     switch ((int)req[0])
//     {
//     case 0x31:
//       Serial.println(F("IO_update"));
//       ad9959_io_update();
//       break;
//     case 0x32:
//       ad9959_cs(false);
//       Serial.println(F("CS_LOW"));
//       break;
//     case 0x33:
//       ad9959_cs(true);
//       Serial.println(F("CS_HIGH"));
//       break;
//     case 0x34:
//       Serial.println(F("RESET"));
//       ad9959_rst();
//       break;
//     default:
//       break;
//     }
  


//     while(client_extra.available()){
//       String temp_req = client_extra.readStringUntil('\r');
//       const uint8_t * req=(uint8_t *)temp_req.c_str();
//       int req_length=temp_req.length();
//       Serial.println(F("extra request: "));
//       Serial.println(temp_req);
//       switch ((int)req[0])
//       {
//       case 0x31:
//         Serial.println(F("IO_update"));
//         ad9959_io_update();
//         break;
//       case 0x32:
//         ad9959_cs(false);
//         Serial.println(F("CS_LOW"));
//         break;
//       case 0x33:
//         ad9959_cs(true);
//         Serial.println(F("CS_HIGH"));
//         break;
//       case 0x34:
//         Serial.println(F("RESET"));
//         ad9959_rst();
//         break;
//       default:
//         break;
//       }
  //   }
  // }
}
