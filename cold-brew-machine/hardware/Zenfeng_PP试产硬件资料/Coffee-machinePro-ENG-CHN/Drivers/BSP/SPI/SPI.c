///**************************************************************************************************/	
// /* @attention
//  *
//  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
//  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
//  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
//  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
//  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
//  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
//**************************************************************************************************/		
//#include "spi.h"

///*****************************************************************************
// * @name       :u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 Byte)
// * @date       :2018-08-09 
// * @function   :Write a byte of data using STM32's hardware SPI
// * @parameters :SPIx: SPI type,x for 1,2,3
//                Byte:Data to be written
// * @retvalue   :Data received by the bus
//******************************************************************************/
// 

///*****************************************************************************
// * @name       :void SPI_SetSpeed(SPI_TypeDef* SPIx,u8 SpeedSet)
// * @date       :2018-08-09 
// * @function   :Set hardware SPI Speed
// * @parameters :SPIx: SPI type,x for 1,2,3
//                SpeedSet:0-high speed
//												 1-low speed
// * @retvalue   :None
//******************************************************************************/
//void SPI_SetSpeed(SPI_TypeDef* SPIx,u8 SpeedSet)
//{
//	SPIx->CR1&=0XFFC7;
//	if(SpeedSet==1)//高速
//	{
//		SPIx->CR1|=SPI_BAUDRATEPRESCALER_2;//Fsck=Fpclk/2	
//	}
//	else//低速
//	{
//		SPIx->CR1|=SPI_BAUDRATEPRESCALER_2; //Fsck=Fpclk/32
//	}
//	SPIx->CR1|=1<<6; //SPI设备使能
//} 

///*****************************************************************************
// * @name       :void SPI2_Init(void)	
// * @date       :2018-08-09 
// * @function   :Initialize the STM32 hardware SPI2
// * @parameters :None
// * @retvalue   :None
//******************************************************************************/
//SPI_HandleTypeDef SPI_InitStructure;

//void SPI1_Init(void)	
//{	
//	//SPI1配置选项
//	SPI1_SPI_CLK_ENABLE(); /* SPI1时钟使能 */
//	    
//	SPI_InitStructure.Instance 					= SPI1_SPI;                                /* SPI1 */
//    SPI_InitStructure.Init.Mode 				= SPI_MODE_MASTER;                        /* 设置SPI工作模式，设置为主模式 */
//    SPI_InitStructure.Init.Direction 			= SPI_DIRECTION_2LINES;              /* 设置SPI单向或者双向的数据模式:SPI设置为双线模式 */
//    SPI_InitStructure.Init.DataSize 			= SPI_DATASIZE_8BIT;                  /* 设置SPI的数据大小:SPI发送接收8位帧结构 */
//    SPI_InitStructure.Init.CLKPolarity 			= SPI_POLARITY_HIGH;               /* 串行同步时钟的空闲状态为高电平 */
//    SPI_InitStructure.Init.CLKPhase 			= SPI_PHASE_2EDGE;                    /* 串行同步时钟的第二个跳变沿（上升或下降）数据被采样 */
//    SPI_InitStructure.Init.NSS 					= SPI_NSS_SOFT;                            /* NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制 */
//    SPI_InitStructure.Init.BaudRatePrescaler 	= SPI_BAUDRATEPRESCALER_2; /* 定义波特率预分频的值:波特率预分频值为256 */
//    SPI_InitStructure.Init.FirstBit 			= SPI_FIRSTBIT_MSB;                   /* 指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始 */
//	SPI_InitStructure.Init.TIMode 				= SPI_TIMODE_DISABLE;                   /* 关闭TI模式 */
//	SPI_InitStructure.Init.CRCCalculation 		= SPI_CRCCALCULATION_DISABLE;   /* 关闭硬件CRC校验 */
//    SPI_InitStructure.Init.CRCPolynomial 		= 7;                             /* CRC值计算的多项式 */
//    
//	HAL_SPI_Init(&SPI_InitStructure);  
//		
//	//使能SPI1
//    __HAL_SPI_ENABLE(&SPI_InitStructure); /* 使能SPI1 */
//}

///**
// * @brief       SPI底层驱动，时钟使能，引脚配置
// *   @note      此函数会被HAL_SPI_Init()调用
// * @param       hspi:SPI句柄
// * @retval      无
// */
//void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//    
//    if (hspi->Instance == SPI1_SPI)
//    {
//		SPI1_SCK_GPIO_CLK_ENABLE();  /* SPI1_SCK脚时钟使能 */
//		SPI1_MOSI_GPIO_CLK_ENABLE(); /* SPI1_MOSI脚时钟使能 */

//		/* SCK引脚模式设置(复用输出) */
//		GPIO_InitStructure.Pin = SPI1_SCK_GPIO_PIN;
//		GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
//		GPIO_InitStructure.Pull = GPIO_PULLUP;
//		GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
//		HAL_GPIO_Init(SPI1_SCK_GPIO_PORT, &GPIO_InitStructure);

//		/* MOSI引脚模式设置(复用输出) */
//		GPIO_InitStructure.Pin = SPI1_MOSI_GPIO_PIN;
//		HAL_GPIO_Init(SPI1_MOSI_GPIO_PORT, &GPIO_InitStructure);
//    }
//}

////SPIx 读写一个字节
////TxData:要写入的字节
////返回值:读取到的字节
//u8 SPI_WriteByte(SPI_TypeDef* SPIx, u8 TxData)
//{
//	uint8_t rxdata;
//	
//	
//	while((SPIx->SR&1<<1)==0){}		//等待发送区空 	
//	SPIx->DR=TxData;
////	HAL_SPI_TransmitReceive(&SPI_InitStructure, &TxData, &rxdata, 1, 1000);
//    return rxdata; /* 返回收到的数据 */	    
//}

//DMA_HandleTypeDef  g_dma_handle;            /* DMA句柄 */

