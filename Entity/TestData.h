/****************************************file start****************************************************/
#ifndef _TESTDATA_E_H
#define	_TESTDATA_E_H

#include	"Operator.h"
#include	"QRCode.h"
#include	"DateTime.h"
#include	"Define.h"
#include	"Temperature.h"

#define	TestDataRecordPageShowNum		8							//һҳ8������

#pragma pack(1)
typedef struct TestSeries_tag {
	unsigned short TestPoint[MaxPointLen];
	unsigned short C_Point[2];
	unsigned short T_Point[2];
	unsigned short B_Point[2];
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
