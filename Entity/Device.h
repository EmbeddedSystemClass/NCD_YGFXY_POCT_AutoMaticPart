/****************************************file start****************************************************/
#ifndef _DEVICE_E_H
#define	_DEVICE_E_H

#include	"Operator.h"

#define	DeviceIdLen	20
#define	DeviceDepartmentLen	30
#define	DeviceAddrLen	50

#pragma pack(1)
typedef struct Device_Tag
{
	Operator operator;															//������
	Operator operators[MaxOperatorSize];										//������
	char department[DeviceDepartmentLen];										//�豸����
	char addr[DeviceAddrLen];													//�豸��ַ
	unsigned int modifyTimeStamp;												//��ǰ������豸��Ϣ��ʱ���������ȷ�����С��˵���������и��£����ȡ
	unsigned short crc;
}Device;
#pragma pack()

#define	DeviceStructSize		sizeof(Device)
#define	DeviceStructCrcSize		DeviceStructSize - 2
	
#endif

/****************************************end of file************************************************/

