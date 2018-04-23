/***************************************************************************************************
*FileName:SystemInit
*Description: ϵͳ����ģ���ʼ��
*Author : xsx
*Data: 2016��4��21��14:18:28
***************************************************************************************************/

/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/
#include	"SystemInit.h"

#include	"Led_Driver.h"
#include	"CardCheck_Driver.h"
#include 	"Usart1_Driver.h"
#include 	"Usart2_Driver.h"
#include 	"Usart3_Driver.h"
#include 	"Usart4_Driver.h"
#include 	"Usart5_Driver.h"
#include 	"Usart6_Driver.h"
#include	"Motor1_Driver.h"
#include	"Motor2_Driver.h"
#if (Motor4Type == Motor4IOMotor)
#include	"Motor4_Driver.h"
#endif
#include 	"Iwdg_Driver.h"
#include	"Timer3_Driver.h"
#include	"CodeScanner_Driver.h"
#include	"DA_Driver.h"
#include	"Ads8325_Driver.h"
#include	"Channel_Driver.h"
#include	"LedCheck_Driver.h"
#include	"RX8025_Driver.h"
#include	"Ceju_Driver.h"

#include 	"usbd_hid_core.h"
#include 	"usbd_usr.h"
#include 	"usbd_desc.h"
#include 	"usb_conf.h"  

#include	"Delay.h"
#include	"Define.h"
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
USB_OTG_CORE_HANDLE USB_OTG_dev;
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
extern void SystemInit(void);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName��MySystemInit
*Description��ϵͳ��ʼ��
*Input��None
*Output��None
*Author��xsx
*Data��2016��4��21��14:20:44
***************************************************************************************************/
void MySystemBSPInit(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	delay_ms(2000);
	
	Led_GPIOInit();						//ϵͳָʾ�Ƴ�ʼ��
	delay_ms(1);

	Usart1_Init();						//����2��ʼ��
	delay_ms(1);
	
	Usart2_Init();						//����3��ʼ��
	delay_ms(1);
	
	Usart3_Init();						//����2��ʼ��
	delay_ms(1);
	
	Usart4_Init();						//����4��ʼ��
	delay_ms(1);
	
	Usart5_Init();						//����4��ʼ��
	delay_ms(1);
	
	Usart6_Init();						//����6��ʼ��
	delay_ms(1);
	
	CardCheck_GPIO_Init();						//��λ���س�ʼ��
	delay_ms(1);
	
	Motor1_GPIO_Init();							//��� IO��ʼ��
	delay_ms(1);
	
	Motor2_GPIO_Init();							//��� IO��ʼ��
	delay_ms(1);

#if (Motor4Type == Motor4IOMotor)
	Motor4_GPIO_Init();
	delay_ms(1);
#endif

	CodeScanner_GPIO_Init();
	delay_ms(1);
	
	DA_GPIOInit();
	delay_ms(1);
	
	Channel_GPIOInit();
	delay_ms(1);
	
	ADS_IO_Init();
	delay_ms(1);
	
	LedCheck_GPIO_Init();
	delay_ms(1);
	
	RTC_BSPInit();
	delay_ms(1);
	
	CejuADC_Init();

	Timer3_Init();						//��ʱ����ʼ��
	delay_ms(1);

	USBD_Init(&USB_OTG_dev, USB_OTG_FS_CORE_ID, &USR_desc, &USBD_HID_cb, &USR_cb);

	IWDG_Init(3, 3000);					//���Ź���ʼ��,��ʱʱ��2S
	delay_ms(500);
}
