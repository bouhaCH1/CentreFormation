#ifndef QRCODEGENERATOR_H
#define QRCODEGENERATOR_H

#include <QString>
#include <QImage>
#include <QPixmap>

class QRCodeGenerator {
public:
    static QImage generateQRCodeImage(const QString &text, int scale = 8);
    static QPixmap generateQRCodePixmap(const QString &text, int size = 250);
};

#endif // QRCODEGENERATOR_H
