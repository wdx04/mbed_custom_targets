/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbed.h"
#include "sdio_device.h"

/* Extern variables ---------------------------------------------------------*/

SD_HandleTypeDef hsd;
EventFlags sd_transfer_state;

static HAL_StatusTypeDef SD_DMAConfigRx(SD_HandleTypeDef *hsd);
static HAL_StatusTypeDef SD_DMAConfigTx(SD_HandleTypeDef *hsd);

/**
  * @brief  Handles SD card interrupt request.
  * @retval None
  */
extern "C" void SDMMC1_IRQHandler(void)
{
   HAL_SD_IRQHandler(&hsd);
}

/**
  * @brief  Handles SD DMA transfer interrupt request.
  * @retval None
  */
extern "C" void DMA2_Channel5_IRQHandler(void)
{
  if((hsd.Context == (SD_CONTEXT_DMA | SD_CONTEXT_READ_SINGLE_BLOCK)) ||
     (hsd.Context == (SD_CONTEXT_DMA | SD_CONTEXT_READ_MULTIPLE_BLOCK)))
  {
    HAL_DMA_IRQHandler(hsd.hdmarx);
  }
  else if((hsd.Context == (SD_CONTEXT_DMA | SD_CONTEXT_WRITE_SINGLE_BLOCK)) ||
          (hsd.Context == (SD_CONTEXT_DMA | SD_CONTEXT_WRITE_MULTIPLE_BLOCK)))
  {
    HAL_DMA_IRQHandler(hsd.hdmatx);
  }
}

/**
 *
 * @param hsd:  Handle for SD handle Structure definition
 */
extern "C" void HAL_SD_MspInit(SD_HandleTypeDef *hsd)
{
  GPIO_InitTypeDef gpioinitstruct = {0};
  RCC_PeriphCLKInitTypeDef  RCC_PeriphClkInit;

  HAL_RCCEx_GetPeriphCLKConfig(&RCC_PeriphClkInit);

  /* Configure the SDMMC1 clock source. The clock is derived from the PLLSAI1 */
  /* Hypothesis is that PLLSAI1 VCO input is 8Mhz */
  RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SDMMC1;
  RCC_PeriphClkInit.PLLSAI1.PLLSAI1N = 24;
  RCC_PeriphClkInit.PLLSAI1.PLLSAI1Q = 4;
  RCC_PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK;
  RCC_PeriphClkInit.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_PLLSAI1;
  if (HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit) != HAL_OK)
  {
    while (1) {}
  }

  /* Enable SDMMC1 clock */
  __HAL_RCC_SDMMC1_CLK_ENABLE();

  /* Enable DMA2 clocks */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /* Common GPIO configuration */
  gpioinitstruct.Mode      = GPIO_MODE_AF_PP;
  gpioinitstruct.Pull      = GPIO_NOPULL;
  gpioinitstruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpioinitstruct.Alternate = GPIO_AF12_SDMMC1;

  /* GPIOC configuration */
  gpioinitstruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;

  HAL_GPIO_Init(GPIOC, &gpioinitstruct);

  /* GPIOD configuration */
  gpioinitstruct.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOD, &gpioinitstruct);

  /* NVIC configuration for SDMMC1 interrupts */
  HAL_NVIC_SetPriority(SDMMC1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(SDMMC1_IRQn);

  /* DMA initialization should be done here but , as there is only one channel for RX and TX it is configured and done directly when required*/
}

/**
 *
 * @param hsd:  Handle for SD handle Structure definition
 */
extern "C" void HAL_SD_MspDeInit(SD_HandleTypeDef *hsd)
{
  GPIO_InitTypeDef gpioinitstruct = {0};

  /* Disable SDMMC1 clock */
  __HAL_RCC_SDMMC1_CLK_DISABLE();

  /* Don't disable GPIOs clock */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /* Common GPIO configuration */
  gpioinitstruct.Mode      = GPIO_MODE_ANALOG;
  gpioinitstruct.Pull      = GPIO_NOPULL;
  gpioinitstruct.Speed     = GPIO_SPEED_FREQ_LOW;
  gpioinitstruct.Alternate = 0;

  /* GPIOC configuration */
  gpioinitstruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;

  HAL_GPIO_Init(GPIOC, &gpioinitstruct);

  /* GPIOD configuration */
  gpioinitstruct.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOD, &gpioinitstruct);

  /* NVIC configuration for SDMMC1 interrupts */
  HAL_NVIC_DisableIRQ(SDMMC1_IRQn);
}

