/****************************************file start****************************************************/
#ifndef _TESTDATA_E_H
#define	_TESTDATA_E_H

#include	"Operator.h"
#include	"QRCode.h"
#include	"DateTime.h"
#include	"Define.h"
#include	"Temperature.h"
#include	"AdjustData.h"

#define	TestDataRecordPageShowNum		8							//һҳ8������
#define	AvregeNum		10								//ƽ��ֵ�˲�����
#define	MaxPointLen	300

#pragma pack(1)
typedef struct TestSeries_tag {
	unsigned short TestPoint[MaxPointLen];
	Point C_Point;
	Point T_Point;
	Point B_Point;
	float BasicBili;
	float BasicResult;
	float AdjustResult;
} TestSeries;
#pragma pack()

#pragma pack(1)
typedef struct TestData_tag {
	Operator 		operator;							//������
	char 			sampleid[MaxSampleIDLen];			//�������
	QRCode 			qrCode;								//��ά����Ϣ
	TestSeries 		testSeries;
	AdjustData 		adjustData;							//У׼����
	DateTime 		testDateTime;						//����ʱ��
	Temperature 	temperature;						//�����¶�
	unsigned short 	time;								//��ʱʱ��
	ResultState 	testResultDesc;						//���Խ��״̬
	unsigned short 	crc;
}TestData;
#pragma pack()


#pragma pack(1)
typedef struct
{
	PageRequest pageRequest;
	DeviceRecordHeader deviceRecordHeader;
	TestData testData[TestDataRecordPageShowNum];
	unsigned char readTotalNum;							//��ȡ������Ч������Ŀ
}TestDataRecordReadPackage;
#pragma pack()

#define	TestSeriesStructSize	sizeof(TestSeries)

#define	TestDataStructSize		sizeof(TestData)
#define	TestDataStructCrcSize	TestDataStructSize - 2

#define	TestDataRecordReadPackageStructSize		sizeof(TestDataRecordReadPackage)
#define	TestDataRecordReadPackageStructCrcSize	TestDataRecordReadPackageStructSize - 2
	
#endif

/****************************************end of file************************************************/