//void DMA_BYTE_init(DMA_Channel_TypeDef* DMAx_CHx)
//{
//	__HAL_RCC_DMA1_CLK_ENABLE();                      /* DMA1时钟使能 */
//    
//    __HAL_LINKDMA(&SPI_InitStructure, hdmatx, g_dma_handle);           /* 将DMA与USART1联系起来(发送DMA) */
//    
//    /* Tx DMA配置 */
//    g_dma_handle.Instance = DMAx_CHx;                               /* USART1_TX使用的DMA通道为: DMA1_Channel4 */
//    g_dma_handle.Init.Direction = DMA_MEMORY_TO_PERIPH;             /* DIR = 1 , 存储器到外设模式 */
//    g_dma_handle.Init.PeriphInc = DMA_PINC_DISABLE;                 /* 外设非增量模式 */
//    g_dma_handle.Init.MemInc = DMA_MINC_ENABLE;                     /* 存储器增量模式 */
//    g_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;    /* 外设数据长度:8位 */
//    g_dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;       /* 存储器数据长度:8位 */
//    g_dma_handle.Init.Mode = DMA_NORMAL;                            /* DMA模式:正常模式 */
//    g_dma_handle.Init.Priority = DMA_PRIORITY_MEDIUM;               /* 中等优先级 */

//    HAL_DMA_Init(&g_dma_handle);
//}

//void DMA_HALFWORD_init(DMA_Channel_TypeDef* DMAx_CHx)
//{
//	__HAL_RCC_DMA1_CLK_ENABLE();                      /* DMA1时钟使能 */
//    
//    __HAL_LINKDMA(&SPI_InitStructure, hdmatx, g_dma_handle);           /* 将DMA与USART1联系起来(发送DMA) */
//    
//    /* Tx DMA配置 */
//    g_dma_handle.Instance = DMAx_CHx;                               /* USART1_TX使用的DMA通道为: DMA1_Channel4 */
//    g_dma_handle.Init.Direction = DMA_MEMORY_TO_PERIPH;             /* DIR = 1 , 存储器到外设模式 */
//    g_dma_handle.Init.PeriphInc = DMA_PINC_DISABLE;                 /* 外设非增量模式 */
//    g_dma_handle.Init.MemInc = DMA_MINC_ENABLE;                     /* 存储器增量模式 */
//    g_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;    /* 外设数据长度:8位 */
//    g_dma_handle.Init.MemDataAlignment = DMA_PDATAALIGN_HALFWORD;       /* 存储器数据长度:8位 */
//    g_dma_handle.Init.Mode = DMA_NORMAL;                            /* DMA模式:正常模式 */
//    g_dma_handle.Init.Priority = DMA_PRIORITY_MEDIUM;               /* 中等优先级 */

//    HAL_DMA_Init(&g_dma_handle);
//}
//                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
//void SPI2_DMA_Transmit(uint8_t *pData, uint16_t size) {
//    HAL_SPI_Transmit_DMA(&SPI_InitStructure, pData, size);
//}





#include "spi.h"

SPI_HandleTypeDef SPI1_InitStructure;
SPI_HandleTypeDef SPI2_InitStructure;
uint8_t flashData = 0;
/*****************************************************************************
 * @函数名称 :u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 TxData)
 * @日期     :2018-08-09 
 * @功能     :使用STM32硬件SPI写入一个字节数据
 * @参数     :SPIx: SPI类型,x可以是1,2,3
              Byte:要写入的数据
 * @返回值   :总线接收到的数据
******************************************************************************/
u8 SPI_WriteByte(SPI_TypeDef* SPIx, u8 TxData)
{
    uint8_t rxdata;
    
    while((SPIx->SR&1<<1)==0){}     //等待发送区空     
    SPIx->DR=TxData;
    return rxdata;                  //返回收到的数据       
}

/*****************************************************************************
 * @函数名称 :void SPI_SetSpeed(SPI_TypeDef* SPIx,u8 SpeedSet)
 * @日期     :2018-08-09 
 * @功能     :设置硬件SPI速度
 * @参数     :SPIx: SPI类型,x可以是1,2,3
              SpeedSet:0-高速 1-低速
 * @返回值   :无
******************************************************************************/
void SPI_SetSpeed(SPI_TypeDef* SPIx,u8 SpeedSet)
{
    SPIx->CR1&=0XFFC7;
    if(SpeedSet==1) //高速
    {
        SPIx->CR1|=SPI_BAUDRATEPRESCALER_2; //Fsck=Fpclk/2    
    }
    else //低速
    {
        SPIx->CR1|=SPI_BAUDRATEPRESCALER_2; //Fsck=Fpclk/32
    }
    SPIx->CR1|=1<<6; //SPI设备使能
} 

/*****************************************************************************
 * @函数名称 :void SPI1_Init(void)    
 * @日期     :2018-08-09 
 * @功能     :初始化STM32硬件SPI1
 * @参数     :无
 * @返回值   :无
******************************************************************************/
void SPI1_Init(void)    
{    
    //SPI1配置选项
    SPI1_SPI_CLK_ENABLE(); /* SPI1时钟使能 */
        
    SPI1_InitStructure.Instance                  = SPI1_SPI;                                /* SPI1 */
    SPI1_InitStructure.Init.Mode                 = SPI_MODE_MASTER;                        /* 设置SPI工作模式，设置为主模式 */
    SPI1_InitStructure.Init.Direction            = SPI_DIRECTION_2LINES;              /* 设置SPI单向或者双向的数据模式:SPI设置为双线模式 */
    SPI1_InitStructure.Init.DataSize             = SPI_DATASIZE_8BIT;                  /* 设置SPI的数据大小:SPI发送接收8位帧结构 */
    SPI1_InitStructure.Init.CLKPolarity          = SPI_POLARITY_HIGH;               /* 串行同步时钟的空闲状态为高电平 */
    SPI1_InitStructure.Init.CLKPhase             = SPI_PHASE_2EDGE;                    /* 串行同步时钟的第二个跳变沿（上升或下降）数据被采样 */
    SPI1_InitStructure.Init.NSS                  = SPI_NSS_SOFT;                            /* NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制 */
    SPI1_InitStructure.Init.BaudRatePrescaler    = SPI_BAUDRATEPRESCALER_2; /* 定义波特率预分频的值:波特率预分频值为256 */
    SPI1_InitStructure.Init.FirstBit             = SPI_FIRSTBIT_MSB;                   /* 指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始 */
    SPI1_InitStructure.Init.TIMode               = SPI_TIMODE_DISABLE;                   /* 关闭TI模式 */
    SPI1_InitStructure.Init.CRCCalculation       = SPI_CRCCALCULATION_DISABLE;   /* 关闭硬件CRC校验 */
    SPI1_InitStructure.Init.CRCPolynomial        = 7;                             /* CRC值计算的多项式 */
    
    HAL_SPI_Init(&SPI1_InitStructure);  
        
    //使能SPI1
    __HAL_SPI_ENABLE(&SPI1_InitStructure); /* 使能SPI1 */
}

