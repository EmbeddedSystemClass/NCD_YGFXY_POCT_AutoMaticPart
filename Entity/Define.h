#ifndef _DEFINE_H_H
#define _DEFINE_H_H

#include	"stm32f4xx.h"
#include 	"FreeRTOS.h"
#include	"ff.h"

/*�����и������ܵ�����*/
#define		AdminPassWord				"201300\0"								//����Ա���룬�����޸��豸id
#define		AdjustPassWord				"201301\0"								//У׼����
#define		TestPassWord				"201302\0"								//�ϻ���������
#define		CheckQRPassWord				"201303\0"								//���Զ�ά������
#define		AdjLedPassWord				"201304\0"								//У׼led����
#define		FactoryResetPassWord		"201305\0"								//�ָ�������������
#define		ChangeValueShowTypePassWord	"201306\0"								//�л������ʾģʽ���Ƿ���ʾ��ʵֵ
#define		UnlockLCDPassWord			"201307\0"								//������Ļһ��

/*V1.0.03*/
#define	GB_SoftVersion	(unsigned short)500
#define	GB_SoftVersion_Build	"Build17050101\0"

/*��������Ϣ*/
#define	NCD_ServerIp_1		116
#define	NCD_ServerIp_2		62
#define	NCD_ServerIp_3		108
#define	NCD_ServerIp_4		201
#define NCD_ServerPort		8080

#define	HttpResponeOK				"HTTP/1.1 200 OK\0"

//poct
#define	ReadTimeUrl					"DeviceReadTime\0"
#define	QueryDeviceByDeviceIdUrl	"DeviceQueryDeviceByDeviceId\0"
#define	UpLoadDeviceAdjustUrl		"UpLoadDeviceAdjust\0"
#define	UpLoadDeviceErrorUrl		"UpLoadDeviceError\0"
#define	UpLoadYGFXYDataUrl			"upLoadYGFXY_Data\0"
#define	UpLoadDeviceMaintenanceUrl	"UpLoadDeviceMaintenance\0"
#define	UpLoadDeviceQualityUrl		"UpLoadDeviceQuality\0"

//ncd server
#define	NcdUpLoadYGFXYDataUrl		"UpLoadYGFXY\0"
#define	QueryRemoteSoftVersionUrl	"QuerySoftInfo\0"
#define	DownRemoteSoftFileUrl		"DownloadSoftFile\0"

/*SD���ļ���*/
#define	TestDataFileName			"0:/TD.ncd\0"
#define	DeviceFileName				"0:/Device.ncd\0"
#define	DeviceAdjustFileName		"0:/Dadj.ncd\0"
#define	DeviceErrorFileName			"0:/Derr.ncd\0"
#define	DeviceMainenanceFileName	"0:/Dmai.ncd\0"
#define	DeviceQualityFileName		"0:/Dqua.ncd\0"


#define	_Use_AdjustLed_Fun	0
#define	_Use_Lwip_Fun	0
/**********************************************************************************************************/
/******************************************�����������*************************************************/
/**********************************************************************************************************/
typedef enum
{ 
	My_Pass = pdPASS,			//�����ɹ�
	My_Fail = pdFAIL			//����ʧ��
}MyRes;

typedef enum
{ 
	true = 1,
	false = 0
}bool;

typedef enum
{ 
	ON = 1,
	OFF = 0
}MyBitAction;

/**********************************************************************************************************/
/******************************************fatfs�ṹ�嶨��*************************************************/
/**********************************************************************************************************/
/*�˽ṹ�������ڽ���sd�����������ڴ�*/
typedef struct
{
	FIL file;
	UINT br;
	UINT bw;
	FRESULT res;
	FSIZE_t size;
}FatfsFileInfo_Def; 

#define	MyFileStructSize sizeof(FatfsFileInfo_Def)
/**********************************************************************************************************/
/**********************************************************************************************************/

#define	PaiDuiWeiNum	8							//�Ŷ�λ����Ŀ
typedef enum
{
	statusNull = 0,									//û��ʼ,Ĭ��״̬
	status_user = 1,								//ѡ�������״̬
	status_sample = 2,								//��ȡ��ƷID״̬
	status_wait1 = 3,								//�ȴ��忨1
	status_preread = 4,								//Ԥ����״̬
	status_start = 5,								//�����Ŷ�
	status_outcard = 6,								//�ȴ��ӿ��۰γ�
	status_in_n = 7,								//����ʱ״̬ -- �ȴ������Ŷ�λ
	status_out_n = 8,								//����ʱ״̬ -- �ȴ��γ��Ŷ�λ
	status_incard_n = 9,							//����ʱ״̬ -- �ȴ����뿨��
	status_timedown = 10,							//����ʱ״̬
	status_prereadagain_n = 11,						//����ʱ״̬ -- �ٴ�Ԥ��������Ҫ��⿨���
	status_in_o = 12,								//��ʱ״̬ -- �ȴ������Ŷ�λ
	status_out_o = 13,								//��ʱ״̬ -- �ȴ��γ��Ŷ�λ
	status_incard_o = 14,							//��ʱ״̬ -- �ȴ����뿨��
	status_timeup = 15,								//��ʱ״̬
	status_prereadagain_o = 16,						//��ʱ״̬ -- �ٴ�Ԥ��������Ҫ��⿨���
	status_timedownagain = 17,						//����ʱ����
	status_test = 18,								//����
}MyPaiDuiStatues;

/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************��ά����ض���**************************************************/
/**********************************************************************************************************/


