/****************************************file start****************************************************/
#ifndef _ITEMCONSTDATA_E_H
#define	_ITEMCONSTDATA_E_H


#define	ItemConstDataNum				10
#define	ItemNameLen						20
#define	ItemChNameLen					50
#define ItemUnitLen						20
#define	ItemNormalValueLen				50

#pragma pack(1)
typedef struct ItemConstData_Tag
{
	char itemName[ItemNameLen];									//��Ŀ����
	char itemCH[ItemChNameLen];									//��Ŀȫ��
	unsigned char pointNum;										//С�������
	float lowstResult;											//��ͼ����
	float highestResult;										//��߼����
	char normalResult[ItemNormalValueLen];						//�ο�ֵ
	char itemMeasure[ItemUnitLen];								//��λ
}ItemConstData;
#pragma pack()


#endif

/****************************************end of file************************************************/