/**
 * @brief       SPI底层驱动，时钟使能，引脚配置
 * @note        此函数会被HAL_SPI_Init()调用
 * @param       hspi:SPI句柄
 * @retval      无
 */
/**
 * @brief       SPI底层驱动，时钟使能，引脚配置
 * @note        此函数会被HAL_SPI_Init()调用
 * @param       hspi:SPI句柄
 * @retval      无
 */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    if (hspi->Instance == SPI1_SPI)
    {
        SPI1_SCK_GPIO_CLK_ENABLE();  /* SPI1_SCK脚时钟使能 */
        SPI1_MOSI_GPIO_CLK_ENABLE(); /* SPI1_MOSI脚时钟使能 */

        /* SCK引脚模式设置(复用输出) */
        GPIO_InitStructure.Pin = SPI1_SCK_GPIO_PIN;
        GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStructure.Pull = GPIO_PULLUP;
        GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(SPI1_SCK_GPIO_PORT, &GPIO_InitStructure);

        /* MOSI引脚模式设置(复用输出) */
        GPIO_InitStructure.Pin = SPI1_MOSI_GPIO_PIN;
        HAL_GPIO_Init(SPI1_MOSI_GPIO_PORT, &GPIO_InitStructure);
    }
    else if (hspi->Instance == SPI2_SPI)
    {
        // SPI2 SCK, MISO, MOSI引脚时钟使能
        SPI2_SCK_GPIO_CLK_ENABLE();
        SPI2_MISO_GPIO_CLK_ENABLE();
        SPI2_MOSI_GPIO_CLK_ENABLE();
        SPI2_CS_GPIO_CLK_ENABLE();
        
        // SCK引脚模式设置(复用输出)
        GPIO_InitStructure.Pin = SPI2_SCK_GPIO_PIN;
        GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStructure.Pull = GPIO_PULLUP;
        GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(SPI2_SCK_GPIO_PORT, &GPIO_InitStructure);
        
        // MISO引脚模式设置(输入)
        GPIO_InitStructure.Pin = SPI2_MISO_GPIO_PIN;
        GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
        GPIO_InitStructure.Pull = GPIO_PULLUP;
        HAL_GPIO_Init(SPI2_MISO_GPIO_PORT, &GPIO_InitStructure);
        
        // MOSI引脚模式设置(复用输出)
        GPIO_InitStructure.Pin = SPI2_MOSI_GPIO_PIN;
        GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
        HAL_GPIO_Init(SPI2_MOSI_GPIO_PORT, &GPIO_InitStructure);
        
        // CS引脚模式设置(普通输出)
        GPIO_InitStructure.Pin = SPI2_CS_GPIO_PIN;
        GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStructure.Pull = GPIO_PULLUP;
        GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(SPI2_CS_GPIO_PORT, &GPIO_InitStructure);
        
        // 默认CS高电平
        HAL_GPIO_WritePin(SPI2_CS_GPIO_PORT, SPI2_CS_GPIO_PIN, GPIO_PIN_SET);
    }
}

//初始化STM32硬件SPI2
void SPI2_Init(void) {    
    SPI2_SPI_CLK_ENABLE();
    SPI2_InitStructure.Instance = SPI2;
    SPI2_InitStructure.Init.Mode = SPI_MODE_MASTER;          // 主模式
    SPI2_InitStructure.Init.Direction = SPI_DIRECTION_2LINES; // 全双工
    SPI2_InitStructure.Init.DataSize = SPI_DATASIZE_8BIT;     // 8位数据
    SPI2_InitStructure.Init.CLKPolarity = SPI_POLARITY_LOW;   // 时钟极性：低电平空闲
    SPI2_InitStructure.Init.CLKPhase = SPI_PHASE_1EDGE;       // 数据采样在第1个边沿
    SPI2_InitStructure.Init.NSS = SPI_NSS_SOFT;               // 软件控制CS
    SPI2_InitStructure.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2; // 分频系数（根据时钟调整）
    SPI2_InitStructure.Init.FirstBit = SPI_FIRSTBIT_MSB;      // MSB优先
    HAL_SPI_Init(&SPI2_InitStructure);
    __HAL_SPI_ENABLE(&SPI2_InitStructure);
}

/*****************************************************************************
 * @函数名称 :void W25Q32_CS_LOW(void)
 * @日期     :2023-08-01 
 * @功能     :设置W25Q32 CS引脚为低电平
 * @参数     :无
 * @返回值   :无
******************************************************************************/
void W25Q32_CS_LOW(void)
{
    HAL_GPIO_WritePin(SPI2_CS_GPIO_PORT, SPI2_CS_GPIO_PIN, GPIO_PIN_RESET);
}

/*****************************************************************************
 * @函数名称 :void W25Q32_CS_HIGH(void)
 * @日期     :2023-08-01 
 * @功能     :设置W25Q32 CS引脚为高电平
 * @参数     :无
 * @返回值   :无
******************************************************************************/
void W25Q32_CS_HIGH(void)
{
    HAL_GPIO_WritePin(SPI2_CS_GPIO_PORT, SPI2_CS_GPIO_PIN, GPIO_PIN_SET);
}

/*****************************************************************************
 * @函数名称 :void W25Q32_Init(void)
 * @日期     :2023-08-01 
 * @功能     :初始化W25Q32闪存
 * @参数     :无
 * @返回值   :无
******************************************************************************/
void W25Q32_Init(void)
{
    SPI2_Init();
    // 等待闪存上电完成
    HAL_Delay(10);
}

/*****************************************************************************
 * @函数名称 :uint8_t W25Q32_ReadStatusReg(uint8_t reg)
 * @日期     :2023-08-01 
 * @功能     :读取W25Q32状态寄存器
 * @参数     :reg: 状态寄存器编号(1或2)
 * @返回值   :状态寄存器值
******************************************************************************/
uint8_t W25Q32_ReadStatusReg(uint8_t reg)
{
    uint8_t cmd, status;
    
    if(reg == 1) cmd = W25X_ReadStatusReg1;
    else if(reg == 2) cmd = W25X_ReadStatusReg2;
    else return 0;
    
    W25Q32_CS_LOW();
    HAL_SPI_Transmit(&SPI2_InitStructure, &cmd, 1, 100);
    HAL_SPI_Receive(&SPI2_InitStructure, &status, 1, 100);
    W25Q32_CS_HIGH();
    
    return status;
}

