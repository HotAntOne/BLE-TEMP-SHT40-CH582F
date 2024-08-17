/********************************** (C) COPYRIGHT *******************************
 * File Name          : heartrate.c
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2020/08/06
 * Description        : 心率计应用程序，初始化广播连接参数，然后广播，直至连接主机后，定时上传心率
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*********************************************************************
 * INCLUDES
 */

#include "CONFIG.h"
#include "battservice.h"
#include "devinfoservice.h"
#include "heartrate.h"
#include "heartrateservice.h"

#include "SHT40.h"
#include "key.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */
// Slow advertising interval in 625us units
#define DEFAULT_SLOW_ADV_INTERVAL            1600*10


/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
static uint8_t heartRate_TaskID; // Task ID for internal task/event processing

static gapRole_States_t gapProfileState = GAPROLE_INIT;


static uint8_t advertData[] = {
    // flags
    0x02,
    GAP_ADTYPE_FLAGS,
    GAP_ADTYPE_FLAGS_GENERAL | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,

    // service UUIDs
    0x03,
    GAP_ADTYPE_16BIT_MORE,
    LO_UINT16(HID_SERV_UUID),
    HI_UINT16(HID_SERV_UUID),

    0x0D, // length of this data
    GAP_ADTYPE_LOCAL_NAME_COMPLETE,
    '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'
};


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void heartRate_ProcessTMOSMsg(tmos_event_hdr_t *pMsg);
static void HeartRateGapStateCB(gapRole_States_t newState, gapRoleEvent_t *pEvent);


/*********************************************************************
 * PROFILE CALLBACKS
 */

// GAP Role Callbacks
static gapRolesCBs_t heartRatePeripheralCB = {
    HeartRateGapStateCB, // Profile State Change Callbacks
    NULL,                // When a valid RSSI is read from controller
    NULL};

// Bond Manager Callbacks
static gapBondCBs_t heartRateBondCB = {
    NULL, // Passcode callback
    NULL,  // Pairing state callback
    NULL  // oob callback
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

__HIGH_CODE
void float_to_str(float dat, uint8_t *str_dat)
{
    // 将float转换成字符串形式，例如±99.9
    // 输出的str_dat为5个字节，包含±99.9
    int32_t d = (int32_t)(dat*10);
    if(d<0)
    {
        str_dat[0] = '-';
        d = -d;
    }else {
        str_dat[0] = ' ';
    }

    str_dat[4] = '0' + d%10;
    d /=10;

    str_dat[3] = '.';

    str_dat[2] = '0' + d%10;
    d /=10;

    str_dat[1] = '0' + d%10;
    d /=10;
}

__HIGH_CODE
void start_read_sht40()
{
    readSHT40_step1();
}

__HIGH_CODE
void update_adv(void)
{
    // 读取数据并更新adv
    const u8 name_index = 9;

    float temp, humi;
//    readSHT40(&temp, &humi);
    readSHT40_step2(&temp, &humi);

    float_to_str(temp, advertData+name_index);
    advertData[name_index+5] = 'C';
    float_to_str(humi, advertData+name_index+6);
    advertData[name_index+11] = 'H';

//    memcpy(scanRspData+2, advertData+9, 12);

    GAP_UpdateAdvertisingData(0, TRUE, sizeof(advertData), advertData);

}

/*********************************************************************
 * @fn      HeartRate_Init
 *
 * @brief   Initialization function for the Heart Rate App Task.
 *          This is called during initialization and should contain
 *          any application specific initialization (ie. hardware
 *          initialization/setup, table initialization, power up
 *          notificaiton ... ).
 *
 * @param   task_id - the ID assigned by TMOS.  This ID should be
 *                    used to send messages and set timers.
 *
 * @return  none
 */
void HeartRate_Init()
{
    heartRate_TaskID = TMOS_ProcessEventRegister(HeartRate_ProcessEvent);

    // Setup the GAP Peripheral Role Profile
    {
        uint8_t initial_advertising_enable = TRUE;
        GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &initial_advertising_enable);

        GAP_SetParamValue(TGAP_DISC_ADV_INT_MIN, DEFAULT_SLOW_ADV_INTERVAL);
        GAP_SetParamValue(TGAP_DISC_ADV_INT_MAX, DEFAULT_SLOW_ADV_INTERVAL);

        GAPRole_SetParameter(GAPROLE_ADVERT_DATA, sizeof(advertData), advertData);
    }

    {
        uint32_t passkey = 7893; // passkey "000000"
        uint8_t  pairMode = GAPBOND_PAIRING_MODE_NO_PAIRING;
        GAPBondMgr_SetParameter(GAPBOND_PERI_DEFAULT_PASSCODE, sizeof(uint32_t), &passkey);
        GAPBondMgr_SetParameter(GAPBOND_PERI_PAIRING_MODE, sizeof(uint8_t), &pairMode);
    }

    // Setup a delayed profile startup
    tmos_set_event(heartRate_TaskID, START_DEVICE_EVT);

    // 初始化定时传感器
    tmos_start_task(heartRate_TaskID, SBP_UP_DEVICE_NAME_EVT, DEFAULT_SLOW_ADV_INTERVAL-70);
}

