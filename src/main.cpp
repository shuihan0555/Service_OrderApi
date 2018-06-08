

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "BaseThread.h"
#include "xlogger.h"



#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 )
#else
#include "config.h"
#endif

#include "quickfix/FileStore.h"
#include "quickfix/SocketInitiator.h"
#include "quickfix/SessionSettings.h"
#include "quickfix/Log.h"

#include <string>
#include <iostream>
#include <fstream>
#include "getopt-repl.h"

#include "Public.h"
#include "MemPoolMgr.h"
#include "CRealtimeResponseFromME.h"
#include "OrderAckSender.h"
#include "OrderConsumer.h"


FIX::SessionSettings g_settings( SERVICECONFIG );


int main( int argc, char** argv )
{

	
	if(-1 == ReadServerConfig())return -1;

	InitLogSys();

	if(-1 == OrderConsumerInit() )return -1;

	COrderAckSender::StartBatchSender();
	sleep(2);

	CMemPoolMgr *pPoolMgr = new CMemPoolMgr();
	if(NULL == pPoolMgr)
	{
		XERROR("new CMemPoolMgr fail.\n");
		return -1;
	}

	
	if (-1 == pPoolMgr->Init() )
	{
		XERROR("CMemPoolMgr create mem pool fail.\n");
		return -1;
	}

	

	try
  	{
	    

	    CRealtimeResponseFromME market;
	    FIX::FileStoreFactory storeFactory( g_settings );
	    FIX::ScreenLogFactory logFactory( g_settings );
	    FIX::SocketInitiator initiator( market, storeFactory, g_settings, logFactory );

	    initiator.start();
	    market.run();
	    initiator.stop();

    return 0;
  	}
  	catch ( std::exception & e )
  	{
    	std::cout << e.what();
    	return -1;
  	}

	return 0;
}