/*****************************************************************************
 * @函数名称 :void W25Q32_WriteEnable(void)
 * @日期     :2023-08-01 
 * @功能     :向W25Q32发送写使能命令
 * @参数     :无
 * @返回值   :无
******************************************************************************/
void W25Q32_WriteEnable(void)
{
    uint8_t cmd = W25X_WriteEnable;
    
    W25Q32_CS_LOW();
    HAL_SPI_Transmit(&SPI2_InitStructure, &cmd, 1, 100);
    W25Q32_CS_HIGH();
}

/*****************************************************************************
 * @函数名称 :void W25Q32_WriteDisable(void)
 * @日期     :2023-08-01 
 * @功能     :向W25Q32发送写禁止命令
 * @参数     :无
 * @返回值   :无
******************************************************************************/
void W25Q32_WriteDisable(void)
{
    uint8_t cmd = W25X_WriteDisable;
    
    W25Q32_CS_LOW();
    HAL_SPI_Transmit(&SPI2_InitStructure, &cmd, 1, 100);
    W25Q32_CS_HIGH();
}

/*****************************************************************************
 * @函数名称 :void W25Q32_WaitForWriteEnd(void)
 * @日期     :2023-08-01 
 * @功能     :等待W25Q32完成写入操作
 * @参数     :无
 * @返回值   :无
******************************************************************************/
void W25Q32_WaitForWriteEnd(void)
{
    uint8_t status;
    
    do {
        status = W25Q32_ReadStatusReg(1);
    } while(status & 0x01); // 检查BUSY位
}

/*****************************************************************************
 * @函数名称 :uint32_t W25Q32_ReadID(void)
 * @日期     :2023-08-01 
 * @功能     :读取W25Q32 ID
 * @参数     :无
 * @返回值   :设备ID (制造商ID + 设备ID)
******************************************************************************/
uint32_t W25Q32_ReadID(void)
{
    uint8_t cmd = W25X_JedecDeviceID;
    uint8_t id[3];
    
    W25Q32_CS_LOW();
    HAL_SPI_Transmit(&SPI2_InitStructure, &cmd, 1, 100);
    HAL_SPI_Receive(&SPI2_InitStructure, id, 3, 100);
    W25Q32_CS_HIGH();
    
    return (id[0] << 16) | (id[1] << 8) | id[2];
}

/*****************************************************************************
 * @函数名称 :void W25Q32_Read(uint32_t addr, uint8_t *buf, uint32_t len)
 * @日期     :2023-08-01 
 * @功能     :从W25Q32读取数据
 * @参数     :addr: 起始地址
 *              buf: 存储读取数据的缓冲区
 *              len: 要读取的字节数
 * @返回值   :无
******************************************************************************/
void W25Q32_Read(uint32_t addr, uint8_t *buf, uint32_t len)
{
    uint8_t cmd[4];
    
    cmd[0] = W25X_ReadData;
    cmd[1] = (addr >> 16) & 0xFF;
    cmd[2] = (addr >> 8) & 0xFF;
    cmd[3] = addr & 0xFF;
    
    W25Q32_CS_LOW();
    HAL_SPI_Transmit(&SPI2_InitStructure, cmd, 4, 100);
    HAL_SPI_Receive(&SPI2_InitStructure, buf, len, 1000);
    W25Q32_CS_HIGH();
}

/*****************************************************************************
 * @函数名称 :void W25Q32_WritePage(uint32_t addr, uint8_t *buf, uint16_t len)
 * @日期     :2023-08-01 
 * @功能     :向W25Q32写入一页数据(最多256字节)
 * @参数     :addr: 起始地址(必须页对齐)
 *              buf: 要写入的数据
 *              len: 要写入的字节数(最大256)
 * @返回值   :无
******************************************************************************/
void W25Q32_WritePage(uint32_t addr, uint8_t *buf, uint16_t len)
{
    uint8_t cmd[4];
    
    W25Q32_WriteEnable();
    
    cmd[0] = W25X_PageProgram;
    cmd[1] = (addr >> 16) & 0xFF;
    cmd[2] = (addr >> 8) & 0xFF;
    cmd[3] = addr & 0xFF;
    
    W25Q32_CS_LOW();
    HAL_SPI_Transmit(&SPI2_InitStructure, cmd, 4, 100);
    HAL_SPI_Transmit(&SPI2_InitStructure, buf, len, 1000);
    W25Q32_CS_HIGH();
    
    W25Q32_WaitForWriteEnd();
}

/*****************************************************************************
 * @函数名称 :void W25Q32_Write(uint32_t addr, uint8_t *buf, uint32_t len)
 * @日期     :2023-08-01 
 * @功能     :向W25Q32写入数据(处理多页写入)
 * @参数     :addr: 起始地址
 *              buf: 要写入的数据
 *              len: 要写入的字节数
 * @返回值   :无
******************************************************************************/
void W25Q32_Write(uint32_t addr, uint8_t *buf, uint32_t len)
{
    uint32_t pageOffset;
    uint32_t pageRemain;
    uint32_t writeLen;
    uint32_t pos = 0;
    
    while(pos < len)
    {
        pageOffset = addr % 256;
        pageRemain = 256 - pageOffset;
        writeLen = (len - pos) > pageRemain ? pageRemain : (len - pos);
        
        W25Q32_WritePage(addr, &buf[pos], writeLen);
        
        addr += writeLen;
        pos += writeLen;
    }
}

/*****************************************************************************
 * @函数名称 :void W25Q32_EraseSector(uint32_t addr)
 * @日期     :2023-08-01 
 * @功能     :擦除4KB扇区
 * @参数     :addr: 扇区内的任意地址
 * @返回值   :无
******************************************************************************/
void W25Q32_EraseSector(uint32_t addr)
{
    uint8_t cmd[4];
    
    W25Q32_WriteEnable();
    
    cmd[0] = W25X_SectorErase;
    cmd[1] = (addr >> 16) & 0xFF;
    cmd[2] = (addr >> 8) & 0xFF;
    cmd[3] = addr & 0xFF;
    
    W25Q32_CS_LOW();
    HAL_SPI_Transmit(&SPI2_InitStructure, cmd, 4, 100);
    W25Q32_CS_HIGH();
    
    W25Q32_WaitForWriteEnd();
}

