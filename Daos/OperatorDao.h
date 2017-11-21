/****************************************file start****************************************************/
#ifndef _OPERATORDAO_D_H
#define	_OPERATORDAO_D_H

#include	"Define.h"
#include	"Operator.h"

MyRes SaveAllOperatorToFile(Operator * operator);
MyRes ReadAllOperatorFromFile(Operator * operator, unsigned short * operatorValidNum);
MyRes deleteOperatorFile(void);

#endif

/****************************************end of file************************************************/
