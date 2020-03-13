#pragma once

#include <QObject>
#include <QReadWriteLock>
#include <QThread>
#include <Windows.h>
#include <iphlpapi.h>

class SendArpThread : public QThread
{
	Q_OBJECT

public:
	SendArpThread(IPAddr DestIP, IPAddr SrcIP);
	~SendArpThread();

	bool m_bIsActive;



private:

	IPAddr m_pDestIP;
	IPAddr m_pSrcIP;

	void run();

};
