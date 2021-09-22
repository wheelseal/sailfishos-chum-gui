#include "chum.h"
#include "chumpackage.h"
#include "chumavailablepackagesmodel.h"
#include "chumupdatesmodel.h"
#include <sailfishapp.h>

#include <QtQuick>

#define CHUM_REGISTER_TYPE(NAME) \
  qmlRegisterType<NAME>("org.chum", 1, 0, #NAME)

int main(int argc, char *argv[]) {
  CHUM_REGISTER_TYPE(Chum);
  CHUM_REGISTER_TYPE(ChumPackage);
  CHUM_REGISTER_TYPE(ChumAvailablePackagesModel);
  CHUM_REGISTER_TYPE(ChumUpdatesModel);

  SailfishApp::application(argc, argv);
  QCoreApplication::setApplicationVersion(QStringLiteral(CHUMGUI_VERSION));

  QQuickView v;
  v.setSource(SailfishApp::pathToMainQml());
  v.show();

  return QCoreApplication::exec();
}
