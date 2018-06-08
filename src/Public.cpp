

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/time.h>
#include <signal.h>


#include "BaseThread.h"
#include "xlogger.h"

#include "Public.h"
#include "ConfigFileReader.h"


STServiceConfig  stServiceConfig;



uint64_t GetTimestamp(const char *date)
{
    int year,month,day,hour,min,sec,usec;
    assert(NULL != date);

    sscanf(date, "%04d%02d%02d-%d:%d:%d.%d", &year,&month,&day,&hour,&min,&sec,&usec);
   
    struct tm stm;
    memset(&stm,0,sizeof(stm));

    stm.tm_year = year - 1900;
    stm.tm_mon = month - 1;
    stm.tm_mday = day;
    stm.tm_hour = hour;
    stm.tm_min = min;
    stm.tm_sec = sec;

   
   	time_t time0 = mktime(&stm) ;
	time_t time1 = time(&time0) * 1000000 + usec ;
   
	return time1 ;
   
}




uint64_t GetUTCCurTimestamp()
{
    struct timeval  curtime;
    gettimeofday(&curtime,NULL);

    uint64_t cut_timestamp  = curtime.tv_sec * 1000000 + curtime.tv_usec ;
    return cut_timestamp;
}




int ReadPublicConfig(CConfigFileReader &config)
{
	char *buf = NULL;

	if (NULL == (buf = config.GetConfigName("SenderCompID")))
	{
		printf("read SenderCompID error \n");
		return -1;
	}
	stServiceConfig.sSender = buf ;

	if (NULL == (buf = config.GetConfigName("TargetCompID")))
	{
		printf("read TargetCompID error \n");
		return -1;
	}
	stServiceConfig.sTarget = buf ;

	/*	
	if (NULL == (buf = config.GetConfigName("Username")))
	{
		printf("read SenderCompID error \n");
		return -1;
	}
	stServiceConfig.spUsername = buf ;
	memset(fortexUserName,0,sizeof(fortexUserName));
	strncpy(fortexUserName,buf,sizeof(fortexUserName)-1);
	//strncpy(fortexUserName,"helloworld",sizeof(fortexUserName)-1);

	if (NULL == (buf = config.GetConfigName("Password")))
	{
		printf("read TargetCompID error \n");
		return -1;
	}
	stServiceConfig.spPassword = buf ;	
	memset(fortexPassword,0,sizeof(fortexPassword));
	strncpy(fortexPassword,buf,sizeof(fortexPassword)-1);
	*/

	
	if (NULL == (buf = config.GetConfigName("logdir")))
	 {
		 printf("read RabbitMQInfo->logdir error \n");
		 return -1;
	 }
	 stServiceConfig.logdir = buf;
	
	 if (NULL == (buf = config.GetConfigName("onlyscreenshowlog")))
	 {
		 printf("read onlyscreenshowlog error \n");
		 return -1;
	 }
	 if (1 == atoi(buf))
		 stServiceConfig.onlyscreenshowlog = true;
	 else
		 stServiceConfig.onlyscreenshowlog = false;
	


	if (NULL == (buf = config.GetConfigName("MempoolNodeCount")))
	{
		printf("read MempoolNodeCount error \n");
		return -1;
	}
	stServiceConfig.MempoolNodeCount = atoi(buf) ;

	

	if (NULL == (buf = config.GetConfigName("RealtimeSenderCount")))
	{
		printf("read RealtimeSenderCount error \n");
		return -1;
	}
	stServiceConfig.RealtimeSenderCount = atoi(buf) ;

	if(nullptr == (buf = config.GetConfigName("AccountID")))
	{
		printf("read AccountID error \n");
		return -1;
	}
	stServiceConfig.AccountID = buf;

	return 0;		

}



int ReadOrderConfig(CConfigFileReader &config)
{
	char *buf = NULL;
	
	if (NULL == (buf = config.GetConfigName("mq_vhost_order")))
	{
		printf("read RabbitMQInfo->mq_vhost_order error \n");
		return -1;
	}
	stServiceConfig.mq_vhost_order = buf;


	
	if (NULL == (buf = config.GetConfigName("mq_exchange_order")))
	{
		printf("read RabbitMQInfo->mq_exchange_order error \n");
		return -1;
	}
	stServiceConfig.mq_exchange_order = buf;


	
	if (NULL == (buf = config.GetConfigName("mq_address_order")))
	{
		printf("read RabbitMQInfo->mq_address_order error \n");
		return -1;
	}
	stServiceConfig.mq_address_order = buf;

	
	
	if (NULL == (buf = config.GetConfigName("mq_port_order")))
	{
		printf("read RabbitMQInfo->mq_port_order error \n");
		return -1;
	}
	stServiceConfig.mq_port_order = buf;

	
	
	if (NULL == (buf = config.GetConfigName("mq_user_order")))
	{
		printf("read RabbitMQInfo->mq_user_order error \n");
		return -1;
	}
	stServiceConfig.mq_user_order = buf;


	
	if (NULL == (buf = config.GetConfigName("mq_password_order")))
	{
		printf("read RabbitMQInfo->mq_password_order error \n");
		return -1;
	}
	stServiceConfig.mq_password_order = buf;


	
	if (NULL == (buf = config.GetConfigName("mq_bindingkey_order")))
	{
		printf("read RabbitMQInfo->mq_bindingkey_order error \n");
		return -1;
	}
	stServiceConfig.mq_bindingkey_order = buf;


	if (NULL == (buf = config.GetConfigName("mq_queue_name_order")))
	{
		printf("read RabbitMQInfo->mq_queue_name_order error \n");
		return -1;
	}
	stServiceConfig.mq_queue_name_order = buf;
	

	return 0;
}



