/*******************************************************************************************

*Department:	Exchange Team

*Decrible:  	简单数据内存池
				
*Auhor:			Savin

*Createdtime:	2018-02-02

*ModifyTime:	


********************************************************************************************/


#ifndef DATAPOOL_HEAD_H
#define DATAPOOL_HEAD_H


#include <pthread.h>
#include <list>
#include <stdlib.h>
#include <string.h>
#include "Public.h"


//#include "quickfix/STQuoteRequest44.h"



using namespace std;




class CMemPoolMgr
{
public:
	CMemPoolMgr();
	~CMemPoolMgr();
	static CMemPoolMgr *	GetInstance();
	int 					Init();
	void 					SetInstance() ;
	STOrderMENode *			GetDataNode();
	void 					PushDataNode(STOrderMENode *pNode);

	
	
private:


	list<STOrderMENode *>m_DataMemPool;
	pthread_mutex_t			m_DataLock;

	
	static CMemPoolMgr *	m_pInstance;

};



#endif // DATAPOOL_HEAD_H




