#include <SHT40.h>

// 定义SHT40的I2C地址和命令
#define SHT40_ADDR 0x44
#define MEASURE_CMD 0xFD


// 读取SHT40温湿度的实现
// https://item.szlcsc.com/3188621.html
#define SHT40_ADDR 0x44
#define SHT40_MEASURE_CMD 0xFD
#define SHT40_MEASURE_DELAY 10  // 根据数据手册调整这个时间
void readSHT40(float *temperature, float *humidity) {

    const uint8_t cmd = SHT40_MEASURE_CMD;  // 测量命令
    uint8_t rx_bytes[6];
    uint32_t temp_raw, humid_raw;
    float temp_f, humid_f;

    IIC_Init();
    // 发送测量命令
    IIC_Start();
    IIC_Send_Byte(SHT40_ADDR << 1);  // 发送写命令（设备地址 + 写位）
    if (IIC_Wait_Ack()) {
        IIC_Stop();
        *temperature = 1.0;
        *humidity = 0.0;
        return;  // 如果没有收到应答，停止
    }

    IIC_Send_Byte(cmd);  // 发送测量命令
    if (IIC_Wait_Ack()) {
        IIC_Stop();
        *temperature = 2.0;
        *humidity = 0.0;
        return;  // 如果没有收到应答，停止
    }
    IIC_Stop();

    DelayMs(SHT40_MEASURE_DELAY); // 等待数据准备

    // 重新启动I2C并设置为读模式
    IIC_Start();
    IIC_Send_Byte((SHT40_ADDR << 1) | 1);  // 设备地址 + 读位
    if (IIC_Wait_Ack()) {
        IIC_Stop();
        *temperature = 3.0;
        *humidity = 0.0;
        return;  // 如果没有收到应答，停止
    }

    for (int i = 0; i < 6; i++) {
        rx_bytes[i] = IIC_Read_Byte(i != 5);  // 读取6个字节
    }
    IIC_Stop();
    IIC_Deinit();

    // 处理读取的数据
    temp_raw = ((uint32_t)rx_bytes[0] << 8) | rx_bytes[1];
    humid_raw = ((uint32_t)rx_bytes[3] << 8) | rx_bytes[4];

    temp_f = -45.0 + 175.0 * temp_raw / 65535.0;
    humid_f = -6.0 + 125.0 * humid_raw / 65535.0;

    // 限制湿度范围在0-100%
    humid_f = (humid_f > 100.0f) ? 100.0f : (humid_f < 0.0f) ? 0.0f : humid_f;

    *temperature = temp_f;
    *humidity = humid_f;
}

void readSHT40_step1() {

    const uint8_t cmd = SHT40_MEASURE_CMD;  // 测量命令

    IIC_Init();
    // 发送测量命令
    IIC_Start();
    IIC_Send_Byte(SHT40_ADDR << 1);  // 发送写命令（设备地址 + 写位）
    if (IIC_Wait_Ack()) {
        IIC_Stop();
        return;  // 如果没有收到应答，停止
    }

    IIC_Send_Byte(cmd);  // 发送测量命令
    if (IIC_Wait_Ack()) {
        IIC_Stop();
        return;  // 如果没有收到应答，停止
    }
    IIC_Stop();

//    DelayMs(SHT40_MEASURE_DELAY); // 等待数据准备
}

void readSHT40_step2(float *temperature, float *humidity) {

    uint8_t rx_bytes[6];
    uint32_t temp_raw, humid_raw;
    float temp_f, humid_f;

    // 重新启动I2C并设置为读模式
    IIC_Start();
    IIC_Send_Byte((SHT40_ADDR << 1) | 1);  // 设备地址 + 读位
    if (IIC_Wait_Ack()) {
        IIC_Stop();
        *temperature = 3.0;
        *humidity = 0.0;
        return;  // 如果没有收到应答，停止
    }

    for (int i = 0; i < 6; i++) {
        rx_bytes[i] = IIC_Read_Byte(i != 5);  // 读取6个字节
    }
    IIC_Stop();
    IIC_Deinit();

    // 处理读取的数据
    temp_raw = ((uint32_t)rx_bytes[0] << 8) | rx_bytes[1];
    humid_raw = ((uint32_t)rx_bytes[3] << 8) | rx_bytes[4];

    temp_f = -45.0 + 175.0 * temp_raw / 65535.0;
    humid_f = -6.0 + 125.0 * humid_raw / 65535.0;

    // 限制湿度范围在0-100%
    humid_f = (humid_f > 100.0f) ? 100.0f : (humid_f < 0.0f) ? 0.0f : humid_f;

    *temperature = temp_f;
    *humidity = humid_f;
}


