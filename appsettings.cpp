#include "appsettings.h"

const QString AppSettings::kConnectionGroup = "/connection";
const QString AppSettings::kserverAddr = "server_addr";
const QString AppSettings::kPort = "port";
const QString AppSettings::kUserName = "user_name";
const QString AppSettings::kPassword = "password";
const QString AppSettings::kModelGroup = "/model";
const QString AppSettings::kMaxRows = "max_rows";

AppSettings& AppSettings::getInstance() {
  static AppSettings instance;
  return instance;
}

AppSettings::AppSettings() { ReadSettings(); }

void AppSettings::setConnectionSettings(const QString& server_name,
                                        const int port,
                                        const QString& user_name,
                                        const QString& password) {
  server_name_ = server_name;
  port_ = static_cast<uint32_t>(port);
  user_name_ = user_name;
  password_ = password;

  beginGroup(kConnectionGroup);
  setValue(kserverAddr, server_name_);
  setValue(kPort, port_);
  setValue(kUserName, user_name_);
  setValue(kPassword, password_);
  endGroup();
  emit connectionSettingsChanged();
}

void AppSettings::setSettings(const QString& server_name, const int port,
                              const QString& user_name, const QString& password,
                              const int max_rows) {
  setConnectionSettings(server_name, port, user_name, password);
  setMaxRows(max_rows);
}

QString AppSettings::getServerName() const { return server_name_; }

int AppSettings::getPort() const { return static_cast<int>(port_); }

QString AppSettings::getUserName() const { return user_name_; }

QString AppSettings::getPassword() const { return password_; }

int AppSettings::getMaxRows() const { return static_cast<int>(max_rows_); }

void AppSettings::ReadSettings() {
  beginGroup(kConnectionGroup);
  server_name_ = value(kserverAddr, "192.168.0.9").toString();
  port_ = value(kPort, 8085).toUInt();
  user_name_ = value(kUserName, "voice_service").toString();
  password_ = value(kPassword, "1qazXSw2").toString();
  endGroup();
  beginGroup(kModelGroup);
  max_rows_ = value(kMaxRows, 4).toUInt();
  endGroup();
}

void AppSettings::setMaxRows(const int max_rows) {
  max_rows_ = static_cast<uint32_t>(max_rows);
  beginGroup(kModelGroup);
  setValue(kMaxRows, max_rows_);
  endGroup();
  emit maxRowSettingChanged(static_cast<int>(max_rows_));
}
