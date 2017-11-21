#ifndef _STRINGDEFINE_H_H
#define _STRINGDEFINE_H_H

#include	"Define.h"

#define		DeviceTypeString						"YGFXY_2\0"								//半自动设备
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

#define		AdminPassWord							"201300\0"								//管理员密码，用于修改设备id
#define		AdjustPassWord							"201301\0"								//校准密码
#define		TestPassWord							"201302\0"								//老化测试密码
#define		CheckQRPassWord							"201303\0"								//测试二维码密码
#define		AdjLedPassWord							"201304\0"								//校准led密码··················
#define		FactoryResetPassWord					"201305\0"							//恢复出厂设置密码
#define		ChangeValueShowTypePassWord				"201306\0"						//切换结果显示模式，是否显示真实值
#define		UnlockLCDPassWord						"201307\0"								//解锁屏幕一次

/*SD卡文件名*/
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
	#define	DeviceNameStr 							"荧光免疫定量分析仪\0"
	#define	CompanyNameStr							"武汉纽康度生物科技股份有限公司\0"
	#define	TesterNameStr							"测试人\0"
	#define	SampleIdStr								"样品编号\0"
	#define	ItemNameStr								"测试项目\0"
	#define	ResultStr								"测试结果\0"
	#define	ReferenceValueStr						"参考值\0"
	#define	TestTimeStr								"测试时间\0"
	#define	PrintTimeStr							"打印时间\0"
	#define	StatementStr							"声明: 本结果仅对本标本负责!\0"
	
	#define	QualityStatusPreparingString			"准备中，请稍等\0"
	#define	QualityStatusWaitCardString				"等待插卡\0"
	#define	QualityStatusTestString					"测试中\0"
	#define	QualityStatusWaitCardOutString			"等待拔卡\0"
	#define	QualityTestingString					"测试中\0"
	#define	QualityClearString						"\0"
	#define	QualityTimeDownString					"倒计时\0"
	#define	QualityTimeUpString						"超时\0"
	#define	QualityPrepareCancelString				"等待取消\0"
	#define	QualityCancellingString					"正在取消\0"
	#define	QualityQRScanningString					"正在读取二维码\0"
	#define	QualityOverString						"测试结束\0"
	
	
	#define	QRReadErrorAndChangeCardString			"二维码读取失败，更换\0"
	#define	QRReadSuccessString						"二维码读取成功\0"
	#define	QRItemChangedString						"项目变更，请更换\0"
	
	#define	TestErrorString							"测试错误\0"
	#define	TestOkString							"测试成功\0"
	
	
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

