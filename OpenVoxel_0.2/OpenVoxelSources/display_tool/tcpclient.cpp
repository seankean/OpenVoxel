/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>
#include <QtNetwork>

#include "tcpclient.h"

//! [0]
tcpClient::tcpClient(QWidget *parent)
:   QObject(parent), networkSession(0)
{


//! [0]
//     hostString = new QString(tr("&Server name:"));
//     portString = new QString(tr("S&erver port:"));

    // find out which IP to connect to
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhostb
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

//     hostLineEdit = new QLineEdit(ipAddress);
//     portLineEdit = new QLineEdit;
//     portLineEdit->setValidator(new QIntValidator(1, 65535, this));

//     hostString->setBuddy(hostLineEdit);
//     portString->setBuddy(portLineEdit);

//     statusString = new QString(tr("This examples requires that you run the "
//                                 "Fortune Server example as well."));

//     getFortuneButton = new QPushButton(tr("Get Fortune"));
//     getFortuneButton->setDefault(true);
//     getFortuneButton->setEnabled(false);

//     quitButton = new QPushButton(tr("Quit"));

//     buttonBox = new QDialogButtonBox;
//     buttonBox->addButton(getFortuneButton, QDialogButtonBox::ActionRole);
//     buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

//! [1]
    tcpSocket = new QTcpSocket(this);
//! [1]

//     connect(hostLineEdit, SIGNAL(textChanged(QString)),
//             this, SLOT(enableGetFortuneButton()));
//     connect(portLineEdit, SIGNAL(textChanged(QString)),
//             this, SLOT(enableGetFortuneButton()));
//     connect(getFortuneButton, SIGNAL(clicked()),
//             this, SLOT(requestNewFortune()));
//     connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
//! [2] //! [3]
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readFortune()));
//! [2] //! [4]
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
//! [3]
            this, SLOT(displayError(QAbstractSocket::SocketError)));
//! [4]

//     QGridLayout *mainLayout = new QGridLayout;
//     mainLayout->addWidget(hostString, 0, 0);
//     mainLayout->addWidget(hostLineEdit, 0, 1);
//     mainLayout->addWidget(portString, 1, 0);
//     mainLayout->addWidget(portLineEdit, 1, 1);
//     mainLayout->addWidget(statusString, 2, 0, 1, 2);
//     mainLayout->addWidget(buttonBox, 3, 0, 1, 2);
//     setLayout(mainLayout);

//     setWindowTitle(tr("Fortune tcpClient"));
//     portLineEdit->setFocus();

    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, SIGNAL(opened()), this, SLOT(sessionOpened()));

	//        getFortuneButton->setEnabled(false);
	// statusString->setText(tr("Opening network session."));
        networkSession->open();
    }
//! [5]
}
//! [5]

//! [6]
void tcpClient::requestCam(QString *hostip,int port)
{
  //    getFortuneButton->setEnabled(false);
    blockSize = 0;
    tcpSocket->abort();
//! [7]
    tcpSocket->connectToHost(*hostip,port);
//! [7]
}
//! [6]

//! [8]
void tcpClient::readCam()
{
//! [9]
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);

    if (blockSize == 0) {
        if (tcpSocket->bytesAvailable() < (int)sizeof(quint16))
            return;
//! [8]

//! [10]
        in >> blockSize;
    }

    if (tcpSocket->bytesAvailable() < blockSize)
        return;
//! [10] //! [11]

    QString cargo;
    in >> cargo;

//     if (nextFortune == currentFortune) {
//         QTimer::singleShot(0, this, SLOT(requestNewFortune()));
//         return;
//     }
//! [11]

//! [12]
//    currentFortune = nextFortune;
//! [9]
//     statusString->setText(currentFortune);
//     getFortuneButton->setEnabled(true);
}
//! [12]

//! [13]
void tcpClient::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
      qDebug() << "RemoteHostClosedError";
      break;
    case QAbstractSocket::HostNotFoundError:
      qDebug() << "RemoteHostClosedError";
      //         QMessageBox::information(this, tr("Cam tcpClient"),
      //                                  tr("The host was not found. Please check the "
      //                                     "host and port settings."));
      break;
    case QAbstractSocket::ConnectionRefusedError:
      qDebug() << "RemoteHostClosedError";
      //         QMessageBox::information(this, tr("Cam tcpClient"),
      //                                  tr("The connection was refused by the peer. "
      //                                     "Make sure the fortune server is running, "
      //                                     "and check that the host name and port "
      //                                     "settings are correct."));
      break;
    default:
      qDebug() << "RemoteHostClosedError";
      //         QMessageBox::information(this, tr("Cam tcpClient"),
      //                                  tr("The following error occurred: %1.")
      //                                  .arg(tcpSocket->errorString()));
      break;
    }
    
    //    getFortuneButton->setEnabled(true);
}
//! [13]

// void tcpClient::enableGetFortuneButton()
// {
//     getFortuneButton->setEnabled((!networkSession || networkSession->isOpen()) &&
//                                  !hostLineEdit->text().isEmpty() &&
//                                  !portLineEdit->text().isEmpty());

// }

void tcpClient::sessionOpened()
{
    // Save the used configuration
    QNetworkConfiguration config = networkSession->configuration();
    QString id;
    if (config.type() == QNetworkConfiguration::UserChoice)
        id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
    else
        id = config.identifier();

    QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
    settings.beginGroup(QLatin1String("QtNetwork"));
    settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
    settings.endGroup();

//     statusStringb->setText(tr("This examples requires that you run the "
//                             "Fortune Server example as well."));

//     enableGetFortuneButton();
}

