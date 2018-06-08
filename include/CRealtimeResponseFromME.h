/*******************************************************************************************

*Department:	Exchange team

*Decrible:  	recv data fram , process
				
*Auhor:			Savin

*Createdtime:	2018-02-02

*ModifyTime:	


********************************************************************************************/


#ifndef FROMME_HEAD_H
#define FROMME_HEAD_H




#include "quickfix/Application.h"
#include "quickfix/MessageCracker.h"
#include "quickfix/Values.h"
#include "quickfix/Mutex.h"
#include "quickfix/fix44/QuoteRequest.h"
#include "quickfix/fix44/MarketDataRequest.h"
#include "quickfix/fix44/NewOrderSingle.h"
#include "quickfix/fix44/OrderCancelRequest.h"

#include "Public.h"
#include <queue>
#include <list>

using namespace std;

class CRealtimeResponseFromME :
      public FIX::Application,
      public FIX::MessageCracker
{
public:
	CRealtimeResponseFromME(){nCoinfileTime = 0;m_nIsLogon=0;}
 	void run();
	static CRWBuffer m_RWBuffer;

private:
  	void 	onCreate( const FIX::SessionID& ) {}
  	void 	onLogon( const FIX::SessionID& sessionID );
  	void 	onLogout( const FIX::SessionID& sessionID );
  	void 	toAdmin( FIX::Message&, const FIX::SessionID& ) ;
  	void 	toApp( FIX::Message&, const FIX::SessionID& )throw( FIX::DoNotSend );
  	void 	fromAdmin( const FIX::Message&, const FIX::SessionID& )throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon ) ;
  	void 	fromApp( const FIX::Message& message, const FIX::SessionID& sessionID )throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType );
	//void 	onMessage( const  FIX44::ExecutionReport& executionReport, const FIX::SessionID& session )throw(FIX::UnsupportedMessageType) ;
  

	int 	ReadCoinpairConfig();	

	void 	SendNewOrderSingle44(STOrderMENode *pNode);
	void	SendOrderCancelRequest44(STOrderMENode *pNode);
	




private:
	time_t 			nCoinfileTime ;
	list<string> 	CoinPairList;
	list<string> 	CoinPairTmpList;
	list<string> 	CoinPairAddList;
	list<string> 	CoinPairDelList;

	int				m_nIsLogon;				// 0 未登陆,1 已登陆
	
 
};





#endif //FROMME_HEAD_H

