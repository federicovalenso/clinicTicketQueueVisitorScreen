#include "appsettings.h"

AppSettings& AppSettings::getInstance()
{
    static AppSettings instance;
    return instance;
}

AppSettings::AppSettings()
{
    ReadSettings();
}

void AppSettings::setConnectionSettings(const QString& serverAddr, const int port)
{
    mServerAddr = serverAddr;
    mPort = port;

    mSettings.beginGroup(CONNECTION_GROUP);
    mSettings.setValue(SERVER_ADDR, serverAddr);
    mSettings.setValue(PORT, port);
    mSettings.endGroup();
}

QString AppSettings::getServerAddr() const
{
    return mServerAddr;
}

int AppSettings::getPort() const
{
    return mPort;
}

void AppSettings::ReadSettings()
{
    mSettings.beginGroup(CONNECTION_GROUP);
    mServerAddr = mSettings.value(SERVER_ADDR, "192.168.0.15").toString();
    mPort = mSettings.value(PORT, 8085).toInt();
    mSettings.endGroup();
}
