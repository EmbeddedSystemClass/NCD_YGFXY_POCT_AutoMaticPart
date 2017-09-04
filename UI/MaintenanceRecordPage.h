#ifndef __MAINTENANCERECORD_P_H__
#define __MAINTENANCERECORD_P_H__

#include	"DeviceMaintenance.h"
#include	"UI_Data.h"


typedef struct MaintenanceRecordPageBuffer_tag {
	PageRequest pageRequest;							//请求参数
	DeviceRecordHeader deviceRecordHeader;				//测试数据头信息
	Page page;											//读取的数据信息
	DeviceMaintenance deviceMaintenance[DeviceMaintenanceRecordPageShowNum];		//读取到的测试数据
	
	unsigned int i;
	char buf[100];										//临时缓冲区
	unsigned int tempvalue1;
	DeviceMaintenance *tempDeviceMaintenance;
	unsigned short lcdinput[100];
}MaintenanceRecordPageBuffer;

MyRes createMaintenanceRecordActivity(Activity * thizActivity, Intent * pram);

#endif

