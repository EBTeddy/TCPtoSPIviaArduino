#include <SPI.h>

//定义宏
#define  SS 15
#define ad9959_pin_update 5
#define ad9959_pin_cs 4
#define ad9959_pin_rst 16
//声明对象
//class ESPMaster;
//声明函数
void ad9959_rst();
void ad9959_init();
void ad9959_io_update();
void ad9959_cs(bool state);
//类定义
class ESPMaster {
  private:
    uint8_t _ss_pin;

  public:
    ESPMaster(uint8_t pin): _ss_pin(pin) {}
    void begin() {
      pinMode(_ss_pin, OUTPUT);
      digitalWrite(_ss_pin, HIGH);
    }

    uint32_t readStatus() {
      digitalWrite(_ss_pin, LOW);
      SPI.transfer(0x04);
      uint32_t status = (SPI.transfer(0) | ((uint32_t)(SPI.transfer(0)) << 8) | ((uint32_t)(SPI.transfer(0)) << 16) | ((uint32_t)(SPI.transfer(0)) << 24));
      digitalWrite(_ss_pin, HIGH);
      return status;
    }

    void writeStatus(uint32_t status) {
      digitalWrite(_ss_pin, LOW);
      SPI.transfer(0x01);
      SPI.transfer(status & 0xFF);
      SPI.transfer((status >> 8) & 0xFF);
      SPI.transfer((status >> 16) & 0xFF);
      SPI.transfer((status >> 24) & 0xFF);
      digitalWrite(_ss_pin, HIGH);
    }

    void readData(uint8_t * data) {
      digitalWrite(_ss_pin, LOW);
      SPI.transfer(0x03);
      SPI.transfer(0x00);
      for (uint8_t i = 0; i < 32; i++) {
        data[i] = SPI.transfer(0);
      }
      digitalWrite(_ss_pin, HIGH);
    }

    void writeData(uint8_t * data, size_t len) {
      uint8_t i = 0;
      digitalWrite(_ss_pin, LOW);
      // while (len-- && i < 32) {
      //   SPI.transfer(data[i++]);
      // }
      // while (i++ < 32) {
      //   SPI.transfer(0);
      // }
      while (len--) {
        SPI.transfer(data[i++]);
      }
      digitalWrite(_ss_pin, HIGH);
    }

    String readData() {
      char data[33];
      data[32] = 0;
      readData((uint8_t *)data);
      return String(data);
    }

    void writeData(const char * data) {
      writeData((uint8_t *)data, strlen(data));
    }
};

