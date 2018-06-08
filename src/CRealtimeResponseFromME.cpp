

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <algorithm> 

#include "CRealtimeResponseFromME.h"
#include "xlogger.h"
#include "MemPoolMgr.h"

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 )
#else
#include "config.h"
#endif

#include "quickfix/Session.h"
#include <iostream>
#include "OrderAckSender.h"
//#include "Public.h"
#include "quickfix/fix44/ExecutionReport.h"


extern FIX::SessionSettings g_settings ;

const int PRECISIONLEN = 100000000 ;



CRWBuffer CRealtimeResponseFromME::m_RWBuffer;





void CRealtimeResponseFromME::toAdmin( FIX::Message& message,
                           const FIX::SessionID& sessionID ) 
{
   const FIX::Dictionary& session_settings = g_settings.get(sessionID);
   const std::string& field = FIELD_GET_REF( message.getHeader(), MsgType);
   if(FIX::MsgType_Logon == field) 
   {
	
        FIX::Username username = session_settings.getString("Username");
        FIX::Password password = session_settings.getString("Password");
        message.setField(FIX::Username(username));
        message.setField(FIX::Password(password));
        message.setField(FIX::HeartBtInt(30));
	    message.setField(FIX::EncryptMethod(0));
	    message.setField(FIX::ResetSeqNumFlag(1));
    }
    std::cout << "1 to Admin: " << message << std::endl;
	
}



void CRealtimeResponseFromME::fromAdmin( const FIX::Message& message, const FIX::SessionID& sessionID )
throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon ) 
{
	std::cout << "2 from Admin:" << message << std::endl;
}
						   


void CRealtimeResponseFromME::onLogon( const FIX::SessionID& sessionID )
{
  	XINFO("Logon sucess \n");
	m_nIsLogon = 1;
 
 	//ReadCoinpairConfig();

	/*
	try
	{
		SubscribeQuoteRequest();
	} 
	catch ( std::exception & e )
	{
		cout <<  "Message Not Sent: %s" << e.what() << std::endl;
	}
	*/
	
  
}

void CRealtimeResponseFromME::onLogout( const FIX::SessionID& sessionID )
{
  	XERROR("Logout, pls check network/ip/port/user/passwd ... etc \n");
	m_nIsLogon = 0;
}




void CRealtimeResponseFromME::toApp( FIX::Message& message, const FIX::SessionID& sessionID )
throw( FIX::DoNotSend )
{
  try
  {
    FIX::PossDupFlag possDupFlag;
    message.getHeader().getField( possDupFlag );
    if ( possDupFlag ) throw FIX::DoNotSend();
  }
  catch ( FIX::FieldNotFound& ) {}

}




/*

以前文档中：
3.6  4.3 被替代

// 新的下单，撤单 的ack 消息: ME --> OrderSrv
订单/撤单ACK
{
	"Tag": 0x3006(order接收ack),0x30061(order拒绝ack),0x4003(撤单接收ACK)
	"OrderID":123212,  
	"ExcelID":"34324",          // order接收ack才有值
	"OrderIDFrME":"2342344",    // order接收ack才有值
	"Text":"asdfasdfasdf",		// order拒绝时的理由,或者撤单时拒绝的理由  这两种情况才有值
	"Time":""
}


*/

void CRealtimeResponseFromME::fromApp( const FIX::Message& message, const FIX::SessionID& sessionID )
throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType )
{
	XINFO("fromApp:%s\n",message.toString().c_str());
	
	STOrderMENode * pNode = CMemPoolMgr::GetInstance()->GetDataNode();
	if(NULL == pNode)
	{
		XERROR("ME --> OrderMe fromApp: Get DataNode from mempool fail \n ");
		return ;
	}

	int hasData = 0;
    const std::string& msgtype = FIELD_GET_REF( message.getHeader(), MsgType);

	if(0 == msgtype.compare("9"))
	{
		hasData = 1;
		//string rejectreason=  message.getField(58);
		strncpy(pNode->sText,(message.getField(58)).c_str(),sizeof(pNode->sText));
		pNode->nTag = 0x4003;
	}
	else if(0 == msgtype.compare("8") )
	{
		//这里得给marvin反应, 订单ACK,撤单ACK 都是8时 区分不开; 所以加上的4状态 在OrderStatus: 0:new order, 4:Canceled,8 reject(order reject)
		string orderstatus = message.getField(39);
		if('0' == orderstatus.at(0)) // order ack
		{
			hasData = 1;
			string excelid = message.getField(17);
			strncpy(pNode->stCanelOrderRequest.sExcelID,excelid.c_str(),sizeof(pNode->stCanelOrderRequest.sExcelID)-1);
			//pNode->stCanelOrderRequest.sExcelID = message.getField(17);
			//pNode->stCanelOrderRequest.sOrderIDFrME = message.getField(37);
			string idFrmMe = message.getField(37);
			strncpy(pNode->stCanelOrderRequest.sOrderIDFrME,idFrmMe.c_str(),sizeof(pNode->stCanelOrderRequest.sOrderIDFrME)-1);

            string orderid = message.getField(11);
            strncpy(pNode->nOrderID,orderid.c_str(),sizeof(pNode->nOrderID));

			pNode->nTag = 0x3006  ;
			
		}
		else if('4' == orderstatus.at(0)) // order canel ack
		{
            string orderid = message.getField(41);
            strncpy(pNode->nOrderID,orderid.c_str(),sizeof(pNode->nOrderID));

			hasData = 1;
			pNode->nTag = 0x4003  ;
		}
		else if('8' == orderstatus.at(0)) // order ack reject
		{
			hasData = 1;
            string orderid = message.getField(41);
            strncpy(pNode->nOrderID,orderid.c_str(),sizeof(pNode->nOrderID));

			string sText = message.getField(58);
			strncpy(pNode->sText,sText.c_str(),sizeof(pNode->sText)-1);
			pNode->nTag = 0x30061  ;
			
		}
	}


	if(1 == hasData)
	{
		COrderAckSender::m_RWBuffer.PushRealtimeData(pNode);
	}
	else
	{
		CMemPoolMgr::GetInstance()->PushDataNode(pNode);
	}


	//crack(message, sessionID);



}