typedef enum
{
	CardCodeScanning = 0,											//����ɨ��
	CardCodeScanOK = 1,												//ɨ���ά��ɹ�
	CardCodeScanFail = 2,											//ɨ��ʧ��
	CardCodeTimeOut = 3,											//����
	CardCodeCardOut = 4,											//�����γ�
	CardCodeScanTimeOut = 5,										//ɨ�賬ʱ
	CardCodeCRCError = 6,											//crc����
	CardUnsupported = 7												//��ǰ����֧��
}ScanCodeResult;





#define	MaxLocation			6500						//����г�
#define	StartTestLocation	1250							//������ʼλ��
#define	EndTestLocation		4300						//���Խ���Ϊֹ
#define	WaittingCardLocation	MaxLocation					//�ȴ��忨��λ��
#define	AvregeNum		10								//ƽ��ֵ�˲�����
#define	FilterNum		5								//ƽ���˲�����
#define	MaxPointLen		((EndTestLocation - StartTestLocation)/AvregeNum - FilterNum)	//���Ե����


/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************��ƷID��ض���**************************************************/
/**********************************************************************************************************/
#define	MaxSampleIDLen	20											//���֧����Ʒid����Ϊ20
/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************������ض���****************************************************/
/***************************************************************************�*******************************/

typedef struct LineInfo_Tag
{
	unsigned short MaxData;				//ʵʱ����������ֵ
	unsigned short Y_Scale;				//����y����͵Ŀ̶�
	unsigned short MUL_Y;				//Y��Ŵ���
}LineInfo;

/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************��Ļ��ض���****************************************************/
/**********************************************************************************************************/
#define	LCD_Head_1		0xAA											//Э��ͷ1
#define	LCD_Head_2		0xBB											//Э��ͷ2

#define W_REGSITER 		0x80          									//д������,��Ӧ��          
#define R_REGSITER 		0x81          									//��ȡ���ݣ���Ӧ��
#define W_ADDRESS  		0x82											//д�����Ĵ�������Ӧ��
#define R_ADDRESS  		0x83											//�������Ĵ������ݣ���Ӧ��
#define W_LINE     		0x84          									//д���߻���������Ӧ��

#pragma pack(1)
typedef struct BasicICO_Tag
{
	unsigned short X;
	unsigned short Y;
	unsigned short ICO_ID;
}Basic_ICO;
#pragma pack()
/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************�Լ칦����ض���****************************************************/
/**********************************************************************************************************/



typedef enum
{
	SelfCheck_None = 0,													//�Լ�δ��ʼ
	SelfChecking = 1,
	SelfCheck_OK = 2,													//�Լ�����
	SelfCheck_Error = 3,												//�Լ��в��ɺ��ԵĴ���
}SelfCheckStatus;
/**********************************************************************************************************/
/**********************************************************************************************************/
typedef enum
{
	DESC = 0,												//����
	ASC = 1													//����
}OrderByEnum;

//��ȡ������Ϣ
#pragma pack(1)
typedef struct PageRequest_tag {
	unsigned int startElementIndex;											//��ʼ��ȡ������0Ϊ��һ��
	unsigned int pageSize;													//ÿҳ����Ŀ
	OrderByEnum orderType;													//����ʽ
	unsigned short crc;
}PageRequest;
#pragma pack()

#define	PageRequestStructSize		sizeof(PageRequest)
#define	PageRequestStructCrcSize	PageRequestStructSize - 2

#pragma pack(1)
typedef struct
{
	unsigned int itemSize;
	unsigned int userUpLoadIndex;
	unsigned int ncdUpLoadIndex;
	unsigned short crc;
}DeviceRecordHeader;
#pragma pack()

#define	DeviceRecordHeaderStructSize		sizeof(DeviceRecordHeader)								//��ౣ����û���Ŀ
#define	DeviceRecordHeaderStructCrcSize		DeviceRecordHeaderStructSize - 2						//��ౣ����û���Ŀ
/**********************************************************************************************************/
/******************************************������ض���****************************************************/
/**********************************************************************************************************/


#define	MaxSaveWifiNum	50												//��ౣ��50���ȵ�
#define	MaxWifiListNum	20												//�����ʾ20���ȵ�
#define	PageWifiNum		8
#define	MaxSSIDLen		50
#define	MaxKEYLen		20

/*wifi�ȵ�*/
#pragma pack(1)
typedef struct wifi_Tag
{
	char ssid[MaxSSIDLen];														//SSID
	char auth[20];														//��֤ģʽ
	char encry[10];														//���ܷ�ʽ
	unsigned char indicator;											//�ź�ǿ��
	char key[MaxKEYLen];														//����
	unsigned short crc;
}WIFI_Def;
#pragma pack()


typedef struct mynetbuf_tag
{
	void * data;
	unsigned short datalen;
}mynetbuf;

typedef enum
{ 
	LinkDown = 0,
	LinkUp = 1
}LinkStatus;



/*ip��ȡ��ʽ*/
typedef enum
{
	Dynamic_IP = 1,														//��̬IP
	Static_IP = 2														//��̬IP
}IPMode;







/**********************************************************************************************************/
/**********************************************************************************************************/

#define	MaxTestDataSaveNum	(unsigned int)1000000

/*********************************************************************************************/
/*********************************************************************************************/
/*********************************************************************************************/
/**************************У׼����***********************************************************/
/*********************************************************************************************/
typedef enum
{
	NoResult = 0,										//��δ���ԣ����δnull
	NoSample = 1,										//δ����
	PeakError = 2,										//��⿨û��C��
	MemError = 3,										//�ڴ����
	TestInterrupt = 8,									//�����ж�
	ResultIsOK = 99,									//���Գɹ�
}ResultState;

typedef enum
{
	Connect_Ok = 1,
	Connect_Error = 0
}PaiduiModuleStatus;

typedef enum
{ 
	NoCard = 0,								//�޿�
	CardIN = 1								//�п�
}CardState_Def;


#endif