/*********************************************************************
 * @fn      HeartRate_ProcessEvent
 *
 * @brief   Heart Rate Application Task event processor.  This function
 *          is called to process all events for the task.  Events
 *          include timers, messages and any other user defined events.
 *
 * @param   task_id  - The TMOS assigned task ID.
 * @param   events - events to process.  This is a bit map and can
 *                   contain more than one event.
 *
 * @return  events not processed
 */
uint16_t HeartRate_ProcessEvent(uint8_t task_id, uint16_t events)
{
    if(events & SYS_EVENT_MSG)
    {
        uint8_t *pMsg;

        if((pMsg = tmos_msg_receive(heartRate_TaskID)) != NULL)
        {
            heartRate_ProcessTMOSMsg((tmos_event_hdr_t *)pMsg);

            // Release the TMOS message
            tmos_msg_deallocate(pMsg);
        }

        // return unprocessed events
        return (events ^ SYS_EVENT_MSG);
    }

    if(events & START_DEVICE_EVT)
    {
        // Start the Device
        GAPRole_PeripheralStartDevice(heartRate_TaskID, &heartRateBondCB, &heartRatePeripheralCB);

        return (events ^ START_DEVICE_EVT);
    }

    if(events & SBP_UP_DEVICE_NAME_EVT)
    {
        start_read_sht40();     // 开始采集数据
        tmos_start_task(heartRate_TaskID, SBP_UP2_DEVICE_NAME_EVT, 16);     // 10ms
        tmos_start_task(heartRate_TaskID, SBP_UP_DEVICE_NAME_EVT, 1600*10);

        return (events ^ SBP_UP_DEVICE_NAME_EVT);
    }

    if(events & SBP_UP2_DEVICE_NAME_EVT)
    {
        update_adv();   // 获取数据并更新
        return (events ^ SBP_UP2_DEVICE_NAME_EVT);
    }


    // Discard unknown events
    return 0;
}

/*********************************************************************
 * @fn      heartRate_ProcessTMOSMsg
 *
 * @brief   Process an incoming task message.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */
static void heartRate_ProcessTMOSMsg(tmos_event_hdr_t *pMsg)
{
    switch(pMsg->event)
    {
        default:

            break;
    }
}

void readv()
{
//    应该进一步设置成重启或者啥的，防止连接。安卓手机被禁止连接，但是苹果手机还是可以连接

    uint8_t  erase_all = TRUE;
    GAPBondMgr_SetParameter(GAPBOND_ERASE_ALLBONDS, sizeof(uint8_t), &erase_all);

    uint8_t initial_advertising_enable = TRUE;

    GAP_SetParamValue(TGAP_DISC_ADV_INT_MIN, DEFAULT_SLOW_ADV_INTERVAL);
    GAP_SetParamValue(TGAP_DISC_ADV_INT_MAX, DEFAULT_SLOW_ADV_INTERVAL);

    GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &initial_advertising_enable);
    GAPRole_SetParameter(GAPROLE_ADVERT_DATA, sizeof(advertData), advertData);

    uint32_t passkey = 7893; // passkey "000000"
    uint8_t  pairMode = GAPBOND_PAIRING_MODE_NO_PAIRING;
    uint8_t  mitm = FALSE;
    uint8_t  ioCap = GAPBOND_IO_CAP_NO_INPUT_NO_OUTPUT;
    uint8_t  bonding = FALSE;
    GAPBondMgr_SetParameter(GAPBOND_PERI_DEFAULT_PASSCODE, sizeof(uint32_t), &passkey);
    GAPBondMgr_SetParameter(GAPBOND_PERI_PAIRING_MODE, sizeof(uint8_t), &pairMode);
}


/*********************************************************************
 * @fn      HeartRateGapStateCB
 *
 * @brief   Notification from the profile of a state change.
 *
 * @param   newState - new state
 *
 * @return  none
 */
static void HeartRateGapStateCB(gapRole_States_t newState, gapRoleEvent_t *pEvent)
{
    // if connected
    if(newState == GAPROLE_CONNECTED)
    {
        readv();
    }
    // if disconnected
    else if(gapProfileState == GAPROLE_CONNECTED &&
            newState != GAPROLE_CONNECTED)
    {

        readv();
    }
    else if(gapProfileState == GAPROLE_ADVERTISING &&
            newState == GAPROLE_WAITING)
    {
        readv();

    }
    else if(newState == GAPROLE_STARTED)
    {

    }

    gapProfileState = newState;
}


/*********************************************************************
*********************************************************************/