/*****************************************************************************
 * @函数名称 :void W25Q32_EraseBlock(uint32_t addr)
 * @日期     :2023-08-01 
 * @功能     :擦除64KB块
 * @参数     :addr: 块内的任意地址
 * @返回值   :无
******************************************************************************/
void W25Q32_EraseBlock(uint32_t addr)
{
    uint8_t cmd[4];
    
    W25Q32_WriteEnable();
    
    cmd[0] = W25X_BlockErase;
    cmd[1] = (addr >> 16) & 0xFF;
    cmd[2] = (addr >> 8) & 0xFF;
    cmd[3] = addr & 0xFF;
    
    W25Q32_CS_LOW();
    HAL_SPI_Transmit(&SPI2_InitStructure, cmd, 4, 100);
    W25Q32_CS_HIGH();
    
    W25Q32_WaitForWriteEnd();
}

/*****************************************************************************
 * @函数名称 :void W25Q32_EraseChip(void)
 * @日期     :2023-08-01 
 * @功能     :擦除整个芯片
 * @参数     :无
 * @返回值   :无
******************************************************************************/
void W25Q32_EraseChip(void)
{
    uint8_t cmd = W25X_ChipErase;
    
    W25Q32_WriteEnable();
    
    W25Q32_CS_LOW();
    HAL_SPI_Transmit(&SPI2_InitStructure, &cmd, 1, 100);
    W25Q32_CS_HIGH();
    
    W25Q32_WaitForWriteEnd();
}

/* 全局序列化缓冲区，用于临时存储序列化后的结构体数据 */
static uint8_t serializeBuf[COFFEE_FLASH_ENTRY_SIZE];

/**
 * @brief 将Coffee_mode结构体序列化为字节数组
 * @param mode 要序列化的结构体指针
 * @param buf 输出缓冲区(必须至少有COFFEE_FLASH_ENTRY_SIZE大小)
 */
static void Serialize(const Coffee_mode *mode, uint8_t *buf)
{
    /* 初始化缓冲区为0xFF(Flash擦除后的状态) */
    memset(buf, 0xFF, COFFEE_FLASH_ENTRY_SIZE);  // 避免脏数据

    /* 序列化字符串字段 */
    memcpy(&buf[0],  mode->name,     COFFEE_NAME_LENGTH);    // 复制名称(10字节)
    memcpy(&buf[10], mode->title_1,  COFFEE_NAME_LENGTH);   // 复制标题1(10字节)
    memcpy(&buf[20], mode->title_2,  COFFEE_NAME_LENGTH);   // 复制标题2(10字节)

    /* 序列化数值字段(从buf[30]开始) */
    uint8_t *p = &buf[30];
    memcpy(p, &mode->Heating_Temp, 2); p += 2;   // 加热温度(2字节)
    memcpy(p, &mode->Cooling_Temp, 2); p += 2;   // 冷却温度(2字节)
    *p++ = mode->Heating_spd;                    // 加热速度(1字节)
    *p++ = mode->Cooling_spd;                    // 冷却速度(1字节)
    memcpy(p, &mode->Heating_Time, 2); p += 2;   // 加热时间(2字节)
    memcpy(p, &mode->Cooling_Time, 2); p += 2;   // 冷却时间(2字节)
    memcpy(p, &mode->Heating_CWratio, 2); p += 2;// 加热水/咖啡比例(2字节)
    memcpy(p, &mode->Cooling_CWratio, 2); p += 2;// 冷却水/咖啡比例(2字节)
    memcpy(p, &mode->Total_Time, 4); p += 4;     // 总时间(4字节)
    memcpy(p, &mode->Total_ml, 2); p += 2;       // 总毫升数(2字节)
    *p++ = mode->mode_tag;                       // 模式标签(1字节)
}

/**
 * @brief 将字节数组反序列化为Coffee_mode结构体
 * @param buf 输入数据缓冲区
 * @param mode 输出的结构体指针
 * @param name 外部提供的名称存储空间
 * @param title1 外部提供的标题1存储空间
 * @param title2 外部提供的标题2存储空间
 */
static void Deserialize(const uint8_t *buf, Coffee_mode *mode, uint8_t *name, uint8_t *title1, uint8_t *title2)
{
    /* 反序列化字符串字段 */
    memcpy(name, &buf[0], COFFEE_NAME_LENGTH);     mode->name = name;    // 名称(10字节)
    memcpy(title1, &buf[10], COFFEE_NAME_LENGTH);  mode->title_1 = title1; // 标题1(10字节)
    memcpy(title2, &buf[20], COFFEE_NAME_LENGTH);  mode->title_2 = title2; // 标题2(10字节)

    /* 反序列化数值字段(从buf[30]开始) */
    const uint8_t *p = &buf[30];
    memcpy(&mode->Heating_Temp, p, 2); p += 2;    // 加热温度(2字节)
    memcpy(&mode->Cooling_Temp, p, 2); p += 2;    // 冷却温度(2字节)
    mode->Heating_spd = *p++;                    // 加热速度(1字节)
    mode->Cooling_spd = *p++;                    // 冷却速度(1字节)
    memcpy(&mode->Heating_Time, p, 2); p += 2;    // 加热时间(2字节)
    memcpy(&mode->Cooling_Time, p, 2); p += 2;    // 冷却时间(2字节)
    memcpy(&mode->Heating_CWratio, p, 2); p += 2; // 加热水/咖啡比例(2字节)
    memcpy(&mode->Cooling_CWratio, p, 2); p += 2; // 冷却水/咖啡比例(2字节)
    memcpy(&mode->Total_Time, p, 4); p += 4;      // 总时间(4字节)
    memcpy(&mode->Total_ml, p, 2); p += 2;        // 总毫升数(2字节)
    mode->mode_tag = *p++;                       // 模式标签(1字节)
}

/**
 * @brief 获取当前Flash中有效记录的数量
 * @return 有效记录数(0-COFFEE_FLASH_MAX_COUNT)
 */