/**
 * @brief  DeInitializes the SD MSP.
 * @param  hsd: SD handle
 * @param  Params : pointer on additional configuration parameters, can be NULL.
 */
extern "C" void SD_MspDeInit(SD_HandleTypeDef *hsd, void *Params)
{
}

/**
  * @brief Rx Transfer completed callbacks
  * @param hsd Pointer SD handle
  * @retval None
  */
extern "C" void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)
{
  sd_transfer_state.set(SD_TRANSFER_OK);
}

/**
  * @brief Tx Transfer completed callbacks
  * @param hsd Pointer to SD handle
  * @retval None
  */
extern "C" void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd)
{
  sd_transfer_state.set(SD_TRANSFER_WRITE_OK);
}

extern "C" void HAL_SD_ErrorCallback(SD_HandleTypeDef *hsd)
{
  sd_transfer_state.set(SD_TRANSFER_ERROR|SD_TRANSFER_WRITE_ERROR);
}

/**
 * @brief  Initializes the SD card device.
 * @retval SD status
 */
uint8_t SD_Init(void)
{
  /* uSD device interface configuration */
  hsd.Instance = SDMMC1;
  hsd.Init.ClockEdge           = SDMMC_CLOCK_EDGE_RISING;
  hsd.Init.ClockBypass         = SDMMC_CLOCK_BYPASS_DISABLE;
  hsd.Init.ClockPowerSave      = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  hsd.Init.BusWide             = SDMMC_BUS_WIDE_1B;
  hsd.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_ENABLE;
  hsd.Init.ClockDiv            = SDMMC_TRANSFER_CLK_DIV;

  for(int tries = 0; tries < 5; tries++)
  {
    /* HAL SD initialization */
    if (HAL_SD_Init(&hsd) != HAL_OK)
    {
        continue;
    }

    /* Configure SD Bus width */
    if (HAL_SD_ConfigWideBusOperation(&hsd, SDMMC_BUS_WIDE_4B) != HAL_OK)
    {
        printf("HAL_SD_ConfigWideBusOperation failed.\n");
        continue;
    }
    else
    {
        return MSD_OK;
    }

    HAL_SD_MspDeInit(&hsd);
    HAL_SD_MspInit(&hsd);
  }

  return  MSD_ERROR;
}

/**
 * @brief  DeInitializes the SD card device.
 * @retval SD status
 */
uint8_t SD_DeInit(void)
{
  uint8_t sd_state = MSD_OK;

  hsd.Instance = SDMMC1;
  /* HAL SD deinitialization */
  if (HAL_SD_DeInit(&hsd) != HAL_OK)
  {
    sd_state = MSD_ERROR;
  }

  __HAL_RCC_SDMMC1_FORCE_RESET();
  __HAL_RCC_SDMMC1_RELEASE_RESET();

  memset(&hsd, 0, sizeof(SD_HandleTypeDef));

  return  sd_state;
}

/**
 * @brief  Reads block(s) from a specified address in an SD card, in polling mode.
 * @param  pData: Pointer to the buffer that will contain the data to transmit
 * @param  ReadAddr: Address from where data is to be read
 * @param  NumOfBlocks: Number of SD blocks to read
 * @param  Timeout: Timeout for read operation
 * @retval SD status
 */
uint8_t SD_ReadBlocks(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks, uint32_t Timeout)
{
    uint8_t sd_state = MSD_OK;

    int result_result = HAL_SD_ReadBlocks(&hsd, (uint8_t *)pData, ReadAddr, NumOfBlocks, Timeout);
    if (result_result != HAL_OK)
    {
        sd_state = MSD_ERROR;
    }

    return sd_state;
}

/**
 * @brief  Writes block(s) to a specified address in an SD card, in polling mode.
 * @param  pData: Pointer to the buffer that will contain the data to transmit
 * @param  WriteAddr: Address from where data is to be written
 * @param  NumOfBlocks: Number of SD blocks to write
 * @param  Timeout: Timeout for write operation
 * @retval SD status
 */
