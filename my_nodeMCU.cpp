#include "my_nodeMCU.h"
#include <SPI.h>
//基于esp8266的SPI主机对象,使用GPIO12-15
void ad9959_io_update(){
  digitalWrite(ad9959_pin_update,LOW);
  delay(12);
  digitalWrite(ad9959_pin_update,HIGH);
  delay(24);
  digitalWrite(ad9959_pin_update,LOW);
  return;
};

void ad9959_init(){
  pinMode(ad9959_pin_update,OUTPUT);
  pinMode(ad9959_pin_cs,OUTPUT);
  pinMode(ad9959_pin_rst,OUTPUT);
  ad9959_rst();
  digitalWrite(ad9959_pin_cs,HIGH);
  digitalWrite(ad9959_pin_update,LOW);
  return;
};

void ad9959_rst(){
  digitalWrite(ad9959_pin_rst,LOW);
  delay(12);
  digitalWrite(ad9959_pin_rst,HIGH);
  delay(24);
  digitalWrite(ad9959_pin_rst,LOW);
  return;
};
void ad9959_cs(bool state){
    if(state)
        digitalWrite(ad9959_pin_cs,HIGH);
    else
        digitalWrite(ad9959_pin_cs,LOW);
};