uint8_t CoffeeFlash_Count(void)
{
    uint8_t buf[COFFEE_FLASH_ENTRY_SIZE];  // 临时数据缓冲区

    /* 遍历所有可能的记录位置 */
    for (uint8_t i = 0; i < COFFEE_FLASH_MAX_COUNT; i++) {
        /* 读取当前页的标志位和数据 */
        uint16_t flag = CoffeeFlash_ReadPage(i, buf);
        /* 如果标志位不是有效标志，返回当前索引(即有效记录数) */
        if (flag != COFFEE_FLASH_FLAG_VALID) {
            return i;
        }
    }
    /* 如果全部有效，返回最大记录数 */
    return COFFEE_FLASH_MAX_COUNT;
}

/**
 * @brief 写入一页数据到Flash
 * @param index 页索引(0-COFFEE_FLASH_MAX_COUNT-1)
 * @param flag 标志位(COFFEE_FLASH_FLAG_VALID或COFFEE_FLASH_FLAG_EMPTY)
 * @param data 要写入的数据(COFFEE_FLASH_ENTRY_SIZE大小)
 */
void CoffeeFlash_WritePage(uint8_t index, uint16_t flag, uint8_t *data)
{
    /* 计算目标地址 = 基地址 + 索引 * 页间隔 */
    uint32_t addr = COFFEE_FLASH_BASE_ADDR + index * COFFEE_FLASH_PAGE_STEP;
    uint8_t buf[COFFEE_FLASH_PAGE_SIZE];  // 页缓冲区(标志位+数据)

    /* 设置标志位(前2字节) */
    buf[0] = (flag >> 8) & 0xFF;  // 标志位高字节
    buf[1] = flag & 0xFF;         // 标志位低字节
    
    /* 复制数据部分 */
    memcpy(&buf[2], data, COFFEE_FLASH_ENTRY_SIZE);
    
    /* 写入Flash */
    W25Q32_Write(addr, buf, COFFEE_FLASH_PAGE_SIZE);
}

/**
 * @brief 从Flash读取一页数据
 * @param index 页索引(0-COFFEE_FLASH_MAX_COUNT-1)
 * @param data 输出数据缓冲区(COFFEE_FLASH_ENTRY_SIZE大小)
 * @return 读取到的标志位
 */
uint16_t CoffeeFlash_ReadPage(uint8_t index, uint8_t *data)
{
    /* 计算目标地址 = 基地址 + 索引 * 页间隔 */
    uint32_t addr = COFFEE_FLASH_BASE_ADDR + index * COFFEE_FLASH_PAGE_STEP;
    uint8_t buf[COFFEE_FLASH_PAGE_SIZE];  // 页缓冲区(标志位+数据)

    /* 从Flash读取完整页 */
    W25Q32_Read(addr, buf, COFFEE_FLASH_PAGE_SIZE);

    /* 组合标志位(前2字节) */
    uint16_t flag = (buf[0] << 8) | buf[1];
    
    /* 复制数据部分 */
    memcpy(data, &buf[2], COFFEE_FLASH_ENTRY_SIZE);

    return flag;  // 返回标志位
}

/**
 * @brief 追加写入一条记录(自动找到下一个可用位置)
 * @param mode 要写入的结构体指针
 */
void CoffeeFlash_Append(Coffee_mode *mode)
{
    /* 获取当前有效记录数 */
    uint8_t index = CoffeeFlash_Count();
    
    /* 检查是否已满 */
    if (index >= COFFEE_FLASH_MAX_COUNT) return;

    /* 序列化结构体 */
    Serialize(mode, serializeBuf);
    
    /* 写入Flash，标记为有效 */
    CoffeeFlash_WritePage(index, COFFEE_FLASH_FLAG_VALID, serializeBuf);
}

/**
 * @brief 替换指定位置的记录(直接覆盖，不保证安全)
 * @param index 要替换的记录索引
 * @param mode 新的结构体数据
 */
void CoffeeFlash_Replace(uint8_t index, Coffee_mode *mode)
{
    /* 检查索引是否有效 */
    if (index >= COFFEE_FLASH_MAX_COUNT) return;

    /* 序列化结构体 */
    Serialize(mode, serializeBuf);
    
    /* 计算目标地址 */
    uint32_t addr = COFFEE_FLASH_BASE_ADDR + index * COFFEE_FLASH_PAGE_STEP;
    
    /* 直接写入数据部分(不更新标志位) */
    W25Q32_Write(addr, serializeBuf, COFFEE_FLASH_ENTRY_SIZE);
}

/**
 * @brief 从Flash读取指定记录
 * @param index 记录索引
 * @param mode 输出的结构体指针
 * @param name 外部提供的名称存储空间
 * @param title1 外部提供的标题1存储空间
 * @param title2 外部提供的标题2存储空间
 */
void CoffeeFlash_Read(uint8_t index, Coffee_mode *mode, uint8_t *name, uint8_t *title1, uint8_t *title2)
{
    /* 检查索引是否有效 */
    if (index >= COFFEE_FLASH_MAX_COUNT) return;

    uint8_t buf[COFFEE_FLASH_PAGE_SIZE];  // 页缓冲区
    /* 计算目标地址 */
    uint32_t addr = COFFEE_FLASH_BASE_ADDR + index * COFFEE_FLASH_PAGE_STEP;

    /* 从Flash读取完整页 */
    W25Q32_Read(addr, buf, COFFEE_FLASH_PAGE_SIZE);

    /* 检查标志位是否有效 */
    uint16_t flag = (buf[0] << 8) | buf[1];
    if (flag != COFFEE_FLASH_FLAG_VALID) return;  // 无效数据，直接返回

    /* 解析数据部分 */
    Deserialize(&buf[2], mode, name, title1, title2);
}

/**
 * @brief 清空所有记录(填充0xFF)
 */
void CoffeeFlash_ClearAll(void)
{
    /* 遍历所有记录位置 */
    for (uint8_t i = 0; i < COFFEE_FLASH_MAX_COUNT; i++) {
        uint8_t blank[COFFEE_FLASH_ENTRY_SIZE];
        /* 准备空白数据(0xFF) */
        memset(blank, 0xFF, COFFEE_FLASH_ENTRY_SIZE);
        
        /* 计算目标地址 */
        uint32_t addr = COFFEE_FLASH_BASE_ADDR + i * COFFEE_FLASH_PAGE_STEP;
        
        /* 写入空白数据 */
        W25Q32_Write(addr, blank, COFFEE_FLASH_ENTRY_SIZE);
    }
}

