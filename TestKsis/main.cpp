

#include <QtCore/QCoreApplication>
#include <QNetworkInterface> 
#include <iostream>
#include <QtDebug>
#include <QtCore>
#include <Windows.h>
#include <stdint.h>
#include <stdio.h>

#include <iphlpapi.h>



#pragma comment(lib, "iphlpapi.lib") // 



#define REV(n) ((n << 24) | (((n>>16)<<24)>>16) | (((n<<16)>>24)<<16) | (n>>24))


void send(IPAddr DestIP, IPAddr SrcIP, PVOID &pMacAddr, PULONG &PhyAddrLen)
{
	SendARP(DestIP, SrcIP, pMacAddr, PhyAddrLen);
}


int main(int argc, char *argv[])
{
	
	QCoreApplication a(argc, argv);
	
	QList<QNetworkInterface> Interfaces = QNetworkInterface::allInterfaces();

	QLibrary library("iphlpapi.dll");

	if (!library.load())
		qDebug() << library.errorString();
	if (library.load())
		qDebug() << "library loaded";



	typedef ULONG (*Func)(IPAddr DestIP, IPAddr SrcIP, PVOID pMacAddr, PULONG PhyAddrLen);
	Func sendArp = (Func)library.resolve("SendARP");


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

					uint DestIP = AddressOfInterface[j].ip().toIPv4Address() & AddressOfInterface[j].netmask().toIPv4Address();	попробовать htons
					

					uint SrcIP = AddressOfInterface[j].ip().toIPv4Address();
					ULONG MacAddr[2];       /* for 6-byte hardware addresses */
					ULONG PhysAddrLen = 6;  /* default to length of six bytes */

				
					for (DestIP++; DestIP < AddressOfInterface[j].broadcast().toIPv4Address(); DestIP++)
					{
						QHostAddress host(DestIP);
						QHostAddress host2(SrcIP);
						std::cout << host2.toString().toStdString()<< "   "<< SrcIP << std::endl;


						
						std::cout << host.toString().toStdString() << std::endl;
						
						SendARP(DestIP, NULL, &MacAddr, &PhysAddrLen);//Сделать в потоке

						//CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)send, (REV(DestIP), REV(SrcIP), &MacAddr, &PhysAddrLen), NULL, NULL);//TcpClient(newConnection)

						/*if (sendArp) {
							sendArp(REV(DestIP), NULL, &MacAddr, &PhysAddrLen);
							qDebug() << "nor";
						}*/
						

						//if (a == ERROR_NOT_FOUND)
						//////{
						//	std::cout << "+";
						//}
						
						//Sleep(100);
						
					}
				}							
			}
			std::cout << "\n\n";
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
