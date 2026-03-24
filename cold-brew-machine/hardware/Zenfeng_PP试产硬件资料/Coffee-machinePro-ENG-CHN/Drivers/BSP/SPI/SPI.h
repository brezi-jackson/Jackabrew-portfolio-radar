///*************************************************************************************************/	
// /* @attention
//  *
//  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
//  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
//  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
//  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
//  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
//  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
//**************************************************************************************************/
//#include "sys.h"

//#ifndef _SPI_H_
//#define _SPI_H_

///******************************************************************************************/
///* SPI1 引脚 定义 */

//#define SPI1_SCK_GPIO_PORT              GPIOA
//#define SPI1_SCK_GPIO_PIN               GPIO_PIN_5
//#define SPI1_SCK_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PB口时钟使能 */

//#define SPI1_MOSI_GPIO_PORT             GPIOA
//#define SPI1_MOSI_GPIO_PIN              GPIO_PIN_7
//#define SPI1_MOSI_GPIO_CLK_ENABLE()     do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PB口时钟使能 */

///* SPI1相关定义 */
//#define SPI1_SPI                        SPI1
//#define SPI1_SPI_CLK_ENABLE()           do{ __HAL_RCC_SPI1_CLK_ENABLE(); }while(0)    /* SPI1时钟使能 */

///******************************************************************************************/


////本测试程序使用的是硬件SPI接口驱动
////除了SPI时钟信号以及SPI读、写信号引脚不可更改，其他引脚都可以更改
////SPI的时钟引脚定义固定为PA5
////SPI的读数据引脚定义固定为PA6
////SPI的写数据引脚定义固定为PA7
// 
//u8 SPI_WriteByte(SPI_TypeDef* SPIx, u8 TxData);

//void SPI1_Init(void);
//void SPI_SetSpeed(SPI_TypeDef* SPIx,u8 SpeedSet);

//void DMA_init(DMA_Channel_TypeDef* DMAx_CHx);


//#endif







#ifndef _SPI_H_
#define _SPI_H_

#include "sys.h"

/******************************************************************************************/
/* SPI1 引脚定义 */

#define SPI1_SCK_GPIO_PORT              GPIOA
#define SPI1_SCK_GPIO_PIN               GPIO_PIN_5
#define SPI1_SCK_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)

#define SPI1_MOSI_GPIO_PORT             GPIOA
#define SPI1_MOSI_GPIO_PIN              GPIO_PIN_7
#define SPI1_MOSI_GPIO_CLK_ENABLE()     do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)

/* SPI1相关定义 */
#define SPI1_SPI                        SPI1
#define SPI1_SPI_CLK_ENABLE()           do{ __HAL_RCC_SPI1_CLK_ENABLE(); }while(0)

/******************************************************************************************/
/* SPI2 引脚定义 (用于W25Q32) */

#define SPI2_SCK_GPIO_PORT              GPIOB
#define SPI2_SCK_GPIO_PIN               GPIO_PIN_13
#define SPI2_SCK_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)

#define SPI2_MISO_GPIO_PORT             GPIOB
#define SPI2_MISO_GPIO_PIN              GPIO_PIN_14
#define SPI2_MISO_GPIO_CLK_ENABLE()     do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)

#define SPI2_MOSI_GPIO_PORT             GPIOB
#define SPI2_MOSI_GPIO_PIN              GPIO_PIN_15
#define SPI2_MOSI_GPIO_CLK_ENABLE()     do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)

#define SPI2_CS_GPIO_PORT               GPIOB
#define SPI2_CS_GPIO_PIN                GPIO_PIN_12
#define SPI2_CS_GPIO_CLK_ENABLE()       do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)

/* SPI2相关定义 */
#define SPI2_SPI                        SPI2
#define SPI2_SPI_CLK_ENABLE()           do{ __HAL_RCC_SPI2_CLK_ENABLE(); }while(0)

/******************************************************************************************/
/* W25Q32 命令定义 */

#define W25X_WriteEnable        0x06
#define W25X_WriteDisable       0x04
#define W25X_ReadStatusReg1     0x05
#define W25X_ReadStatusReg2     0x35
#define W25X_WriteStatusReg     0x01
#define W25X_PageProgram        0x02
#define W25X_QuadPageProgram    0x32
#define W25X_BlockErase         0xD8
#define W25X_SectorErase        0x20
#define W25X_ChipErase          0xC7
#define W25X_PowerDown          0xB9
#define W25X_ReleasePowerDown   0xAB
#define W25X_DeviceID           0xAB
#define W25X_ManufactDeviceID   0x90
#define W25X_JedecDeviceID      0x9F
#define W25X_ReadData           0x03
#define W25X_FastRead           0x0B
#define W25X_FastReadDual       0x3B
#define W25X_PageRead           0xD2
#define W25X_ReadUniqueID       0x4B
#define W25X_Enable4ByteAddr    0xB7
#define W25X_Exit4ByteAddr      0xE9