/**
 * @brief 安全替换指定记录(保证数据一致性)
 * @param index 要替换的记录索引
 * @param newMode 新的结构体数据
 * 实现方式：
 * 1. 读取所有有效记录到内存
 * 2. 修改指定记录
 * 3. 擦除整个扇区
 * 4. 重新写入所有记录
 */
void CoffeeFlash_ReplaceSafely(uint8_t index, Coffee_mode *newMode)
{
    /* 检查索引是否有效 */
    if (index >= COFFEE_FLASH_MAX_COUNT) return;

    /* 临时存储所有有效记录 */
    Coffee_mode tempList[COFFEE_FLASH_MAX_COUNT];
    /* 为字符串字段分配缓冲区 */
    uint8_t nameBuf[COFFEE_FLASH_MAX_COUNT][COFFEE_NAME_LENGTH], 
            title1Buf[COFFEE_FLASH_MAX_COUNT][COFFEE_NAME_LENGTH], 
            title2Buf[COFFEE_FLASH_MAX_COUNT][COFFEE_NAME_LENGTH];
    uint8_t validCount = 0;  // 有效记录计数器

    /* 读取所有有效记录 */
    for (uint8_t i = 0; i < COFFEE_FLASH_MAX_COUNT; i++) {
        uint8_t raw[COFFEE_FLASH_ENTRY_SIZE];
        /* 读取当前页的标志位和数据 */
        uint16_t flag = CoffeeFlash_ReadPage(i, raw);

        /* 如果是有效记录，反序列化并存储 */
        if (flag == COFFEE_FLASH_FLAG_VALID) {
            Deserialize(raw, &tempList[validCount], nameBuf[validCount], title1Buf[validCount], title2Buf[validCount]);
            validCount++;
        }
    }

    /* 如果索引有效，替换对应记录 */
    if (index < validCount) {
        tempList[index] = *newMode;  // 复制结构体内容
        /* 更新指针指向缓冲区 */
        tempList[index].name = nameBuf[index];
        tempList[index].title_1 = title1Buf[index];
        tempList[index].title_2 = title2Buf[index];
        /* 复制字符串内容 */
        memcpy(nameBuf[index], newMode->name, COFFEE_NAME_LENGTH);
        memcpy(title1Buf[index], newMode->title_1, COFFEE_NAME_LENGTH);
        memcpy(title2Buf[index], newMode->title_2, COFFEE_NAME_LENGTH);
    }

    /* 擦除整个扇区(确保数据一致性) */
    W25Q32_EraseSector(0x000000);

    /* 重新写入所有记录 */
    for (uint8_t i = 0; i < validCount; i++) {
        /* 序列化当前记录 */
        Serialize(&tempList[i], serializeBuf);
        /* 写入Flash，标记为有效 */
        CoffeeFlash_WritePage(i, COFFEE_FLASH_FLAG_VALID, serializeBuf);
    }
}

/**
 * @brief 删除指定记录并整理空间
 * @param deleteIndex 要删除的记录索引
 * 实现方式：
 * 1. 读取所有有效记录到内存
 * 2. 删除指定记录
 * 3. 前移后续记录
 * 4. 擦除整个扇区
 * 5. 重新写入剩余记录
 * 6. 清空剩余空间
 */
void CoffeeFlash_DeleteAndCompact(uint8_t deleteIndex)
{
    /* 检查索引是否有效 */
    if (deleteIndex >= COFFEE_FLASH_MAX_COUNT) return;

    /* 临时存储所有有效记录 */
    Coffee_mode tempList[COFFEE_FLASH_MAX_COUNT];
    /* 为字符串字段分配缓冲区 */
    uint8_t nameBuf[COFFEE_FLASH_MAX_COUNT][COFFEE_NAME_LENGTH], 
            title1Buf[COFFEE_FLASH_MAX_COUNT][COFFEE_NAME_LENGTH], 
            title2Buf[COFFEE_FLASH_MAX_COUNT][COFFEE_NAME_LENGTH];
    uint8_t validCount = 0;  // 有效记录计数器

    /* 读取所有有效记录 */
    for (uint8_t i = 0; i < COFFEE_FLASH_MAX_COUNT; i++) {
        uint8_t raw[COFFEE_FLASH_ENTRY_SIZE];
        /* 读取当前页的标志位和数据 */
        uint16_t flag = CoffeeFlash_ReadPage(i, raw);

        /* 如果是有效记录，反序列化并存储 */
        if (flag == COFFEE_FLASH_FLAG_VALID) {
            Deserialize(raw, &tempList[validCount], nameBuf[validCount], title1Buf[validCount], title2Buf[validCount]);
            validCount++;
        }
    }

    /* 检查要删除的索引是否有效 */
    if (deleteIndex >= validCount) return;

    /* 删除操作(通过前移后续记录实现) */
    for (uint8_t i = deleteIndex; i < validCount - 1; i++) {
        tempList[i] = tempList[i + 1];  // 复制结构体内容
        /* 更新指针指向缓冲区 */
        tempList[i].name = nameBuf[i];
        tempList[i].title_1 = title1Buf[i];
        tempList[i].title_2 = title2Buf[i];
        /* 复制字符串内容 */
        memcpy(nameBuf[i], nameBuf[i + 1], COFFEE_NAME_LENGTH);
        memcpy(title1Buf[i], title1Buf[i + 1], COFFEE_NAME_LENGTH);
        memcpy(title2Buf[i], title2Buf[i + 1], COFFEE_NAME_LENGTH);
    }

    validCount--;  // 减少有效记录数

    /* 擦除整个扇区(确保数据一致性) */
    W25Q32_EraseSector(0x000000);

    /* 重新写入剩余记录 */
    for (uint8_t i = 0; i < validCount; i++) {
        /* 序列化当前记录 */
        Serialize(&tempList[i], serializeBuf);
        /* 写入Flash，标记为有效 */
        CoffeeFlash_WritePage(i, COFFEE_FLASH_FLAG_VALID, serializeBuf);
    }

    /* 准备空白数据(0xFF) */
    uint8_t blank[COFFEE_FLASH_ENTRY_SIZE];
    memset(blank, 0xFF, COFFEE_FLASH_ENTRY_SIZE);

    /* 清空剩余空间 */
    for (uint8_t i = validCount; i < COFFEE_FLASH_MAX_COUNT; i++) {
        /* 计算目标地址 */
        uint32_t addr = COFFEE_FLASH_BASE_ADDR + i * COFFEE_FLASH_PAGE_STEP;
        /* 写入空白数据 */
        W25Q32_Write(addr, blank, COFFEE_FLASH_ENTRY_SIZE);
    }
}

