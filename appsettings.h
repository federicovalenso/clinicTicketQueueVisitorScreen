#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QSettings>
#include <QString>
#include <QByteArray>

class AppSettings
{
public:
    static AppSettings& getInstance();
    void setConnectionSettings(const QString& serverAddr,
                               const int port);

    QString getServerAddr() const;
    int getPort() const;

private:
    AppSettings();
    AppSettings(const AppSettings&) = delete;
    AppSettings& operator=(AppSettings&) = delete;
    void ReadSettings();

    QSettings mSettings;
    const QString CONNECTION_GROUP = "/connection";
    const QString SERVER_ADDR = "server_addr";
    const QString PORT = "port";

    QString mServerAddr;
    int mPort;
};

#endif // APPSETTINGS_H
