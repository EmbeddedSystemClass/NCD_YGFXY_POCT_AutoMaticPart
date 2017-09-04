#ifndef __MAINTENANCERECORD_P_H__
#define __MAINTENANCERECORD_P_H__

#include	"DeviceMaintenance.h"
#include	"UI_Data.h"


typedef struct MaintenanceRecordPageBuffer_tag {
	PageRequest pageRequest;							//�������
	DeviceRecordHeader deviceRecordHeader;				//��������ͷ��Ϣ
	Page page;											//��ȡ��������Ϣ
	DeviceMaintenance deviceMaintenance[DeviceMaintenanceRecordPageShowNum];		//��ȡ���Ĳ�������
	
	unsigned int i;
	char buf[100];										//��ʱ������
	unsigned int tempvalue1;
	DeviceMaintenance *tempDeviceMaintenance;
	unsigned short lcdinput[100];
}MaintenanceRecordPageBuffer;

MyRes createMaintenanceRecordActivity(Activity * thizActivity, Intent * pram);

#endif

