/*
 * wire.c
 *
 *  Created on: Apr 10, 2022
 *      Author: karol
 */
#include "wire.h"
#include "gpio.h"
#include "usart.h"

HAL_StatusTypeDef wire_init(void)
{
  return ;//HAL_TIM_Base_Start(&htim7);
}

static void set_baudrate(uint32_t baudrate)
{
	  huart3.Instance = USART3;
	  huart3.Init.BaudRate = baudrate;
	  huart3.Init.WordLength = UART_WORDLENGTH_8B;
	  huart3.Init.StopBits = UART_STOPBITS_1;
	  huart3.Init.Parity = UART_PARITY_NONE;
	  huart3.Init.Mode = UART_MODE_TX_RX;
	  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT;
	  huart3.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
	  if (HAL_HalfDuplex_Init(&huart3) != HAL_OK)
	  {
	    Error_Handler();
	  }
}

HAL_StatusTypeDef wire_reset(void)
{
 /* int rc;

  HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_RESET);
  delay_us(480);
  HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_SET);
  delay_us(70);
  rc = HAL_GPIO_ReadPin(DS_GPIO_Port, DS_Pin);
  delay_us(410);

  if (rc == 0)
    return HAL_OK;
  else
    return HAL_ERROR;*/
	  uint8_t data_out = 0xF0;
	  uint8_t data_in = 0;

	  set_baudrate(9600);
	  HAL_UART_Transmit(&huart3, &data_out, 1, HAL_MAX_DELAY);
	  HAL_UART_Receive(&huart3, &data_in, 1, HAL_MAX_DELAY);
	  set_baudrate(115200);

	  if (data_in != 0xF0)
	    return HAL_OK;
	  else
	    return HAL_ERROR;
}

static int read_bit(void)
{
 /* int rc;
  HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_RESET);
  delay_us(6);
  HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_SET);
  delay_us(9);
  rc = HAL_GPIO_ReadPin(DS_GPIO_Port, DS_Pin);
  delay_us(55);
  return rc;*/
  uint8_t data_out = 0xFF;
  uint8_t data_in = 0;
  HAL_UART_Transmit(&huart3, &data_out, 1, HAL_MAX_DELAY);
  HAL_UART_Receive(&huart3, &data_in, 1, HAL_MAX_DELAY);

  return data_in & 0x01;
}

uint8_t wire_read(void)
{
  uint8_t value = 0;
  int i;
  for (i = 0; i < 8; i++) {
    value >>= 1;
    if (read_bit())
      value |= 0x80;
  }
  return value;
}

static void write_bit(int value)
{
 /* if (value) {
    HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_RESET);
    delay_us(6);
    HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_SET);
    delay_us(64);
  } else {
    HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_RESET);
    delay_us(60);
    HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_SET);
    delay_us(10);
  }*/

  if (value) {
      uint8_t data_out = 0xff;
    HAL_UART_Transmit(&huart3, &data_out, 1, HAL_MAX_DELAY);
  } else {
      uint8_t data_out = 0x0;
    HAL_UART_Transmit(&huart3, &data_out, 1, HAL_MAX_DELAY);
  }
}

void wire_write(uint8_t byte)
{
  int i;
  for (i = 0; i < 8; i++) {
    write_bit(byte & 0x01);
    byte >>= 1;
  }
}

static uint8_t byte_crc(uint8_t crc, uint8_t byte)
{
  int i;
  for (i = 0; i < 8; i++) {
    uint8_t b = crc ^ byte;
    crc >>= 1;
    if (b & 0x01)
      crc ^= 0x8c;
    byte >>= 1;
  }
  return crc;
}

uint8_t wire_crc(const uint8_t* data, int len)
{
  int i;
    uint8_t crc = 0;

    for (i = 0; i < len; i++)
      crc = byte_crc(crc, data[i]);

    return crc;
}




