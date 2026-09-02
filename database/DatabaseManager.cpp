#include "DatabaseManager.h"
#include <QtGlobal>
#include <QSqlError>
#include <QDebug>


#ifdef Q_OS_WIN
#include <windows.h>
#endif

DatabaseManager::DatabaseManager() {
#ifdef Q_OS_WIN
    // Load Oracle XE client DLL path on Windows
    SetDllDirectoryA("C:\\oraclexe\\app\\oracle\\product\\11.2.0\\server\\bin");
    LoadLibraryA("C:\\oraclexe\\app\\oracle\\product\\11.2.0\\server\\bin\\oci.dll");
#endif

    // Primary driver: QOCI (Oracle native)
    if (QSqlDatabase::isDriverAvailable("QOCI")) {
        m_db = QSqlDatabase::addDatabase("QOCI");
        m_db.setHostName("localhost");
        m_db.setPort(1521);
        m_db.setDatabaseName("XE");
        m_db.setUserName("SYSTEM");
        m_db.setPassword("180702");
    } else {
        // Fallback: QODBC
        m_db = QSqlDatabase::addDatabase("QODBC");
        m_db.setDatabaseName("DRIVER={Oracle in XE};Server=localhost;Port=1521;Database=XE;Uid=SYSTEM;Pwd=180702;");
    }
}


DatabaseManager::~DatabaseManager() {
    disconnect();
}

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::connect() {
    if (m_db.isOpen()) {
        return true;
    }
    if (!m_db.open()) {
        qCritical() << "Erreur de connexion Oracle:" << m_db.lastError().text();
        return false;
    }
    qDebug() << "Connexion réussie à Oracle XE!";
    return true;
}

void DatabaseManager::disconnect() {
    if (m_db.isOpen()) {
        m_db.close();
    }
}

bool DatabaseManager::isConnected() const {
    return m_db.isOpen();
}

QString DatabaseManager::lastError() const {
    return m_db.lastError().text();
}
