#ifndef __MAINTENANCERECORD_P_H__
#define __MAINTENANCERECORD_P_H__

#include	"DeviceMaintenance.h"
#include	"UI_Data.h"


typedef struct MaintenanceRecordPageBuffer_tag {

	unsigned char selectindex;							//ѡ�е�����
	unsigned int pageindex;							//ҳ������
	unsigned int maxpagenum;
	
	DeviceMaintenanceReadPackge deviceMaintenanceReadPackge;		//��ȡ����
	
	char buf[100];										//��ʱ������
	char tempBuf[100];										//��ʱ������
	unsigned int tempvalue1;
	unsigned int tempvalue2;
	DeviceMaintenance *tempDeviceMaintenance;
	unsigned short lcdinput[100];
}MaintenanceRecordPageBuffer;

MyRes createMaintenanceRecordActivity(Activity * thizActivity, Intent * pram);

#endif

