

#include <QtCore/QCoreApplication>
#include <QNetworkInterface> 
#include <iostream>
#include <QtDebug>
#include <QtCore>
#include <Windows.h>
#include "sendarpthread.h"

#include <iphlpapi.h>
//#include <ws2tcpip.h>


#pragma comment (lib, "Ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib") 


//#define REV(n) ((n << 24) | (((n>>16)<<24)>>16) | (((n<<16)>>24)<<16) | (n>>24))



int main(int argc, char *argv[])
{
	
	QCoreApplication a(argc, argv);
	

	QList<SendArpThread*> lpSendArpThread;

	QList<QNetworkInterface> Interfaces = QNetworkInterface::allInterfaces();


	for (int i = 0; i < Interfaces.size(); i++)
	{
		QNetworkInterface::InterfaceType asd = Interfaces[i].type();

		QNetworkInterface::InterfaceFlags sda = Interfaces[i].flags();

		if (Interfaces[i].type() == QNetworkInterface::Ethernet || Interfaces[i].type() == QNetworkInterface::Wifi || Interfaces[i].type() == QNetworkInterface::Unknown)
		{
			std::cout << "Name            \t" << Interfaces[i].humanReadableName().toStdString() << std::endl;
			std::cout << "Hardware address\t" << Interfaces[i].hardwareAddress().toStdString() << std::endl;
			QList<QNetworkAddressEntry> AddressOfInterface = Interfaces[i].addressEntries();

			for (int j = 1; j < AddressOfInterface.size(); j++)
			{
				if (Interfaces[i].flags() & QNetworkInterface::IsUp)
				{
					std::cout << "broadcast       \t" << AddressOfInterface[j].broadcast().toString().toStdString() << std::endl;
					std::cout << "ipV4            \t" << AddressOfInterface[j].ip().toString().toStdString() << std::endl;
					std::cout << "Mask            \t" << AddressOfInterface[j].netmask().toString().toStdString() << std::endl;

					IPAddr DestIP = AddressOfInterface[j].ip().toIPv4Address() & AddressOfInterface[j].netmask().toIPv4Address();
					
					

					IPAddr SrcIP = inet_addr(AddressOfInterface[j].ip().toString().toStdString().c_str());
					ULONG MacAddr[2];       /* for 6-byte hardware addresses */
					ULONG PhysAddrLen = 6;  /* default to length of six bytes */

				
					for (DestIP++; DestIP < AddressOfInterface[j].broadcast().toIPv4Address(); DestIP++)
					{
						
						QHostAddress DestIPHost(DestIP);
						IPAddr DestIPN = inet_addr(DestIPHost.toString().toStdString().c_str());
						//QHostAddress DestIPNHost(DestIPN);

						//std::cout << DestIPN << "   " << DestIPNHost.toString().toStdString() << std::endl;

						QHostAddress SrcIPSource(SrcIP);
						//std::cout << SrcIPSource.toString().toStdString()<< "   "<< SrcIP << std::endl;

						SendArpThread *pSendArp = new SendArpThread(DestIPN, SrcIP);
						pSendArp->start();

						lpSendArpThread.append(pSendArp);

						//std::cout << host.toString().toStdString() << std::endl;
						
						//SendARP(DestIPN, SrcIP, &MacAddr, &PhysAddrLen);//Сделать в потоке

						//CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)send, (REV(DestIP), REV(SrcIP), &MacAddr, &PhysAddrLen), NULL, NULL);//TcpClient(newConnection)

						
					}
				}							
			}
			std::cout << "\n\n";
		}
	}
	bool isOk = false;

	while (isOk == false)
	{
		isOk == true;
		foreach(const SendArpThread *pThread, lpSendArpThread)
		{
			if (pThread->m_bIsActive == true)
			{
				isOk = false;
				break;
			}		
			isOk = true;
		}
	}



	QProcess cmd;
	cmd.start("cmd.exe", QStringList() << "/c" << "chcp 437");
	cmd.waitForStarted();
	cmd.waitForFinished();
	cmd.start("cmd.exe", QStringList() << "/c" << "arp -a");
	cmd.waitForStarted();
	cmd.waitForFinished();
	QString ARP = cmd.readAll();

	std::cout << ARP.toStdString();
	//out << QString(ddd) << endl;

	return a.exec();
}