int ReadOrderAckConfig(CConfigFileReader &config)
{

	char *buf = NULL;
	
	if (NULL == (buf = config.GetConfigName("mq_vhost_order_ack")))
	{
		printf("read RabbitMQInfo->mq_vhost_order_ack error \n");
		return -1;
	}
	stServiceConfig.mq_vhost_order_ack = buf;


	
	if (NULL == (buf = config.GetConfigName("mq_exchange_order_ack")))
	{
		printf("read RabbitMQInfo->mq_exchange_order_ack error \n");
		return -1;
	}
	stServiceConfig.mq_exchange_order_ack = buf;


	
	if (NULL == (buf = config.GetConfigName("mq_address_order_ack")))
	{
		printf("read RabbitMQInfo->mq_address_order_ack error \n");
		return -1;
	}
	stServiceConfig.mq_address_order_ack = buf;

	
	
	if (NULL == (buf = config.GetConfigName("mq_port_order_ack")))
	{
		printf("read RabbitMQInfo->mq_port_order_ack error \n");
		return -1;
	}
	stServiceConfig.mq_port_order_ack = buf;

	
	
	if (NULL == (buf = config.GetConfigName("mq_user_order_ack")))
	{
		printf("read RabbitMQInfo->mq_user_order_ack error \n");
		return -1;
	}
	stServiceConfig.mq_user_order_ack = buf;


	
	if (NULL == (buf = config.GetConfigName("mq_password_order_ack")))
	{
		printf("read RabbitMQInfo->mq_password_order_ack error \n");
		return -1;
	}
	stServiceConfig.mq_password_order_ack = buf;


	
	if (NULL == (buf = config.GetConfigName("mq_routingkey_order_ack")))
	{
		printf("read RabbitMQInfo->mq_routingkey_order_ack error \n");
		return -1;
	}
	stServiceConfig.mq_routingkey_order_ack = buf;	


	return 0;
}




int ReadServerConfig()
{
	
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigaction( SIGPIPE, &sa, 0 );

	CConfigFileReader config;	
		
	if( -1 == config.LoadFile(SERVICECONFIG)  ) 
	{
		printf("open config file: %s,fail:%s\n",SERVICECONFIG, strerror(errno));
		return -1;
	}

	

	if(-1==ReadPublicConfig(config))return -1;

	if(-1==ReadOrderConfig(config))return -1;
	
	if(-1==ReadOrderAckConfig(config))return -1;

	return 0;
	

}


void InitLogSys()
{
	(*XLogger::GetObject("OnezFixMarket")).StdErr(stServiceConfig.onlyscreenshowlog).Colour(true).Dir(stServiceConfig.logdir).BuffDelay(1);
	XINFO("InitLogSys ....");
}


CRWBuffer::CRWBuffer()
{
	pthread_mutex_init(&m_wListLock,NULL);
	pthread_mutex_init(&m_rListLock,NULL);

	m_pRealtimeDataList_w = &m_RealtimeDataList1;
	m_pRealtimeDataList_r = &m_RealtimeDataList2;
	
}




void CRWBuffer::PushRealtimeData(STOrderMENode *pData)
{
	assert(pData!=NULL);

	pthread_mutex_lock(&m_wListLock);
	m_pRealtimeDataList_w->push_back(pData);	
	pthread_mutex_unlock(&m_wListLock);	
}



STOrderMENode * CRWBuffer::GetRealtimeData()
{
	pthread_mutex_lock(&m_rListLock);
	{
	
		do
		{
			if(0 < m_pRealtimeDataList_r->size())
			{
				STOrderMENode *pNode = m_pRealtimeDataList_r->front();
				m_pRealtimeDataList_r->pop_front();
				pthread_mutex_unlock(&m_rListLock);
				return pNode;
			}

			pthread_mutex_lock(&m_wListLock);
			{
				if(0 == m_pRealtimeDataList_w->size())
				{
					pthread_mutex_unlock(&m_wListLock);
					pthread_mutex_unlock(&m_rListLock);
					//XINFO("CWorkDataListMgr GetDataNode: list is empty ! \n");
					return NULL;
				}
				
				list<STOrderMENode *>	*p = m_pRealtimeDataList_r;
				m_pRealtimeDataList_r = m_pRealtimeDataList_w;
				m_pRealtimeDataList_w = p;
			
			}
			pthread_mutex_unlock(&m_wListLock);
			
			continue;
			
		}while(1);
	}
	pthread_mutex_unlock(&m_rListLock);

}




