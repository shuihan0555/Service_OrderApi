#include <time.h>
#include <unistd.h>

#include "OrderConsumer.h"
#include "xlogger.h"
#include "CRealtimeResponseFromME.h"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

using namespace std;
using namespace rapidjson;


using rapidjson::Document;
using rapidjson::Value;


int OrderConsumerInit()
{

	STTnodeConfig tnodeconfig ;
	tnodeconfig.mq_vhost= stServiceConfig.mq_vhost_order;
	tnodeconfig.mq_exchange_group = stServiceConfig.mq_exchange_order;
	tnodeconfig.mq_host = stServiceConfig.mq_address_order;
	tnodeconfig.mq_port= atoi(stServiceConfig.mq_port_order.c_str()) ;
	tnodeconfig.mq_user = stServiceConfig.mq_user_order;
	tnodeconfig.mq_passwd= stServiceConfig.mq_password_order;


	
	auto pNode = new snetwork_xservice_tnode::TNode(tnodeconfig);
	if(-1 == pNode->Init() )
	{
		XERROR("Change Notice Init fail , pls check ! \n");
		return -1;
	}

	
	
	
	OrderConsumer *pConsumer = new OrderConsumer(stServiceConfig.mq_bindingkey_order.c_str(),
	stServiceConfig.mq_queue_name_order.c_str());
	
	if(-1 == pNode->AddTNodeConsumer(pConsumer) )
	{
		XERROR("OrderConsumer AddTNodeConsumer hist consumber  ERROR \n");
		pNode->Close();
		return -1;
	}

	pNode->RunConsumer();

	XINFO("OrderConsumer Init ok ....\n");
	
	return 0;	
}


int ReadRequestPublic(Document &doc,STOrderMENode *pNode, char *pMsg)
{
	Value & v = doc["Tag"];
	if (!v.IsInt())
	{
		XERROR("OrderSrv --> OrderMe: Tag is not int in json:%s \n ",pMsg);
		CMemPoolMgr::GetInstance()->PushDataNode(pNode);
		return -1 ;
	}
	pNode->nTag = v.GetInt();


	Value & v1 = doc["OrderID"];
	if (!v1.IsInt64())
	{
		XERROR("OrderSrv --> OrderMe: OrderID is not long in json:%s \n ",pMsg);
		CMemPoolMgr::GetInstance()->PushDataNode(pNode);
		return -1 ;
	}
	//pNode->nOrderID = ;
	sprintf(pNode->nOrderID,"%ld",v1.GetInt64());


	Value &v2 = doc["Symbol"];
	if (!v2.IsString())
	{
		XERROR("OrderSrv --> OrderMe: Symbol is not string in json:%s \n ",pMsg);
		CMemPoolMgr::GetInstance()->PushDataNode(pNode);
		return -1 ;
	}
	string symbol = v2.GetString() ;
	strncpy(pNode->sSymbol , symbol.c_str(),SYMBOLLEN );
	

	Value & v3 = doc["Side"];
	if (!v3.IsInt())
	{
		XERROR("OrderSrv --> OrderMe: Side is not int in json:%s \n ",pMsg);
		CMemPoolMgr::GetInstance()->PushDataNode(pNode);
		return -1 ;
	}
	if(v3.GetInt() == 1)
		pNode->nSide = '1';
	else
		pNode->nSide = '2';

	using rapidjson::Value;
	Value & v4 = doc["Price"];
	if (!v4.IsDouble())
	{
		XERROR("OrderSrv --> OrderMe: Price is not double in json:%s \n ",pMsg);
		CMemPoolMgr::GetInstance()->PushDataNode(pNode);
		return -1 ;
	}
	pNode->nPrice = v4.GetDouble();

    return  0;
}



int NewOrderRequest(Document &doc,STOrderMENode *pNode, char *pMsg)
{

	using rapidjson::Value;
	Value & v3 = doc["OrderType"];
	if (!v3.IsInt())
	{
		XERROR("OrderSrv --> OrderMe: OrderType is not int in json:%s \n ",pMsg);
		CMemPoolMgr::GetInstance()->PushDataNode(pNode);
		return -1 ;
	}
	if (v3.GetInt() == 1)
		pNode->stNewOrderRequest.nOrderType = '1';
	else
		pNode->stNewOrderRequest.nOrderType = '2';
	

	

	using rapidjson::Value;
	Value & v4 = doc["OrderQty"];
	if (!v4.IsDouble())
	{
		XERROR("OrderSrv --> OrderMe: OrderQty is not double in json:%s \n ",pMsg);
		CMemPoolMgr::GetInstance()->PushDataNode(pNode);
		return -1 ;
	}

	pNode->stNewOrderRequest.nOrderQty = v4.GetDouble();
	

	return 0;
}

int CancelOrderRequest(Document &doc,STOrderMENode *pNode, char *pMsg)
{

	Value & v1 = doc["OrderIDFrME"];
	if (!v1.IsString())
	{
		XERROR("OrderSrv --> OrderMe: OrderIDFrME is not string in json:%s \n ",pMsg);
		CMemPoolMgr::GetInstance()->PushDataNode(pNode);
		return -1 ;
	}
	string idfromme = v1.GetString(); 
	strncpy(pNode->stCanelOrderRequest.sOrderIDFrME ,  idfromme.c_str() ,127);


	Value & v2 = doc["ExcelID"];
	if (!v2.IsString())
	{
		XERROR("OrderSrv --> OrderMe: ExcelID is not string in json:%s \n ",pMsg);
		CMemPoolMgr::GetInstance()->PushDataNode(pNode);
		return -1 ;
	}
	string excelid = v2.GetString() ;
	strncpy(pNode->stCanelOrderRequest.sExcelID , excelid.c_str(),127 );

	return 0;
	
}


uint32 OrderConsumer::ConsumerData(char *pMsg,int nMsgLen)
{

	XINFO("OrderSrv --> OrderMe:[%d][%s]",nMsgLen,pMsg);
		
	Document doc;
	doc.Parse<0>(pMsg);
	if (doc.HasParseError())
	{
		XINFO("OrderSrv --> OrderMe: json parse error [%s]",doc.GetParseError());   
		return -1;
	}

	STOrderMENode * pNode = CMemPoolMgr::GetInstance()->GetDataNode();
	if(NULL == pNode)
	{
		XERROR("OrderSrv --> OrderMe: Get DataNode from mempool fail \n ");
		return -1;
	}
	
		
	if(-1 == ReadRequestPublic(doc,pNode,pMsg))return 0;


	int ret = -1; 
	if(NEWORDERREQUEST == pNode->nTag )
	{
		ret = NewOrderRequest(doc,pNode,pMsg);
	}
	else if(CANCELORDERREQUEST == pNode->nTag )
	{
		ret = CancelOrderRequest(doc,pNode,pMsg);
	}
	else
	{
		XERROR("OrderSrv --> OrderMe: Tag is error:0x%04x \n ",pNode->nTag);
		CMemPoolMgr::GetInstance()->PushDataNode(pNode);
		return -1;
	}
	  

	if(-1 == ret)return -1;

	
	CRealtimeResponseFromME::m_RWBuffer.PushRealtimeData(pNode);
			
	return 0;
}





