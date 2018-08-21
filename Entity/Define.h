#ifndef _DEFINE_H_H
#define _DEFINE_H_H

#include	"stm32f4xx.h"
#include 	"FreeRTOS.h"
#include	"ff.h"

/**********************************************************************************************************/
/**********************************************************************************************************/
/**********************************************************************************************************/
//build by device id
#define	DeviceAll						0x30
#define	Device_NCD13021806109			0x31
#define	Device_NCD13021801104			0x32
#define	Device_NCD13021801105			0x33
#define	Device_NCD13021801106			0x34
#define	DeviceBuildId					DeviceAll	

#if (DeviceBuildId == DeviceAll)

	#define	GB_SoftVersion	(unsigned short)1016
	#define	GB_SoftVersionStr	"V1.0.16\0"
	#define	GB_SoftVersion_Build	"Build18081501\0"
	
#elif (DeviceBuildId == Device_NCD13021806109)

	#define	GB_SoftVersion	(unsigned short)1016
	#define	GB_SoftVersionStr	"V1.0.16\0"
	#define	GB_SoftVersion_Build	"Build18070501\0"

#elif (DeviceBuildId == Device_NCD13021801104)
	
	#define	GB_SoftVersion	(unsigned short)1015
	#define	GB_SoftVersionStr	"V1.0.15\0"
	#define	GB_SoftVersion_Build	"Build18070501\0"
	
#elif (DeviceBuildId == Device_NCD13021801105)
	
	#define	GB_SoftVersion	(unsigned short)1015
	#define	GB_SoftVersionStr	"V1.0.15\0"
	#define	GB_SoftVersion_Build	"Build18070501\0"
	
#elif (DeviceBuildId == Device_NCD13021801106)
	
	#define	GB_SoftVersion	(unsigned short)1016
	#define	GB_SoftVersionStr	"V1.0.16\0"
	#define	GB_SoftVersion_Build	"Build18070501\0"
	
#endif

//��ά�붨��
#define QRVersion0Define		0								//��ά��ԭʼ�汾����
#define QRVersion2Define		'2'								//��ά��汾2����
#define QRVersion3Define		'3'								//��ά��汾3����
#define QRVersionUnDefine		0xff							//��֧�ֶ�ά��汾
#define	GB_QRVersion			QRVersion3Define				//��ǰ֧�ֵĶ�ά��汾�����¼���

/*��������Ϣ*/
#define	NCD_ServerIp_1		116
#define	NCD_ServerIp_2		62
#define	NCD_ServerIp_3		108
#define	NCD_ServerIp_4		201
#define NCD_ServerPort		8080


#define	Motor4IOMotor					0x90							//IO�ڿ��Ƶ�צ��
#define	Motor4UsartMotor				0x91							//���ڿ��Ƶ�צ��
#define	Motor4Type						Motor4UsartMotor

#define	DEVICE_EN						100
#define	DEVICE_CN						101
#define	DeviceLanguage					DEVICE_CN

#define	UserNormalProgram				0x60							//ͨ�ó���,�ϴ���ncd�Լ������
#define	UserXGProgram					0x61							//Т������ҽԺ����Т��LIS
#define	UserProgramType					UserNormalProgram				//�����û�ѡ����

#define	Device_Final					0x27                            //��ʽ����
#define	Device_Demo						0x28                            //��ʾ����
#define	Device_FastDemo					0x29                            //�������ʾ����
#define Device_TestCenter               0x2A                            //�������ĳ���
#define	DeviceUseType					Device_Final	

/**********************************************************************************************************/
/**********************************************************************************************************/
/**********************************************************************************************************/


#define	PaiDuiWeiNum					9							//�Ŷ�λ����Ŀ
#define	MaxQualityCount					PaiDuiWeiNum

#define	MaxUpLoadTestDataNum			1							//��������һ���ϴ���Ŀ��must less than PageContentItemNum
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
	Bool_Null = 0,
	Bool_True = 1,
	Bool_False = 2
}MyBool;

typedef enum
{ 
	ON = 1,
	OFF = 0
}MyBitAction;

#define SystemTestting                 				((uint16_t)0x0001)
#define SystemDeviceInfoModifying                 	((uint16_t)0x0002)
#define SystemOperatorModifying                 	((uint16_t)0x0004)
#define SystemNetInfoModifying                 		((uint16_t)0x0008)
#define SystemLookRecord                 			((uint16_t)0x0010)
#define SystemQualityong                			((uint16_t)0x0020)
#define SystemMaintenancing                 		((uint16_t)0x0040)
#define SystemSetting                 				((uint16_t)0x0080)


#define	FreeRTOSZeroDelay	0				//0ms
#define	FreeRTOSTenDelay	10				//10ms
#define	FreeRTOSDelay_50	50				//10ms
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
	unsigned int tempValue1;
	unsigned short i;
	unsigned char *tempPoint;
}FatfsFileInfo_Def; 