///**
// * @brief 保存系统标志位到Flash
// * @param flags: 包含标志位的结构体指针
// */
//void SaveSystemFlags(const SystemFlags* flags) {
////    // 确保Flash已初始化
////    W25Q32_Init();
//    // 擦除整个扇区 (4KB)
//    W25Q32_EraseSector(FLAGS_STORAGE_ADDR);
//    // 写入标志位
//    W25Q32_Write(FLAGS_STORAGE_ADDR, (uint8_t*)flags, sizeof(SystemFlags));
//    // 等待写入完成
//    W25Q32_WaitForWriteEnd();
//}

///**
// * @brief 从Flash读取系统标志位
// * @param flags: 存储读取标志位的结构体指针
// */
//void LoadSystemFlags(SystemFlags* flags) {
////    // 确保Flash已初始化
////    W25Q32_Init();
//    
//    // 从Flash读取标志位
//    W25Q32_Read(FLAGS_STORAGE_ADDR, (uint8_t*)flags, sizeof(SystemFlags));
//}

///**
// * @brief 计算校验和
// * @param flags: 标志位结构体指针(不包含checksum字段)
// * @return 校验和
// */
//uint8_t CalculateChecksum(const SystemFlags* flags) {
//    return flags->languageMark ^ flags->UnitMark ^ flags->SoundMark;
//}

///**
// * @brief 初始化默认标志位
// * @param flags: 标志位结构体指针
// */
//void InitDefaultFlags(SystemFlags* flags) {
//    flags->languageMark = DEFAULT_LANGUAGE_MARK;
//    flags->UnitMark = DEFAULT_UNIT_MARK;
//    flags->SoundMark = DEFAULT_SOUND_MARK;
//    flags->checksum = CalculateChecksum(flags);
//}

///**
// * @brief 验证标志位数据是否有效
// * @param flags: 标志位结构体指针
// * @return 1-有效, 0-无效
// */
//uint8_t ValidateFlags(const SystemFlags* flags) {
//    // 检查校验和
//    uint8_t expectedChecksum = CalculateChecksum(flags);
//    
//    // 检查Flash擦除状态(全FF)
//    uint8_t isErased = (flags->languageMark == 0xFF && 
//                       flags->UnitMark == 0xFF && 
//                       flags->SoundMark == 0xFF);
//    
//    return (!isErased) && (flags->checksum == expectedChecksum);
//}

///**
// * @brief 保存系统标志位到Flash
// * @param flags: 包含标志位的结构体指针
// */
//void SaveSystemFlags(const SystemFlags* flags) {
//    // 确保Flash已初始化
//    W25Q32_Init();
//    
//    // 擦除整个扇区 (4KB)
//    W25Q32_EraseSector(FLAGS_STORAGE_ADDR);
//    
//    // 计算校验和并保存
//    SystemFlags flagsToSave = *flags;
//    flagsToSave.checksum = CalculateChecksum(flags);
//    
//    // 写入标志位
//    W25Q32_Write(FLAGS_STORAGE_ADDR, (uint8_t*)&flagsToSave, sizeof(SystemFlags));
//    
//    // 等待写入完成
//    W25Q32_WaitForWriteEnd();
//}

///**
// * @brief 从Flash读取系统标志位
// * @param flags: 存储读取标志位的结构体指针
// * @return 1-读取成功且数据有效, 0-数据无效(使用默认值)
// */
//uint8_t LoadSystemFlags(SystemFlags* flags) {
//    // 确保Flash已初始化
//    W25Q32_Init();
//    
//    // 从Flash读取标志位
//    W25Q32_Read(FLAGS_STORAGE_ADDR, (uint8_t*)flags, sizeof(SystemFlags));
//    
//    // 验证数据有效性
//    if(ValidateFlags(flags)) {
//        return 1; // 数据有效
//    } else {
//        // 数据无效，初始化默认值
//        InitDefaultFlags(flags);
//        return 0; // 数据无效
//    }
//}

/**
 * @brief 将系统标志位写入Flash
 * @param flags 包含标志位的结构体指针
 */
void SaveSystemFlags(SystemFlags *flags) {
    // 设置有效性标志
    flags->isValid = FLAGS_VALID_MARKER;
    
    // 擦除整个扇区(必须先擦除才能写入)
    W25Q32_EraseSector(FLAGS_STORAGE_ADDR);
    
    // 写入数据
    W25Q32_Write(FLAGS_STORAGE_ADDR, (uint8_t*)flags, sizeof(SystemFlags));
    
    // 等待写入完成
    W25Q32_WaitForWriteEnd();
}

/**
 * @brief 从Flash读取系统标志位
 * @param flags 用于存储读取结果的标志位结构体指针
 * @return 0-成功读取有效数据, 1-数据无效或读取失败
 */
uint8_t LoadSystemFlags(SystemFlags *flags) {
    // 读取数据
    W25Q32_Read(FLAGS_STORAGE_ADDR, (uint8_t*)flags, sizeof(SystemFlags));
    
    // 检查有效性标志
    if(flags->isValid != FLAGS_VALID_MARKER) {
        // 数据无效，返回默认值
        flags->languageMark = 0;
        flags->UnitMark = 0;
        flags->SoundMark = 1;
        flags->isValid = 0;
        return 1;
    }
    
    return 0;
}

/**
 * @brief 初始化系统标志位(首次运行时调用)
 */
void InitSystemFlags(void) {
    SystemFlags flags;
    
    // 尝试读取标志位
    if(LoadSystemFlags(&flags)) {
        // 如果读取失败(可能是第一次运行)，设置默认值并保存
        flags.languageMark = 0;
        flags.UnitMark = 0;
        flags.SoundMark = 1;
        flags.isValid = FLAGS_VALID_MARKER;
        
        SaveSystemFlags(&flags);
    }
}


