# bootloader 移植

Bootloader简单来说就是**一段程序**，它在**整个系统上电后最先运行**，其作用就是引导程序下载到主FLASH里面。它的好处就是在调试接口没有引出时，能够方便地**更新或者升级mcu上的代码**，试想一下，你的产品卖出去后如果想要更新程序，通用jlink通过swd口烧写肯定是不现实的，顾客也不会有烧写器，而bootloader就能通过串口或者蓝牙或者**SD卡**等实现**一键升级**。顺便说一句，你的jlink能够烧写程序也是因为jlink自带bootloader。

## bootloader 地址分配
在没有Bootloader的情况下，代码一般从Flash的首地址开始运行，比如STM32F1的**flash首地址为0x0800 0000**;而当我们加入Bootloader后，整个FLASH的代码布局如图所示：
![bootloader地址分配](image\bootloader地址分配.jpg)

## bootloader 更新程序流程
如使用SD卡升级：
- 判断是否有SD卡插入
- 运行文件升级程序
- 升级成功后跳转到APP程序所在地址

![bootloader更新程序](image\bootloader示意图.jpg)

## BOOTLOADER 移植流程
- 查看原理图
- 图形化配置：时钟、文件系统、SPI、串口
- 移植AT代码，更新函数：SPI_ReadWriteByte
- 配置user_diskio.c
- 更正图形化配置，包括SPI时钟分频、FAT最大Sector size、堆栈最小内存
- SPI_ReadWriteByte用寄存器来写
- 在SPIFlashUpdate这里卡住，最后发现不是使用这个函数
- 移植InternalFlashUpdater，配置flash擦除写入
- 实现地址跳转到APP

## 注意事项
- 不清楚要修改`user_diskio.c`还是`diskio.c`，找不到`tff.c`
  - AT版本是修改了代码，直接使用HAL库生成的需要修改`user_diskio.c`
- 挂载失败
  - AT的挂载函数和ST的不同，`f_mount(&fs,"0:",1);`
- 最大错误，把InternalFlashUpdater看成是以太网升级，导致以为是SPI发送速率问题，需要修改SPI_ReadWriteByte为寄存器形式
  - 结果是SPIFlashUpdate是写入SD卡里面，InternalFlashUpdater则是内部升级，flash擦写实现更新，所以SPI只用来读SD卡里面的数据
- SPI_ReadWriteByte用寄存器来写时遇到许多问题，有关类型转换的
  - DR寄存器写入失败，导致`while (__HAL_SPI_GET_FLAG( &hspi1, SPI_FLAG_RXNE ) == RESET);`卡住
  - 返回的数据也没有类型转换，导致SD卡初始化不成功
```c	
uint8_t Rxdata;
/* 等待发送缓冲区为空，TXE事件 */
while (__HAL_SPI_GET_FLAG( &hspi1, SPI_FLAG_TXE ) == RESET);

/* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
*((__IO uint8_t *)&SPI1->DR) = TxData;

/* 等待接收缓冲区非空，RXNE事件 */
while (__HAL_SPI_GET_FLAG( &hspi1, SPI_FLAG_RXNE ) == RESET);

/* 读取数据寄存器，获取接收缓冲区数据 */
Rxdata = *(__IO uint8_t *)&SPI1->DR;

return Rxdata;
```
- 跳转到APP程序地址错误
  - `if(((*(__IO uint32_t *)APP_FLASH_ADDR) & 0x2FFDB000) == 0x20000000)`需要根据flash大小以及ram地址做对应修改
  - flash擦除写入错误，一个扇区写入，最小地址是0x08，再乘上每个地址有8个bit，一共是64位，创建的数组空间char只有8位，所以需要类型转换
```c
/* Program one page */
for (offset = 0; offset < PAGE_SIZE; offset+=8) {
    status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, page_address+offset, *(uint64_t *)(&image_buffer[offset]));
    if (status != HAL_OK) {
        printf("Program int falled.\n");
        ret = -2;
        goto exit;
    }
}
```
  - 额外操作：去初始化、屏蔽中断
```c
void jump_to_app(uint32_t APP_FLASH_ADDR)
{
	uint32_t JumpAddress;
	pFunction Jump_To_Application;

	/* 检查栈顶地址是否合法 */
	if(((*(__IO uint32_t *)APP_FLASH_ADDR) & 0x2FFDB000) == 0x20000000)
	{
		printf("jump\n");

		/* 去初始化 */
		HAL_SuspendTick();
		HAL_RCC_DeInit();
		HAL_DeInit();

		/* 屏蔽所有中断，防止在跳转过程中，中断干扰出现异常 */
		__disable_irq();

		/* 用户代码区第二个 字 为程序开始地址(复位地址) */
		JumpAddress = *(__IO uint32_t *) (APP_FLASH_ADDR + 4);

		/* Initialize user application's Stack Pointer */
		/* 初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址) */
		__set_MSP(*(__IO uint32_t *) APP_FLASH_ADDR);

		/* 类型转换 */
		Jump_To_Application = (pFunction) JumpAddress;

		/* 跳转到 APP */
		Jump_To_Application();
	}
}
```