//初始化IIC
void IIC_Init(void)
{
    // SCL PA15 SDA PA14
    GPIOB_ModeCfg(PIN_SDA | PIN_SCL, GPIO_ModeOut_PP_5mA);

    SCL_HIGH();
    SDA_HIGH();
}

void IIC_Deinit(void)
{
    GPIOB_ModeCfg(PIN_SDA | PIN_SCL, GPIO_ModeIN_PU);
}

//产生IIC起始信号
void IIC_Start(void)
{
    SDA_OUT();     //sda线输出
    SDA_HIGH();
    SCL_HIGH();
    DelayUs(4);
    SDA_LOW(); //START: when CLK is high, DATA changes from high to low
    DelayUs(4);
    SCL_LOW(); //钳住I2C总线，准备发送或接收数据
}
//产生IIC停止信号
void IIC_Stop(void)
{
    SDA_OUT(); //sda线输出
    SCL_LOW();
    SDA_LOW(); //STOP: when CLK is high DATA changes from low to high
    DelayUs(4);
    SCL_HIGH();
    SDA_HIGH(); //发送I2C总线结束信号
    DelayUs(4);
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
    u8 ucErrTime = 0;
    SDA_IN();      //SDA设置为输入
    SDA_HIGH(); DelayUs(1);
    SCL_HIGH(); DelayUs(1);
    while (READ_SDA())
    {
        ucErrTime++;
        if (ucErrTime > 250)
        {
            IIC_Stop();
            return 1;
        }
    }
    SCL_LOW(); //时钟输出0
    return 0;
}

//产生ACK应答
void IIC_Ack(void)
{
    SCL_LOW();
    SDA_OUT();
    SDA_LOW();
    DelayUs(2);
    SCL_HIGH();
    DelayUs(2);
    SCL_LOW();
}

//不产生ACK应答
void IIC_NAck(void)
{
    SCL_LOW();
    SDA_OUT();
    SDA_HIGH();
    DelayUs(2);
    SCL_HIGH();
    DelayUs(2);
    SCL_LOW();
}
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void IIC_Send_Byte(u8 txd)
{
    u8 t;
    SDA_OUT();
    SCL_LOW(); //拉低时钟开始数据传输
    for (t = 0; t < 8; t++)
    {
        ((txd & 0x80) >> 7) == 0 ? SDA_LOW() : SDA_HIGH();
        txd <<= 1;
        DelayUs(2);   //对TEA5767这三个延时都是必须的
        SCL_HIGH();
        DelayUs(2);
        SCL_LOW();
        DelayUs(2);
    }
}

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
u8 IIC_Read_Byte(unsigned char ack)
{
    unsigned char i, receive = 0;
    SDA_IN(); //SDA设置为输入
    for (i = 0; i < 8; i++)
    {
        SCL_LOW();
        DelayUs(2);
        SCL_HIGH();
        receive <<= 1;
        if (READ_SDA()) receive++;
        DelayUs(1);
    }
    if (!ack)
        IIC_NAck(); //发送nACK
    else
        IIC_Ack(); //发送ACK
    return receive;
}

//IO方向设置
void SDA_IN() {
    GPIOB_ModeCfg(PIN_SDA, GPIO_ModeIN_PU);
}
void SDA_OUT() {
    GPIOB_ModeCfg(PIN_SDA, GPIO_ModeOut_PP_5mA);
}

void SCL_HIGH(){
    GPIOB_SetBits(PIN_SCL);
}
void SCL_LOW(){
    GPIOB_ResetBits(PIN_SCL);
}

void SDA_HIGH(){
    GPIOB_SetBits(PIN_SDA);
}
void SDA_LOW(){
    GPIOB_ResetBits(PIN_SDA);
}

u8 READ_SDA()
{
    return GPIOB_ReadPortPin(PIN_SDA) ? 1 : 0;
}
