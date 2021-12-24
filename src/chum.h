#pragma once

#include <QHash>
#include <QObject>
#include <QSet>

#include "chumpackage.h"
#include "ssu.h"

namespace PackageKit {
class Transaction;
}

class Chum : public QObject {
  Q_OBJECT
  Q_PROPERTY(bool    busy           READ busy NOTIFY busyChanged)
  Q_PROPERTY(quint32 installedCount READ installedCount NOTIFY installedCountChanged)
  Q_PROPERTY(bool    repoAvailable  READ repoAvailable NOTIFY repoUpdated)
  Q_PROPERTY(bool    repoManaged    READ repoManaged NOTIFY repoUpdated)
  Q_PROPERTY(bool    repoTesting    READ repoTesting WRITE setRepoTesting NOTIFY repoUpdated)
  Q_PROPERTY(QString status         READ status NOTIFY statusChanged)
  Q_PROPERTY(quint32 updatesCount   READ updatesCount NOTIFY updatesCountChanged)

public:
  enum PackageOperation {
    PackageUnknownOperation,
    PackageInstallation,
    PackageUpdate,
  };
  Q_ENUM(PackageOperation)

  bool    busy() const { return m_busy; }
  quint32 installedCount() const { return m_installed_count; }
  bool    repoAvailable() const { return m_ssu.repoAvailable(); }
  bool    repoManaged() const { return m_ssu.manageRepo(); }
  bool    repoTesting() const { return m_ssu.repoTesting(); }
  QString status() const { return m_status; }
  quint32 updatesCount() const { return m_updates_count; }

  void    setRepoTesting(bool testing);

  QList<ChumPackage*> packages() const { return m_packages.values(); }
  Q_INVOKABLE ChumPackage* package(const QString &id) const { return m_packages.value(id, nullptr); }

  // static public methods
  static Chum* instance();

public slots:
  void getUpdates(bool force = false);
  void refreshRepo(bool force = false);
  void installPackage(const QString &id);
  void updatePackage(const QString &id);

signals:
  void busyChanged();
  void installedCountChanged();
  void error(QString errorTxt);
  void errorFatal(QString errorTitle, QString errorTxt);
  void statusChanged();
  void updatesCountChanged();
  void packagesChanged();
  void packageOperationStarted( PackageOperation operation, const QString &name);
  void packageOperationFinished(PackageOperation operation, const QString &name, const QString &version);
  void repoUpdated(); // signal ssu properties change
  void repositoryRefreshed();

private:
  explicit Chum(QObject *parent = nullptr);

  QString packageId(const QString &pkg_id);

  void repositoriesListUpdated();

  void getUpdatesFinished();
  void refreshPackages();
  void refreshPackagesInstalled();
  void refreshPackagesFinished();
  void refreshDetails();
  void refreshInstalledVersion();

  void startOperation(PackageKit::Transaction *pktr, const QString &pkg_id);

private:
  Ssu           m_ssu;
  bool          m_busy{false};
  QString       m_status;
  quint32       m_installed_count{0};
  quint32       m_updates_count{0};

  QHash<QString, ChumPackage*> m_packages;
  QSet<QString>                m_packages_last_refresh;
  QSet<QString>                m_packages_last_refresh_installed;

  // static
  static Chum*         s_instance;
  static const QString s_repo_name;
};