#define	MyFileStructSize sizeof(FatfsFileInfo_Def)
/**********************************************************************************************************/
/**********************************************************************************************************/
typedef enum
{
	statusNull = 0,									//û��ʼ,Ĭ��״̬
	statusNone = 19,								//û��ʼ,Ĭ��״̬
	statusMotorMoveWaitCard = 1,					//����������е��ȴ��忨��
	statusWaitCardPutIn = 2,						//�ȴ��忨
	statusWaitCardPutOut = 4,						//�ȴ��忨
	statusWaitScanQR = 3,							//�ȴ�ɨ���ά����
	statusMotorPutCardDown = 6,						//��ȡ��ά��ɹ��󣬵ȴ�����˶����������Ŷ�λ
	statusMotorOrigin = 7,							//�ȴ������ԭ��
	status_start = 5,								//�����Ŷ�
	status_timedown = 10,							//����ʱ״̬
	status_timeup = 15,								//��ʱ״̬
	status_testting = 16,							//��ʼ����
	statusPrepareTest = 18,							//�������״̬������ʱʱ��С��10s���л�������ʱ����ȴ�
	statusTestMotor = 20,							//�ȴ�����ߵ�����λ��
	statusPutCardOut = 21,							//�����ų��豸
	statusWaitCardOut = 22,							//�ȴ����ų��豸
	statusWaitPutCardInTestPlace = 23,				//�ȴ����ڲ��Զ˷ź�
	statusInputSampleID = 24,						//����������ƷID
	status_end = 17,
}MyPaiDuiStatues;

#define	StartTestTime								10					//��ֹ�������Ե���Сʱ��
#define	DisableCreateNewTestTime					30					//��ֹ�������Ե���Сʱ��
#define	DisableReturnTime							40					//��ֹ���أ���������ʱ��
/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************��ά����ض���**************************************************/
/**********************************************************************************************************/


typedef enum
{
	CardScanNone = 0,
	
	CardCodeScanOK = 1,												//ɨ���ά��ɹ�
	CardCodeScanFail = 2,											//ɨ��ʧ��
	CardCodeTimeOut = 3,											//����
	CardCodeCardOut = 4,											//�����γ�
	CardCodeScanTimeOut = 5,										//ɨ�賬ʱ
	CardCodeCRCError = 6,											//crc����
	CardUnsupported = 7,											//��ǰ����֧��
	CardCodeScanning = 8,											//����ɨ��
}ScanCodeResult;


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

#define	BasicIcoStructSize		sizeof(Basic_ICO)
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
	unsigned int pageIndex;											//��ʼ��ȡ������0Ϊ��һ��
	unsigned char pageSize;													//ÿҳ����Ŀ
	OrderByEnum orderType;													//����ʽ
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
	unsigned char parm[20];								//not used
	unsigned short crc;
}DeviceRecordHeader;
#pragma pack()

#define	DeviceRecordHeaderStructSize		sizeof(DeviceRecordHeader)								//��ౣ����û���Ŀ
#define	DeviceRecordHeaderStructCrcSize		DeviceRecordHeaderStructSize - 2						//��ౣ����û���Ŀ
#define	PageContentItemNum					10														//һ������ȡ����Ŀ
#pragma pack(1)
typedef struct
{
	unsigned int totalPageSize;																		//��ҳ��
	unsigned int totalItemSize;																		//����
	unsigned int readItemSize;																		//��������Ŀ
	bool isForNCD;																					//true��ȡŦ�������ݣ�false��ȡ�û�����
	void * content[PageContentItemNum];																//����
}Page;
#pragma pack()

#define	PageStructSize		sizeof(Page)

/**********************************************************************************************************/
/**********************************************************************************************************/
typedef enum
{
	NoResult = 0,										//��δ���ԣ����δnull
	NoSample = 1,										//δ����
	PeakError = 2,										//��⿨û��C��
	MemError = 3,										//�ڴ����
    T_CV_0_05 = 4,                                      //�Ҳ���T������Ĭ��λ�õ�cv>0.05
    C_CV_ERR_1 = 5,
    C_CV_ERR_2 = 6,
    C_L_T_L_1 = 7,                                        //cλ��С��Tλ��
    C_L_T_L_2 = 9,                                        //cλ�� - Tλ�� (50-100)
    C_CV_T_CV = 10,                                     //c_cv + t_cv < 0.13
    CANLIU = 11,                                        //����
    B_V_ERR = 12,                                       //����ֵ����
	TestInterrupt = 8,									//�����ж�
	qrError = 13,										//��ά�����
	ResultIsOK = 99,									//���Գɹ�
}ResultState;

#pragma pack(1)
typedef struct Point_tag {
	unsigned short y;
	unsigned short x;
}Point;
#pragma pack()

/**********************************************************************************************************/
/**********************************************************************************************************/
#pragma pack(1)
typedef struct TempDataBuffer_tag {
	unsigned char i;
	unsigned short j;
	unsigned int tempInt;
	double tempDouble;
	unsigned short shortData[1024];
	unsigned char * tempCharP;
	unsigned short * tempShortP;
}TempDataBuffer;
#pragma pack()
#define	TempDataBufferStructSize	sizeof(TempDataBuffer)


#pragma pack(1)
typedef struct PrintInfo_Tag
{
	char header[30];
	unsigned short crc;
}PrintInfo;
#pragma pack()

#endif

