
/*******************************************************************************************

*Department:	Exchange team

*Decrible:  	Order Consumer :  NewOrder/CanelOrder
				
*Auhor:			Savin

*Createdtime:	2018-02-02

*ModifyTime:	


********************************************************************************************/
#ifndef ORDER_CONSUMER_HEAD_H
#define ORDER_CONSUMER_HEAD_H

#include "tnode.h"
#include "Public.h"
#include "MemPoolMgr.h"


using namespace snetwork_xservice_tnode;


int OrderConsumerInit();



class OrderConsumer : public TNodeConsumer
{
public:
	OrderConsumer(const char *bindingkey, const char *queuename  )
	{
		
		m_bindingkey = bindingkey ;
		m_queuename = queuename;
	}
	OrderConsumer(){}
	
	std::string GetBindingkey(){ return m_bindingkey ; }
	std::string GetQueueName(){ return m_queuename ; }

	
	 unsigned int ConsumerData(char *pMsg,int nMsgLen);
	
	

private:
	
	std::string m_bindingkey ;
	std::string m_queuename  ;

};



#endif //ORDER_CONSUMER_HEAD_H


