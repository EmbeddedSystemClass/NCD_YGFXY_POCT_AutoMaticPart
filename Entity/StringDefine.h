#ifndef _STRINGDEFINE_H_H
#define _STRINGDEFINE_H_H

#include	"Define.h"

#define		DeviceTypeString						"YGFXY_2\0"								//���Զ��豸
#define		DefaultDeviceId							DeviceTypeString

#define		fileLengthParmString					"Content-Length: \0"
#define		fileLengthParmStringLen					16
#define		fileStartStr							"i am zhangxiong^*^!\0"
#define		fileStartStrLen							19


#if(DeviceLanguage == DEVICE_CN)
	#define	DeviceLanguageString	"CH"
#elif(DeviceLanguage == DEVICE_EN)
	#define	DeviceLanguageString	"EN"
#endif

#define		AdminPassWord							"201300\0"								//����Ա���룬�����޸��豸id
#define		AdjustPassWord							"201301\0"								//У׼����
#define		TestPassWord							"201302\0"								//�ϻ���������
#define		CheckQRPassWord							"201303\0"								//���Զ�ά������
#define		AdjLedPassWord							"201304\0"								//У׼led���롤����������������������������������
#define		FactoryResetPassWord					"201305\0"							//�ָ�������������
#define		ChangeValueShowTypePassWord				"201306\0"						//�л������ʾģʽ���Ƿ���ʾ��ʵֵ
#define		UnlockLCDPassWord						"201307\0"								//������Ļһ��

/*SD���ļ���*/
#define	TestDataFileName							"0:/TD.ncd\0"
#define	DeviceFileName								"0:/Device.ncd\0"
#define	OperatorFileName							"0:/Oper.ncd\0"
#define	DeviceAdjustFileName						"0:/Dadj.ncd\0"
#define	DeviceErrorFileName							"0:/Derr.ncd\0"
#define	DeviceMainenanceFileName					"0:/Dmai.ncd\0"
#define	DeviceQualityFileName						"0:/Dqua.ncd\0"

#if (NCDServerType == NCDServerFinal)
#define		NcdServerReadTimeUrlStr				"/NCD_Server/up_dtime\0"
#define		NcdServerUpDeviceUrlStr				"/NCD_Server/up_device\0"
#define		NcdServerUpTestDataUrlStr			"/NCD_Server/UpLoadYGFXY\0"
#define		NcdServerQuerySoftUrlStr			"/NCD_Server/deviceSoftInfo\0"
#define		NcdServerDownSoftUrlStr				"/NCD_Server/DownloadSoftFile\0"
#elif (NCDServerType == NCDServerDebug)
#define		NcdServerReadTimeUrlStr				"/NCD_ServerDebug/up_dtime\0"
#define		NcdServerUpDeviceUrlStr				"/NCD_ServerDebug/up_device\0"
#define		NcdServerUpTestDataUrlStr			"/NCD_ServerDebug/UpLoadYGFXY\0"
#define		NcdServerQuerySoftUrlStr			"/NCD_ServerDebug/deviceSoftInfo\0"
#define		NcdServerDownSoftUrlStr				"/NCD_ServerDebug/DownloadSoftFile\0"
#endif

#if(DeviceLanguage == DEVICE_CN)
	#define	DeviceNameStr 							"ӫ�����߶���������\0"
	#define	CompanyNameStr							"�人Ŧ��������Ƽ��ɷ����޹�˾\0"
	#define	TesterNameStr							"������\0"
	#define	SampleIdStr								"��Ʒ���\0"
	#define	ItemNameStr								"������Ŀ\0"
	#define	ResultStr								"���Խ��\0"
	#define	ReferenceValueStr						"�ο�ֵ\0"
	#define	TestTimeStr								"����ʱ��\0"
	#define	PrintTimeStr							"��ӡʱ��\0"
	#define	StatementStr							"����: ��������Ա��걾����!\0"
	
	#define	QualityStatusPreparingString			"׼���У����Ե�\0"
	#define	QualityStatusWaitCardString				"�ȴ��忨\0"
	#define	QualityStatusTestString					"������\0"
	#define	QualityStatusWaitCardOutString			"�ȴ��ο�\0"
	#define	QualityTestingString					"������\0"
	#define	QualityClearString						"\0"
	#define	QualityTimeDownString					"����ʱ\0"
	#define	QualityTimeUpString						"��ʱ\0"
	#define	QualityPrepareCancelString				"�ȴ�ȡ��\0"
	#define	QualityCancellingString					"����ȡ��\0"
	#define	QualityQRScanningString					"���ڶ�ȡ��ά��\0"
	#define	QualityOverString						"���Խ���\0"
	
	
	#define	QRReadErrorAndChangeCardString			"��ά���ȡʧ�ܣ�����\0"
	#define	QRReadSuccessString						"��ά���ȡ�ɹ�\0"
	#define	QRItemChangedString						"��Ŀ����������\0"
	
	#define	TestErrorString							"���Դ���\0"
	#define	TestOkString							"���Գɹ�\0"
	
	
#elif(DeviceLanguage == DEVICE_EN)
	#define	DeviceNameStr 							"Fluor Immun Quant Analyzer\0"
	#define	CompanyNameStr							"Wuhan Newcando Biotechnology Co.,Ltd.\0"
	#define	TesterNameStr							"Tester\0"
	#define	SampleIdStr								"SampleId\0"
	#define	ItemNameStr								"Item\0"
	#define	ResultStr								"Result\0"
	#define	ReferenceValueStr						"Reference\0"
	#define	TestTimeStr								"Test Time\0"
	#define	PrintTimeStr							"Print Time\0"
	#define	StatementStr							"Statement: This result is only responsible for this specimen!\0"
	
	#define	QualityStatusPreparingString			"Preparing...\0"
	#define	QualityStatusWaitCardString				"Put Card In\0"
	#define	QualityStatusTestString					"Testing\0"
	#define	QualityStatusWaitCardOutString			"Put Card Out\0"
	#define	QualityTestingString					"Testing\0"
	#define	QualityClearString						"\0"
	#define	QualityTimeDownString					"Count Down\0"
	#define	QualityTimeUpString						"Count Up\0"
	#define	QualityPrepareCancelString				"Wait Cancel\0"
	#define	QualityCancellingString					"Cancelling\0"
	#define	QualityQRScanningString					"Scanning QR\0"
	#define	QualityOverString						"Test Over\0"
	
	#define	QRReadErrorAndChangeCardString			"QR Read Fail, please Change\0"
	#define	QRReadSuccessString						"QR Read Success\0"
	#define	QRItemChangedString						"Item Changed, please Change\0"
	
	#define	TestErrorString							"Test Error\0"
	#define	TestOkString							"Test OK\0"
#endif

#endif