/* Coffee Mode 结构体定义 */
typedef struct {
    uint8_t *name;
    uint8_t *title_1;
    uint8_t *title_2;
    
    int16_t Heating_Temp;
    int16_t Cooling_Temp;
                    
    uint8_t Heating_spd;
    uint8_t Cooling_spd;
            
    int16_t Heating_Time;
    int16_t Cooling_Time;    
    
    uint16_t Heating_CWratio;
    uint16_t Cooling_CWratio;
    
    uint32_t Total_Time;
    uint16_t Total_ml;
    
    uint8_t mode_tag;
} Coffee_mode;

// 定义标志位存储地址 (第二个扇区起始地址为0x1000)
//#define FLAGS_STORAGE_ADDR 0x1000

// 标志位结构体
//typedef struct {
//    uint8_t languageMark;
//    uint8_t UnitMark;
//    uint8_t SoundMark;
//} SystemFlags;

//// 标志位结构体
//typedef struct {
//    uint8_t languageMark;
//    uint8_t UnitMark;
//    uint8_t SoundMark;
//    uint8_t checksum; // 校验字节
//} SystemFlags;

/* 标志位存储结构 */
typedef struct {
    uint8_t languageMark;
    uint8_t UnitMark;
    uint8_t SoundMark;
    uint8_t isValid;  // 有效性标志 (0xA5表示有效)
} SystemFlags;

/* 标志位存储地址定义 */
#define FLAGS_STORAGE_SECTOR  1  // 第二个扇区(扇区0是第一个)
#define FLAGS_STORAGE_ADDR    (FLAGS_STORAGE_SECTOR * 4096)  // 每个扇区4KB
#define FLAGS_VALID_MARKER    0xA5  // 有效性标志

/******************************************************************************************/
/* 函数声明 */

// SPI1相关函数
u8 SPI_WriteByte(SPI_TypeDef* SPIx, u8 TxData);
void SPI1_Init(void);
void SPI_SetSpeed(SPI_TypeDef* SPIx,u8 SpeedSet);

// SPI2相关函数
/* SPI2相关函数 */
/**
 * @brief 初始化SPI2接口(用于W25Q32)
 */
void SPI2_Init(void);

/**
 * @brief 初始化W25Q32闪存
 */
void W25Q32_Init(void);

/**
 * @brief 设置W25Q32片选引脚为低电平
 */
void W25Q32_CS_LOW(void);

/**
 * @brief 设置W25Q32片选引脚为高电平
 */
void W25Q32_CS_HIGH(void);

/**
 * @brief 读取W25Q32状态寄存器
 * @param reg: 寄存器编号(1或2)
 * @return 状态寄存器值
 */
uint8_t W25Q32_ReadStatusReg(uint8_t reg);

/**
 * @brief 发送写使能命令到W25Q32
 */
void W25Q32_WriteEnable(void);

/**
 * @brief 发送写禁止命令到W25Q32
 */
void W25Q32_WriteDisable(void);

/**
 * @brief 等待W25Q32写入操作完成
 */
void W25Q32_WaitForWriteEnd(void);

/**
 * @brief 读取W25Q32设备ID
 * @return 设备ID(制造商ID+设备ID)
 */
uint32_t W25Q32_ReadID(void);

/**
 * @brief 从W25Q32读取数据
 * @param addr: 起始地址
 * @param buf: 存储读取数据的缓冲区
 * @param len: 要读取的字节数
 */
void W25Q32_Read(uint32_t addr, uint8_t *buf, uint32_t len);

/**
 * @brief 向W25Q32写入一页数据(最多256字节)
 * @param addr: 起始地址(必须页对齐)
 * @param buf: 要写入的数据
 * @param len: 要写入的字节数(最大256)
 */
void W25Q32_WritePage(uint32_t addr, uint8_t *buf, uint16_t len);

/**
 * @brief 向W25Q32写入数据(自动处理跨页写入)
 * @param addr: 起始地址
 * @param buf: 要写入的数据
 * @param len: 要写入的字节数
 */
void W25Q32_Write(uint32_t addr, uint8_t *buf, uint32_t len);

/**
 * @brief 擦除W25Q32 4KB扇区
 * @param addr: 扇区内的任意地址
 */
