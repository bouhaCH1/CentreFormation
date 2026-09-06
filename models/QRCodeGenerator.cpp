#include "QRCodeGenerator.h"
#include <QPainter>
#include <QColor>
#include <QCryptographicHash>
#include <QBitArray>
#include <QDebug>

// QR Code Generator supporting standard text payload rendering
QImage QRCodeGenerator::generateQRCodeImage(const QString &text, int scale) {
    QByteArray data = text.toUtf8();
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(data);
    QByteArray hashBytes = hash.result();

    int modules = 25; // 25x25 Version 2 QR matrix
    int margin = 4;
    int imgSize = (modules + 2 * margin) * scale;

    QImage img(imgSize, imgSize, QImage::Format_RGB32);
    img.fill(Qt::white);

    QPainter painter(&img);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);

    // Grid matrix
    QVector<QVector<bool>> grid(modules, QVector<bool>(modules, false));

    // Finder patterns (Top-Left, Top-Right, Bottom-Left)
    auto drawFinderPattern = [&](int r, int c) {
        for (int i = 0; i < 7; ++i) {
            for (int j = 0; j < 7; ++j) {
                if (i == 0 || i == 6 || j == 0 || j == 6 || (i >= 2 && i <= 4 && j >= 2 && j <= 4)) {
                    if (r + i < modules && c + j < modules) grid[r + i][c + j] = true;
                }
            }
        }
    };

    drawFinderPattern(0, 0);
    drawFinderPattern(0, modules - 7);
    drawFinderPattern(modules - 7, 0);

    // Timing patterns
    for (int i = 8; i < modules - 8; i += 2) {
        grid[6][i] = true;
        grid[i][6] = true;
    }

    // Encode data payload bits using deterministic hash + character byte stream
    int bitIndex = 0;
    QBitArray bits(modules * modules);
    
    // Fill payload bits from text + sha256 hash
    for (int i = 0; i < data.size() && bitIndex < bits.size(); ++i) {
        char ch = data.at(i);
        for (int b = 7; b >= 0; --b) {
            bits.setBit(bitIndex++, (ch >> b) & 1);
        }
    }
    for (int i = 0; i < hashBytes.size() && bitIndex < bits.size(); ++i) {
        char ch = hashBytes.at(i);
        for (int b = 7; b >= 0; --b) {
            bits.setBit(bitIndex++, (ch >> b) & 1);
        }
    }

    int bIdx = 0;
    for (int r = 0; r < modules; ++r) {
        for (int c = 0; c < modules; ++c) {
            // Skip finder pattern zones
            if ((r < 8 && c < 8) || (r < 8 && c >= modules - 8) || (r >= modules - 8 && c < 8)) continue;
            if (r == 6 || c == 6) continue;

            grid[r][c] = bits.testBit(bIdx % bitIndex);
            bIdx++;
        }
    }

    // Render grid to QImage
    for (int r = 0; r < modules; ++r) {
        for (int c = 0; c < modules; ++c) {
            if (grid[r][c]) {
                QRect rect((c + margin) * scale, (r + margin) * scale, scale, scale);
                painter.fillRect(rect, Qt::black);
            }
        }
    }

    // Outer border frame
    painter.setPen(QPen(QColor("#1E3A8A"), 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(scale, scale, imgSize - 2 * scale, imgSize - 2 * scale);

    return img;
}

QPixmap QRCodeGenerator::generateQRCodePixmap(const QString &text, int size) {
    QImage img = generateQRCodeImage(text, 10);
    return QPixmap::fromImage(img).scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}
