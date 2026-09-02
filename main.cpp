#include <QApplication>
#include <QStyleFactory>
#include <QMessageBox>
#include <QFont>
#include "database/DatabaseManager.h"
#include "views/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Force Fusion style for consistent cross-platform rendering
    app.setStyle(QStyleFactory::create("Fusion"));

    // Application metadata
    app.setApplicationName("Centre de Formation");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("ESPRIT");

    // Set global font
    QFont font("Segoe UI", 10);
    font.setStyleHint(QFont::SansSerif);
    app.setFont(font);

    // Connect to Oracle database (single connection - singleton)
    if (!DatabaseManager::instance().connect()) {
        QMessageBox::critical(
            nullptr,
            "Erreur de connexion",
            QString("Impossible de se connecter à la base de données Oracle XE.\n\n"
                    "Vérifiez que :\n"
                    "  • Le service Oracle XE est démarré\n"
                    "  • Le port 1521 est accessible\n"
                    "  • Les identifiants sont corrects\n\n"
                    "Détail: %1").arg(DatabaseManager::instance().lastError())
        );
        return 1;
    }

    MainWindow window;
    window.show();

    return app.exec();
}
