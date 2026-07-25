#include <format>
#include <cstddef>
#include <cstdlib>
#include <string_view>
#include <QApplication>
#include <QImageReader>
#include <QDir>
#include <QCommandLineParser>
#include "SCWind.h"
#include "VersionDialog.h"

int main(int argc, char** argv) {
  QApplication qapp(argc, argv);
  QCommandLineParser parser;
  parser.setApplicationDescription("Minecraft map art generator");

  parser.addOption(QCommandLineOption{
    "build-dir-mode",
    "Read blocks from local dir. Only useful for development"});
  parser.process(qapp);

  QDir::setCurrent(QCoreApplication::applicationDirPath());
  QImageReader::setAllocationLimit(INT32_MAX);

  const auto config=[&]() {
    const QString SC_default_blocks_dir =
#ifdef __linux__
        QStringLiteral("../share/SlopeCraft/Blocks");
#else
        QStringLiteral("./Blocks");
#endif
    app_config cfg;
    if (parser.isSet("build-dir-mode")) {
      cfg.blocks_dir_path = "../SCL_block_lists";
    } else {
      cfg.blocks_dir_path = SC_default_blocks_dir;
    }

    return cfg;
  }();

  SCWind wind{nullptr, config};

  wind.show();
  wind.setWindowTitle(SCWind::default_wind_title());

  bool is_language_ZH = QLocale::system().uiLanguages().contains("zh");

  // this line is used to test the translation

  for (int i = 0; i < argc; i++) {
    if (std::string_view(argv[i]) == "--lang-force-to-en") {
      is_language_ZH = false;
      break;
    }
    if (std::string_view(argv[i]) == "--lang-force-to-zh") {
      is_language_ZH = true;
      break;
    }
  }

  if (is_language_ZH) {
    wind.set_lang(::SCL_language::Chinese);
  } else {
    wind.set_lang(::SCL_language::English);
  }

  VersionDialog::start_network_request(&wind, "SlopeCraft",
                                       QUrl{SCWind::update_url},
                                       SCWind::network_manager(), false);

  return qapp.exec();
}