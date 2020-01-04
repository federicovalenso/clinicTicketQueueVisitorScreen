#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QByteArray>
#include <QSettings>
#include <QString>
#include <chrono>

class AppSettings : public QSettings {
  Q_OBJECT
 public:
  static AppSettings& getInstance();
  static constexpr std::chrono::milliseconds kCleanInterval{180'000};

  AppSettings(const AppSettings&) = delete;
  AppSettings(AppSettings&&) = delete;
  AppSettings& operator=(const AppSettings&) = delete;
  AppSettings& operator=(AppSettings&&) = delete;
  ~AppSettings() override = default;

  Q_INVOKABLE void setConnectionSettings(const QString& server_name,
                                         const int port,
                                         const QString& user_name,
                                         const QString& password);
  Q_INVOKABLE void setSettings(const QString& server_name, const int port,
                               const QString& user_name,
                               const QString& password, const int max_rows);

  Q_INVOKABLE QString getServerName() const;
  Q_INVOKABLE int getPort() const;
  Q_INVOKABLE QString getUserName() const;
  Q_INVOKABLE QString getPassword() const;
  Q_INVOKABLE int getMaxRows() const;

 signals:
  void connectionSettingsChanged();
  void maxRowSettingChanged(int);

 private:
  AppSettings();
  void ReadSettings();
  void setMaxRows(const int max_rows);

  static const QString kConnectionGroup;
  static const QString kserverAddr;
  static const QString kPort;
  static const QString kUserName;
  static const QString kPassword;
  static const QString kModelGroup;
  static const QString kMaxRows;

  QString server_name_;
  uint32_t port_ = 0;
  QString user_name_;
  QString password_;
  uint32_t max_rows_ = 0;
};

#endif  // APPSETTINGS_H
