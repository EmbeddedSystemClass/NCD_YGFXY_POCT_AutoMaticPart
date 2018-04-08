/****************************************file start************************************************/
#ifndef	__TCP_F_H_H
#define	__TCP_F_H_H

#include	"Define.h"
#include	"HttpBuf.h"

#if (UserProgramType == UserNormalProgram)

MyRes CommunicateWithServerByLineNet(HttpBuf * httpBuffer);

#elif (UserProgramType == UserXGProgram)

MyRes CommunicateWithLisByLineNet(HttpBuf * httpBuffer);

#endif

#endif
/****************************************end of file***********************************************/
