
#include <endian.h>
#include <arpa/inet.h>

#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include "OrderAckSender.h"
#include "xlogger.h"
#include "Public.h"
#include "MemPoolMgr.h"
//#include "tnodeconfig_v0.h"
#include <list>
#include "CRealtimeResponseFromME.h"




using namespace std;



const int JSONLEN = 1024;


CRWBuffer COrderAckSender::m_RWBuffer;

COrderAckSender::COrderAckSender()
{
}



void COrderAckSender::Run() 
{

	STTnodeConfig tnodeconfig ;
	tnodeconfig.mq_vhost= stServiceConfig.mq_vhost_order_ack ;
	tnodeconfig.mq_exchange_group = stServiceConfig.mq_exchange_order_ack ;
	tnodeconfig.mq_host = stServiceConfig.mq_address_order_ack ;
	tnodeconfig.mq_port= atoi( (stServiceConfig.mq_port_order_ack).c_str() );
	tnodeconfig.mq_user = stServiceConfig.mq_user_order_ack ;
	tnodeconfig.mq_passwd= stServiceConfig.mq_password_order_ack;


	
	TNode tnode(tnodeconfig);
	if(-1 == tnode.Init() )
	{
		XERROR("tnode init fail , pls check ! \n");
		return ;
	}
	
	

	
	char sJsonBuf[JSONLEN];

	while(1)
	{
		
		
	
		STOrderMENode *pData  = m_RWBuffer.GetRealtimeData();
		if(NULL == pData)
		{
			usleep(3000);
			continue;
		}

		
		// 组装JSON
		memset(sJsonBuf,0,JSONLEN);
		snprintf(sJsonBuf,JSONLEN-1,"\"Tag\":%d,\"OrderID\":%s,\"ExcelID\":\"%s\",\"OrderIDFrME\":\"%s\",\"Text\":\"%s\",\"Time\":\"%d\"" ,
		pData->nTag,
		pData->nOrderID,
		pData->stCanelOrderRequest.sExcelID,
		pData->stCanelOrderRequest.sOrderIDFrME,
		pData->sText,
		time(NULL)
		);
		
		

		//  发送 
		tnode.PublishToMQ( stServiceConfig.mq_routingkey_order_ack.c_str(), sJsonBuf,strlen(sJsonBuf) ) ;

		XINFO("OrderME --> MQ :[%d][%s]\n",strlen(sJsonBuf),sJsonBuf);
		
	}

	//tnode.FreeDataBuff((char *)data_buffer);
	
}
   

void 	COrderAckSender::StartBatchSender()
{

	int i = 0;
    for(i = 0; i < stServiceConfig.RealtimeSenderCount; i++ )
    {
        COrderAckSender *pWorker = new COrderAckSender();
        pWorker->Start();
    }

	
	XINFO("sender %d module run ...\n",stServiceConfig.RealtimeSenderCount);

}


