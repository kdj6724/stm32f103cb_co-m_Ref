// hc_06.c
// kdj6724@naver.com
// 2018-10-26
#include "stm32f1xx_hal.h"
#include "hc_06.h"

UART_HandleTypeDef* hc06UART_ = NULL;
uint8_t rxbuf_[2];

int hc06_send_atcmd(char* data, size_t len) {
	HAL_StatusTypeDef status;
  if (hc06UART_ == NULL) {
    printf("[hc-06] uart is NULL(%d)\n", __LINE__);
    return -1;
  }
  printf("[hc-06] send : %s\n", data);
	status = HAL_UART_Transmit(hc06UART_, (uint8_t*)data, len, 1000);
	return (status == HAL_OK ? len : 0);
}

int hc06_init(UART_HandleTypeDef* uart) {
  hc06UART_ = uart;
  hc06_set_baudrate(HC06_BAUD_9600);
  hc06_set_bluetoothname("kdj", 3);
  hc06_set_pin("1111", 4);
  HAL_UART_Receive_IT(uart, rxbuf_, 1);
}

int hc06_receive_byte(uint8_t rx) {
  char* ptr = NULL;
  int res = HC06_CMD0;

  if (hc06UART_ == NULL) {
    printf("[hc-06] uart is NULL(%d)\n", __LINE__);
    return -1;
  }
  if (rxbuf_[0] == HC06_UART_ENDBYTE) {
    res = HC06_CMD1;
  } else {
    printf("[hc-06] receive : 0x%x\n", rxbuf_[0]);
  }
  memset(rxbuf_, 0, sizeof(rxbuf_));
  HAL_UART_Receive_IT(hc06UART_, rxbuf_, 1);
  return res;
}

int hc06_set_baudrate(int val) {
  uint8_t cmd[32];
  memset(cmd, 0, size_t(cmd));
  sprintf(cmd, "%sBAUD%d",HC06_ATCMD_PREFIX, val);
  return hc06_send_atcmd(cmd, strlen(cmd));
}

int hc06_set_bluetoothname(char* name, size_t len) {
  uint8_t cmd[32];
  memset(cmd, 0, size_t(cmd));
  sprintf(cmd, "%sNAME%s",HC06_ATCMD_PREFIX, name);
  return hc06_send_atcmd(cmd, strlen(cmd));
}

int hc06_set_pin(char* pin, size_t len) {
  uint8_t cmd[32];
  memset(cmd, 0, size_t(cmd));
  sprintf(cmd, "%sPIN%s",HC06_ATCMD_PREFIX, pin);
  return hc06_send_atcmd(cmd, strlen(cmd));
}