uint8_t SD_WriteBlocks(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks, uint32_t Timeout)
{
    uint8_t sd_state = MSD_OK;

    if (HAL_SD_WriteBlocks(&hsd, (uint8_t *)pData, WriteAddr, NumOfBlocks, Timeout) != HAL_OK)
    {
        sd_state = MSD_ERROR;
    }

    return sd_state;
}

/**
 * @brief  Reads block(s) from a specified address in an SD card, in DMA mode.
 * @param  pData: Pointer to the buffer that will contain the data to transmit
 * @param  ReadAddr: Address from where data is to be read
 * @param  NumOfBlocks: Number of SD blocks to read
 * @retval SD status
 */
uint8_t SD_ReadBlocks_DMA(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks)
{
  HAL_StatusTypeDef  sd_state = HAL_OK;

  sd_transfer_state.clear(SD_TRANSFER_OK|SD_TRANSFER_ERROR);

  /* Invalidate the dma tx handle*/
  hsd.hdmatx = NULL;

  /* Prepare the dma channel for a read operation */
  sd_state = SD_DMAConfigRx(&hsd);

  if (sd_state == HAL_OK)
  {
    /* Read block(s) in DMA transfer mode */
    sd_state = HAL_SD_ReadBlocks_DMA(&hsd, (uint8_t *)pData, ReadAddr, NumOfBlocks);
  }

  if (sd_state == HAL_OK)
  {
    return MSD_OK;
  }
  else
  {
    return MSD_ERROR;
  }
}

/**
 * @brief  Writes block(s) to a specified address in an SD card, in DMA mode.
 * @param  pData: Pointer to the buffer that will contain the data to transmit
 * @param  WriteAddr: Address from where data is to be written
 * @param  NumOfBlocks: Number of SD blocks to write
 * @retval SD status
 */
uint8_t SD_WriteBlocks_DMA(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks)
{
  HAL_StatusTypeDef  sd_state = HAL_OK;

  sd_transfer_state.clear(SD_TRANSFER_WRITE_OK|SD_TRANSFER_WRITE_ERROR);

  /* Invalidate the dma rx handle*/
  hsd.hdmarx = NULL;

  /* Prepare the dma channel for a read operation */
  sd_state = SD_DMAConfigTx(&hsd);

  if (sd_state == HAL_OK)
  {
    /* Write block(s) in DMA transfer mode */
    sd_state = HAL_SD_WriteBlocks_DMA(&hsd, (uint8_t *)pData, WriteAddr, NumOfBlocks);
  }

  if (sd_state == HAL_OK)
  {
    return MSD_OK;
  }
  else
  {
    return MSD_ERROR;
  }
}

/**
 * @brief  Erases the specified memory area of the given SD card.
 * @param  StartAddr: Start byte address
 * @param  EndAddr: End byte address
 * @retval SD status
 */
uint8_t SD_Erase(uint32_t StartAddr, uint32_t EndAddr)
{
  HAL_StatusTypeDef  sd_state = HAL_OK;

  sd_state = HAL_SD_Erase(&hsd, StartAddr, EndAddr);

  if (sd_state == HAL_OK)
  {
    return MSD_OK;
  }
  else
  {
    return MSD_ERROR;
  }
}

/**
 * @brief  Gets the current SD card data status.
 * @param  None
 * @retval Data transfer state.
 *          This value can be one of the following values:
 *            @arg  SD_TRANSFER_OK: No data transfer is acting
 *            @arg  SD_TRANSFER_BUSY: Data transfer is acting
 */
uint8_t SD_GetCardState(void)
{
  HAL_SD_CardStateTypedef card_state;
  card_state = HAL_SD_GetCardState(&hsd);

  if (card_state == HAL_SD_CARD_TRANSFER)
  {
    return (SD_TRANSFER_OK);
  }
  else if ((card_state == HAL_SD_CARD_SENDING) ||
           (card_state == HAL_SD_CARD_RECEIVING) ||
           (card_state == HAL_SD_CARD_PROGRAMMING))
  {
    return (SD_TRANSFER_BUSY);
  }
  else
  {
    return (SD_TRANSFER_ERROR);
  }
}

/**
 * @brief  Get SD information about specific SD card.
 * @param  CardInfo: Pointer to HAL_SD_CardInfoTypedef structure
 * @retval None
 */
