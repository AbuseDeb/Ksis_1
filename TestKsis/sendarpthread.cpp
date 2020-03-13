#include "sendarpthread.h"

SendArpThread::SendArpThread(IPAddr DestIP, IPAddr SrcIP)
	:m_pDestIP(DestIP), m_pSrcIP(SrcIP)
{
}

SendArpThread::~SendArpThread()
{
}

void SendArpThread::run()
{
	m_bIsActive = true;


	ULONG MacAddr[2];       /* for 6-byte hardware addresses */
	ULONG PhysAddrLen = 6;

	SendARP(m_pDestIP, m_pSrcIP, &MacAddr, &PhysAddrLen);


	m_bIsActive = false;
}