void W25Q32_EraseSector(uint32_t addr);

/**
 * @brief 擦除W25Q32 64KB块
 * @param addr: 块内的任意地址
 */
void W25Q32_EraseBlock(uint32_t addr);

/**
 * @brief 擦除整个W25Q32芯片
 */
void W25Q32_EraseChip(void);

/* Coffee模式闪存操作函数 */
#define COFFEE_FLASH_MAX_COUNT 10      /* 最大模式数量 */
#define COFFEE_FLASH_ENTRY_SIZE 64     /* 每个模式条目大小 */
#define COFFEE_FLASH_PAGE_SIZE (2 + COFFEE_FLASH_ENTRY_SIZE)  // 加上2字节标志位
#define COFFEE_FLASH_BASE_ADDR 0x000100 /* 模式存储基地址 */
#define COFFEE_FLASH_PAGE_STEP 0x100   /* 每个模式地址间隔   一页存一个结构体，如0x000100,0x000200...*/  
#define COFFEE_NAME_LENGTH  10   /* 品种名字的最大长度 */

// 定义标志位
#define COFFEE_FLASH_FLAG_VALID 0x55AA // 有效数据标志
#define COFFEE_FLASH_FLAG_EMPTY 0xFFFF // 空数据标志

// 默认标志位值
#define DEFAULT_LANGUAGE_MARK 0
#define DEFAULT_UNIT_MARK     0
#define DEFAULT_SOUND_MARK    1


//Flash单个结构体追加写入
void CoffeeFlash_Append(Coffee_mode *mode);

//Flash单个结构体读取，index为对应下标，对应页地址，需要有3个字符数组缓冲区
void CoffeeFlash_Read(uint8_t index, Coffee_mode *mode, uint8_t *name, uint8_t *title1, uint8_t *title2);

//Flash单个结构体替换，index为对应下标，对应页地址
void CoffeeFlash_Replace(uint8_t index, Coffee_mode *mode);

//获取当前闪存中存储的模式数量
uint8_t CoffeeFlash_Count(void);

//清除闪存中所有模式数据
void CoffeeFlash_ClearAll(void);
/**
 * @brief 安全替换闪存中的模式(先备份再擦除最后写入)
 * @param index: 要替换的模式索引(0-9)
 * @param newMode: 新的模式结构体指针
 */
void CoffeeFlash_ReplaceSafely(uint8_t index, Coffee_mode *newMode);
/**
 * @brief 删除指定索引的模式并整理闪存空间
 * @param deleteIndex: 要删除的模式索引(0-9)
 */
void CoffeeFlash_DeleteAndCompact(uint8_t deleteIndex);
uint16_t CoffeeFlash_ReadPage(uint8_t index, uint8_t *data);

///**
// * @brief 保存系统标志位到Flash
// * @param flags: 包含标志位的结构体指针
// */
//void SaveSystemFlags(const SystemFlags* flags);

///**
// * @brief 从Flash读取系统标志位
// * @param flags: 存储读取标志位的结构体指针
// */
//void LoadSystemFlags(SystemFlags* flags);


///**
// * @brief 计算校验和
// * @param flags: 标志位结构体指针(不包含checksum字段)
// * @return 校验和
// */
//uint8_t CalculateChecksum(const SystemFlags* flags);
///**
// * @brief 初始化默认标志位
// * @param flags: 标志位结构体指针
// */
//void InitDefaultFlags(SystemFlags* flags);
///**
// * @brief 验证标志位数据是否有效
// * @param flags: 标志位结构体指针
// * @return 1-有效, 0-无效
// */
//uint8_t ValidateFlags(const SystemFlags* flags);

///**
// * @brief 保存系统标志位到Flash
// * @param flags: 包含标志位的结构体指针
// */
//void SaveSystemFlags(const SystemFlags* flags);

///**
// * @brief 从Flash读取系统标志位
// * @param flags: 存储读取标志位的结构体指针
// * @return 1-读取成功且数据有效, 0-数据无效(使用默认值)
// */
//uint8_t LoadSystemFlags(SystemFlags* flags);

/**
 * @brief 将系统标志位写入Flash
 * @param flags 包含标志位的结构体指针
 */
void SaveSystemFlags(SystemFlags *flags);

/**
 * @brief 从Flash读取系统标志位
 * @param flags 用于存储读取结果的标志位结构体指针
 * @return 0-成功读取有效数据, 1-数据无效或读取失败
 */
uint8_t LoadSystemFlags(SystemFlags *flags);

/**
 * @brief 初始化系统标志位(首次运行时调用)
 */
void InitSystemFlags(void);


#endif