void SD_GetCardInfo(SD_Cardinfo_t *CardInfo)
{
    /* Get SD card Information, copy structure for portability */
    HAL_SD_CardInfoTypeDef HAL_CardInfo;

    HAL_SD_GetCardInfo(&hsd, &HAL_CardInfo);

    if (CardInfo)
    {
        CardInfo->CardType = HAL_CardInfo.CardType;
        CardInfo->CardVersion = HAL_CardInfo.CardVersion;
        CardInfo->Class = HAL_CardInfo.Class;
        CardInfo->RelCardAdd = HAL_CardInfo.RelCardAdd;
        CardInfo->BlockNbr = HAL_CardInfo.BlockNbr;
        CardInfo->BlockSize = HAL_CardInfo.BlockSize;
        CardInfo->LogBlockNbr = HAL_CardInfo.LogBlockNbr;
        CardInfo->LogBlockSize = HAL_CardInfo.LogBlockSize;
    }
}

/**
 * @brief  Wait for completion of DMA read operation
 * @retval DMA operation result
 *          This value can be one of the following values:
 *            @arg  SD_TRANSFER_OK: Data read success
 *            @arg  SD_TRANSFER_ERROR: Data read error
 */
uint8_t SD_DMA_WaitForReadCplt(int timeout_ms)
{
    return sd_transfer_state.wait_any_for(SD_TRANSFER_OK|SD_TRANSFER_ERROR, chrono::milliseconds(timeout_ms));
}

/**
 * @brief  Wait for completion of DMA write operation
 * @retval DMA operation result
 *          This value can be one of the following values:
 *            @arg  SD_TRANSFER_OK: Data write success
 *            @arg  SD_TRANSFER_BUSY: Data write error
 */
uint8_t SD_DMA_WaitForWriteCplt(int timeout_ms)
{
    return (sd_transfer_state.wait_any_for(SD_TRANSFER_WRITE_OK|SD_TRANSFER_WRITE_ERROR, chrono::milliseconds(timeout_ms)) >> 2);
}

/**
  * @brief Configure the DMA to receive data from the SD card
  * @retval
  *  HAL_ERROR or HAL_OK
  */
static HAL_StatusTypeDef SD_DMAConfigRx(SD_HandleTypeDef *hsd)
{
  static DMA_HandleTypeDef hdma_rx;
  HAL_StatusTypeDef status = HAL_ERROR;

  /* Configure DMA Rx parameters */
  hdma_rx.Init.Request             = DMA_REQUEST_7;
  hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  hdma_rx.Init.Priority            = DMA_PRIORITY_VERY_HIGH;

  hdma_rx.Instance = DMA2_Channel5;

  /* Associate the DMA handle */
  __HAL_LINKDMA(hsd, hdmarx, hdma_rx);

  /* Stop any ongoing transfer and reset the state*/
  HAL_DMA_Abort(&hdma_rx);

  /* Deinitialize the Channel for new transfer */
  HAL_DMA_DeInit(&hdma_rx);

  /* Configure the DMA Channel */
  status = HAL_DMA_Init(&hdma_rx);

  /* NVIC configuration for DMA transfer complete interrupt */
  HAL_NVIC_SetPriority(DMA2_Channel5_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(DMA2_Channel5_IRQn);

  return (status);
}

/**
  * @brief Configure the DMA to transmit data to the SD card
  * @retval
  *  HAL_ERROR or HAL_OK
  */
static HAL_StatusTypeDef SD_DMAConfigTx(SD_HandleTypeDef *hsd)
{
  static DMA_HandleTypeDef hdma_tx;
  HAL_StatusTypeDef status;

  /* Configure DMA Tx parameters */
  hdma_tx.Init.Request             = DMA_REQUEST_7;
  hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  hdma_tx.Init.Priority            = DMA_PRIORITY_VERY_HIGH;

  hdma_tx.Instance = DMA2_Channel5;

  /* Associate the DMA handle */
  __HAL_LINKDMA(hsd, hdmatx, hdma_tx);

  /* Stop any ongoing transfer and reset the state*/
  HAL_DMA_Abort(&hdma_tx);

  /* Deinitialize the Channel for new transfer */
  HAL_DMA_DeInit(&hdma_tx);

  /* Configure the DMA Channel */
  status = HAL_DMA_Init(&hdma_tx);

  /* NVIC configuration for DMA transfer complete interrupt */
  HAL_NVIC_SetPriority(DMA2_Channel5_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(DMA2_Channel5_IRQn);

  return (status);
}
