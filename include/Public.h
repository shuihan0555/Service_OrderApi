/*******************************************************************************************

*Department:	TP

*Decrible:  	FIX SiganlEngine
				
				


*Auhor:			Savin

*Createdtime:	2017-06-07

*ModifyTime:	


********************************************************************************************/


#ifndef FIXSE_PUBLIC_HEAD_H
#define FIXSE_PUBLIC_HEAD_H

#include <pthread.h>
#include <assert.h>
#include <list>
#include <vector>
#include <map>
#include <string>
#include "xlogger.h"


#define SERVICECONFIG "OrderAPI.cfg"
using namespace std;

using namespace snetwork_xservice_xflagger;


typedef unsigned int uint32;
typedef unsigned short uint16;

#define NEWORDERREQUEST	0x3005 

#define CANCELORDERREQUEST 0x4002

#define SYMBOLLEN 32




typedef struct __STNewOrderRequest
{
	double nOrderQty;
	char   nOrderType;
	char   pad[3];
	
}STNewOrderRequest;


typedef struct __STCanelOrderRequest
{
	
	char sOrderIDFrME[128];
	char sExcelID[128];

}STCanelOrderRequest;


//先集中简化
typedef struct __STOrderMENode
{
	int 	nTag;
	char	nOrderID[32];	
	char 	sSymbol[SYMBOLLEN];
	char	nSide;
	char 	pad[3];
	double	nPrice;
	
	char	sText[128];
	

	STNewOrderRequest 		stNewOrderRequest;
	STCanelOrderRequest 	stCanelOrderRequest;
	
	
}STOrderMENode;


typedef struct _STServiceConfig
{

	//新订单,取消订单 
	string mq_vhost_order;
	string mq_exchange_order;
	string mq_address_order;
	string mq_port_order;
	string mq_user_order;
	string mq_password_order;
	string mq_bindingkey_order;
	string mq_queue_name_order;

	
	//新订单,取消订单 请求ACK 的消费者
	string mq_vhost_order_ack ;
	string mq_exchange_order_ack;
	string mq_address_order_ack;
	string mq_port_order_ack;
	string mq_user_order_ack;
	string mq_password_order_ack;
	string mq_routingkey_order_ack;

	

	

		
	// log info
	string  logdir;
	bool    onlyscreenshowlog;
	
	// sp
	string spUsername;
	string spPassword;
	string sTarget;
	string sSender;

	int MempoolNodeCount;
	int RealtimeSenderCount;


	string AccountID;

}STServiceConfig;


extern STServiceConfig  stServiceConfig;

extern STServiceConfig 	stServiceConfig;

uint64_t 	GetTimestamp(const char *date);

uint64_t 	GetUTCCurTimestamp() ;

int 		ReadServerConfig();

void 		InitCoinPair();

void 		InitLogSys();







class CRWBuffer
{
public:
	CRWBuffer();

	void 									PushRealtimeData(STOrderMENode *pData);
	STOrderMENode * 						GetRealtimeData();


private:
	list<STOrderMENode *>			m_RealtimeDataList1;
	list<STOrderMENode *>			*m_pRealtimeDataList_w;
	pthread_mutex_t					m_wListLock;

	list<STOrderMENode *>			m_RealtimeDataList2;
	list<STOrderMENode *>			*m_pRealtimeDataList_r;
	pthread_mutex_t					m_rListLock;

};








#endif //FIXSE_PUBLIC_HEAD_H

