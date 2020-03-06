

#include <QtCore/QCoreApplication>
#include <QNetworkInterface> 
#include <iostream>
#include <QtDebug>
#include <QtCore>
#include <Windows.h>
#include <stdint.h>

#include <iphlpapi.h>


#pragma comment(lib, "iphlpapi.lib") // 

int main(int argc, char *argv[])
{
	
	QCoreApplication a(argc, argv);


	QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
	QTextStream out(stdout);
	out.setCodec(codec);
	//QNetworkInterface Network;
	
	QList<QNetworkInterface> Interfaces = QNetworkInterface::allInterfaces();


	for (int i = 0; i < Interfaces.size(); i++)
	{
		if (Interfaces[i].type() == QNetworkInterface::Ethernet || Interfaces[i].type() == QNetworkInterface::Wifi)
		{
			std::cout << Interfaces[i].humanReadableName().toStdString() << std::endl;
			std::cout << Interfaces[i].hardwareAddress().toStdString() << std::endl;
			QList<QNetworkAddressEntry> AddressOfInterface = Interfaces[i].addressEntries();

			for (int j = 0; j < AddressOfInterface.size(); j++)
			{
				std::cout << AddressOfInterface[j].broadcast().toString().toStdString() << std::endl;
				std::cout << AddressOfInterface[j].ip().toString().toStdString() << std::endl;
				std::cout << AddressOfInterface[j].netmask().toString().toStdString() << std::endl;

				IPAddr DestIP = AddressOfInterface[j].ip().toIPv4Address() & AddressOfInterface[j].netmask().toIPv4Address();					
				IPAddr SrcIP = AddressOfInterface[j].ip().toIPv4Address();
				ULONG MacAddr[2];       /* for 6-byte hardware addresses */
				ULONG PhysAddrLen = 6;  /* default to length of six bytes */

				for (DestIP++; DestIP < AddressOfInterface[j].broadcast().toIPv4Address(); DestIP++)
				{
					//QHostAddress host(DestIP);
					//std::cout << host.toString().toStdString() << std::endl;
					SendARP(DestIP, SrcIP, &MacAddr, &PhysAddrLen);
				}				
			}
		}
	}

	QProcess cmd;
	cmd.start("cmd.exe", QStringList() << "/c" << "chcp 437");
	cmd.waitForStarted();
	cmd.waitForFinished();
	cmd.start("cmd.exe", QStringList() << "/c" << "arp -a");
	cmd.waitForStarted();
	cmd.waitForFinished();
	QString ddd = cmd.readAll();

	out << QString(ddd) << endl;

	return a.exec();
}
