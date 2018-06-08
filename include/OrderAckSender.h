/*******************************************************************************************

*Department:	Exchange Team

*Decrible:  	ACK数据 发给ORDERSERVICE
				
*Auhor:			Savin

*Createdtime:	2018-02-02

*ModifyTime:	


********************************************************************************************/



#ifndef SENDORDERACK_HEAD_H
#define SENDORDERACK_HEAD_H

#include <pthread.h>
#include <list>
#include "BaseThread.h"
#include "tnode.h"
#include "MemPoolMgr.h"



using namespace std;
using namespace snetwork_xservice_tnode ;

class COrderAckSender   : public CBaseThread
{  
public:  
	COrderAckSender();
	
   	virtual void 							Run() ;  
   

	static void 							StartBatchSender();
	static CRWBuffer m_RWBuffer;
  



};




#endif //SENDORDERACK_HEAD_H