int CRealtimeResponseFromME::ReadCoinpairConfig()
{
	/*	
	CoinPairAddList.clear();

	FILE* fp = fopen(COINPAIRECONFIG, "r");
	if (!fp)
	{
		XERROR("can not open %s", COINPAIRECONFIG);
		exit(-1);
	}

	char buf[16];
	for (;;)
	{
		memset(buf, 0, 16);
		char* p = fgets(buf, 16, fp);
		if (!p)break;
		buf[6]='\0';
		CoinPairAddList.push_back(buf);
	}


	fclose(fp);

	*/

	return 0;
}



 

void CRealtimeResponseFromME::run()
{

	while(1)
	{
		if(0 == m_nIsLogon)
		{
			sleep(1);
			continue;
		}

		STOrderMENode *pNode = m_RWBuffer.GetRealtimeData();	
		if(NULL == pNode)
		{
			usleep(100);
			continue;
		}

		if(NEWORDERREQUEST == pNode->nTag)
		{
			SendNewOrderSingle44(pNode);
		}
		else if(CANCELORDERREQUEST == pNode->nTag)
		{
			SendOrderCancelRequest44(pNode);
		}
		
		CMemPoolMgr::GetInstance()->PushDataNode(pNode);
		
	}
	

}





void CRealtimeResponseFromME::SendNewOrderSingle44(STOrderMENode *pNode)
{

	assert(pNode != NULL);

	FIX44::NewOrderSingle newOrderSingle;
	
	newOrderSingle.setField(FIX::Account(stServiceConfig.AccountID));
	newOrderSingle.setField(FIX::ClOrdID(pNode->nOrderID));
	newOrderSingle.setField(FIX::Symbol(pNode->sSymbol));
	newOrderSingle.setField(FIX::OrdType(pNode->stNewOrderRequest.nOrderType));
	newOrderSingle.setField(FIX::Side(pNode->nSide));
 
	newOrderSingle.setField(44,std::to_string(pNode->nPrice));
	newOrderSingle.setField(38,std::to_string(pNode->stNewOrderRequest.nOrderQty  ));
	
	newOrderSingle.setField(FIX::OrderQty(pNode->nSide));
	newOrderSingle.setField(FIX::TransactTime(true));

	FIX::Header &head = newOrderSingle.getHeader();
  	head.setField( FIX::SenderCompID( stServiceConfig.sSender) );
  	head.setField( FIX::TargetCompID(stServiceConfig.sTarget) );

	XINFO("SendNewOrderSingle44:[%s]\n", newOrderSingle.toString().c_str() );
	
	FIX::Session::sendToTarget( newOrderSingle );
	
}






void  CRealtimeResponseFromME::SendOrderCancelRequest44(STOrderMENode *pNode)
{
	FIX44::OrderCancelRequest orderCancelRequest;

	orderCancelRequest.setField(FIX::OrigClOrdID( pNode->nOrderID));
	orderCancelRequest.setField(FIX::ClOrdID("0"));
	//orderCancelRequest.setField(FIX:ExcelID(pNode->stCanelOrderRequest.sExcelID));
	orderCancelRequest.setField(17,pNode->stCanelOrderRequest.sExcelID);
	orderCancelRequest.setField(37,pNode->stCanelOrderRequest.sOrderIDFrME);
	
	orderCancelRequest.setField(FIX::Symbol(pNode->sSymbol));
	orderCancelRequest.setField(FIX::Side(pNode->nSide));
	orderCancelRequest.setField(44,std::to_string(pNode->nPrice));

 	FIX::Header &head = orderCancelRequest.getHeader();
  	head.setField( FIX::SenderCompID( stServiceConfig.sSender) );
  	head.setField( FIX::TargetCompID(stServiceConfig.sTarget) );

	XINFO("orderCancelRequest44:[%s]\n", orderCancelRequest.toString().c_str() );
	
  	FIX::Session::sendToTarget( orderCancelRequest );
	
}






