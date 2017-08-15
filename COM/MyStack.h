/****************************************file start****************************************************/
#ifndef	MYSTACK_H_H
#define	MYSTACK_H_H

#include	"Define.h"
#include	"Intent.h"

//����һ��ҳ����Ϣ

typedef struct Activity_Tag
{
	char * pageName;																	//activity���֣�ָ��һ��Ψһ��ַ
	MyRes (* pageCreate)(void * thizactivity, Intent * pram);					//����ÿ��ҳ�����ĺ���������ҳ���ڴ�
	void (* pageStart)(void);															//��ʾҳ�棬��ʼ��ҳ������
	void (* pageInput)(unsigned char *pbuf , unsigned short len);						//��ȡҳ������
	void (* pageFresh)(void);															//ҳ��ˢ��
	void (* pageHide)(void);															//ҳ�����أ��л�����ҳ��
	void (* pageResume)(void);															//������״̬���أ�������ʾ
	void (* pageDestroy)(void);															//ҳ�����٣���ʾ��һ��ҳ��
	Intent * pram;																		//��һ��ҳ�洫�����Ĳ�����ַ����ȡ����Ҫ�ͷ�
	MyRes (* childPageCreate)(void * thizactivity, Intent * pram);			//��ҳ�洴������ָ��
}Activity;


typedef struct _stacknode_
{
    Activity * activity;									//��ǰ�ڵ��ҳ����Ϣ
    struct _stacknode_ * lastStackNode;						//��һ�ڵ��ַ
}StackNode;


typedef struct
{
	StackNode * top;										//ջ���ڵ�
}LinkStack;


#define ActivityStructSize	sizeof(Activity)

void InitLinkStack(LinkStack * linkStack);
MyRes StackPush(LinkStack * linkStack, Activity * nodeData);
MyRes StackPop(LinkStack * linkStack, bool isResume);
MyRes StackTop(LinkStack * linkStack, Activity ** activity);

#endif

/****************************************end of file************************************************/
