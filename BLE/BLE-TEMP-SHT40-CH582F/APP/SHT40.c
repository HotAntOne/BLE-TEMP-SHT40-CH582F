#include <SHT40.h>

// ����SHT40��I2C��ַ������
#define SHT40_ADDR 0x44
#define MEASURE_CMD 0xFD


// ��ȡSHT40��ʪ�ȵ�ʵ��
// https://item.szlcsc.com/3188621.html
#define SHT40_ADDR 0x44
#define SHT40_MEASURE_CMD 0xFD
#define SHT40_MEASURE_DELAY 10  // ���������ֲ�������ʱ��
void readSHT40(float *temperature, float *humidity) {

    const uint8_t cmd = SHT40_MEASURE_CMD;  // ��������
    uint8_t rx_bytes[6];
    uint32_t temp_raw, humid_raw;
    float temp_f, humid_f;

    IIC_Init();
    // ���Ͳ�������
    IIC_Start();
    IIC_Send_Byte(SHT40_ADDR << 1);  // ����д����豸��ַ + дλ��
    if (IIC_Wait_Ack()) {
        IIC_Stop();
        *temperature = 1.0;
        *humidity = 0.0;
        return;  // ���û���յ�Ӧ��ֹͣ
    }

    IIC_Send_Byte(cmd);  // ���Ͳ�������
    if (IIC_Wait_Ack()) {
        IIC_Stop();
        *temperature = 2.0;
        *humidity = 0.0;
        return;  // ���û���յ�Ӧ��ֹͣ
    }
    IIC_Stop();

    DelayMs(SHT40_MEASURE_DELAY); // �ȴ�����׼��

    // ��������I2C������Ϊ��ģʽ
    IIC_Start();
    IIC_Send_Byte((SHT40_ADDR << 1) | 1);  // �豸��ַ + ��λ
    if (IIC_Wait_Ack()) {
        IIC_Stop();
        *temperature = 3.0;
        *humidity = 0.0;
        return;  // ���û���յ�Ӧ��ֹͣ
    }

    for (int i = 0; i < 6; i++) {
        rx_bytes[i] = IIC_Read_Byte(i != 5);  // ��ȡ6���ֽ�
    }
    IIC_Stop();
    IIC_Deinit();

    // �����ȡ������
    temp_raw = ((uint32_t)rx_bytes[0] << 8) | rx_bytes[1];
    humid_raw = ((uint32_t)rx_bytes[3] << 8) | rx_bytes[4];

    temp_f = -45.0 + 175.0 * temp_raw / 65535.0;
    humid_f = -6.0 + 125.0 * humid_raw / 65535.0;

    // ����ʪ�ȷ�Χ��0-100%
    humid_f = (humid_f > 100.0f) ? 100.0f : (humid_f < 0.0f) ? 0.0f : humid_f;

    *temperature = temp_f;
    *humidity = humid_f;
}

void readSHT40_step1() {

    const uint8_t cmd = SHT40_MEASURE_CMD;  // ��������

    IIC_Init();
    // ���Ͳ�������
    IIC_Start();
    IIC_Send_Byte(SHT40_ADDR << 1);  // ����д����豸��ַ + дλ��
    if (IIC_Wait_Ack()) {
        IIC_Stop();
        return;  // ���û���յ�Ӧ��ֹͣ
    }

    IIC_Send_Byte(cmd);  // ���Ͳ�������
    if (IIC_Wait_Ack()) {
        IIC_Stop();
        return;  // ���û���յ�Ӧ��ֹͣ
    }
    IIC_Stop();

//    DelayMs(SHT40_MEASURE_DELAY); // �ȴ�����׼��
}

void readSHT40_step2(float *temperature, float *humidity) {

    uint8_t rx_bytes[6];
    uint32_t temp_raw, humid_raw;
    float temp_f, humid_f;

    // ��������I2C������Ϊ��ģʽ
    IIC_Start();
    IIC_Send_Byte((SHT40_ADDR << 1) | 1);  // �豸��ַ + ��λ
    if (IIC_Wait_Ack()) {
        IIC_Stop();
        *temperature = 3.0;
        *humidity = 0.0;
        return;  // ���û���յ�Ӧ��ֹͣ
    }

    for (int i = 0; i < 6; i++) {
        rx_bytes[i] = IIC_Read_Byte(i != 5);  // ��ȡ6���ֽ�
    }
    IIC_Stop();
    IIC_Deinit();

    // �����ȡ������
    temp_raw = ((uint32_t)rx_bytes[0] << 8) | rx_bytes[1];
    humid_raw = ((uint32_t)rx_bytes[3] << 8) | rx_bytes[4];

    temp_f = -45.0 + 175.0 * temp_raw / 65535.0;
    humid_f = -6.0 + 125.0 * humid_raw / 65535.0;

    // ����ʪ�ȷ�Χ��0-100%
    humid_f = (humid_f > 100.0f) ? 100.0f : (humid_f < 0.0f) ? 0.0f : humid_f;

    *temperature = temp_f;
    *humidity = humid_f;
}


//��ʼ��IIC
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

//����IIC��ʼ�ź�
void IIC_Start(void)
{
    SDA_OUT();     //sda�����
    SDA_HIGH();
    SCL_HIGH();
    DelayUs(4);
    SDA_LOW(); //START: when CLK is high, DATA changes from high to low
    DelayUs(4);
    SCL_LOW(); //ǯסI2C���ߣ�׼�����ͻ��������
}
//����IICֹͣ�ź�
void IIC_Stop(void)
{
    SDA_OUT(); //sda�����
    SCL_LOW();
    SDA_LOW(); //STOP: when CLK is high DATA changes from low to high
    DelayUs(4);
    SCL_HIGH();
    SDA_HIGH(); //����I2C���߽����ź�
    DelayUs(4);
}

//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
    u8 ucErrTime = 0;
    SDA_IN();      //SDA����Ϊ����
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
    SCL_LOW(); //ʱ�����0
    return 0;
}

//����ACKӦ��
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

//������ACKӦ��
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
void IIC_Send_Byte(u8 txd)
{
    u8 t;
    SDA_OUT();
    SCL_LOW(); //����ʱ�ӿ�ʼ���ݴ���
    for (t = 0; t < 8; t++)
    {
        ((txd & 0x80) >> 7) == 0 ? SDA_LOW() : SDA_HIGH();
        txd <<= 1;
        DelayUs(2);   //��TEA5767��������ʱ���Ǳ����
        SCL_HIGH();
        DelayUs(2);
        SCL_LOW();
        DelayUs(2);
    }
}

//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
u8 IIC_Read_Byte(unsigned char ack)
{
    unsigned char i, receive = 0;
    SDA_IN(); //SDA����Ϊ����
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
        IIC_NAck(); //����nACK
    else
        IIC_Ack(); //����ACK
    return receive;
}

//IO��������
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
