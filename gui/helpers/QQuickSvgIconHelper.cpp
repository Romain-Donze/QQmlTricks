
#include "QQuickSvgIconHelper.h"

#include <QUrl>
#include <QDir>
#include <QHash>
#include <QFile>
#include <QImage>
#include <QDebug>
#include <QPainter>
#include <QDirIterator>
#include <QStringBuilder>
#include <QCoreApplication>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QCryptographicHash>
#include <QSvgRenderer>
#include <QMultiHash>
#include <QByteArray>
#include <QDebug>
#include <QVarLengthArray>

class SvgMetaDataCache {
public:
    explicit SvgMetaDataCache (void)
        : hasher (QCryptographicHash::Md5)
    {
        changeBasePath  (qApp->applicationDirPath ());
        changeCachePath (QStandardPaths::writableLocation (QStandardPaths::CacheLocation) % "/SvgIconsCache");
    }

    virtual ~SvgMetaDataCache (void) { }

    void changeBasePath (const QString & path) {
        basePath = path;
    }

    void changeCachePath (const QString & path) {
        cachePath = path;
        QDir ().mkpath (cachePath);
    }

    QString baseFile (const QString & name = "") {
        return (basePath % "/" % name);
    }

    QString cacheFile (const QString & name = "") {
        return (cachePath % "/" % name);
    }

    QString hashFile (const QString & filePath) {
        QString ret;
        QFile file (filePath);
        if (file.open (QFile::ReadOnly)) {
            hasher.reset ();
            hasher.addData (&file);
            ret = hasher.result ().toHex ();
            hasher.reset ();
            file.close ();
        }
        return ret;
    }

    QString hashData (const QByteArray & data) {
        QString ret;
        if (!data.isEmpty ()) {
            hasher.reset ();
            hasher.addData (data);
            ret = hasher.result ().toHex ();
            hasher.reset ();
        }
        return ret;
    }

    bool hasHashInIndex (const QString & hash) {
        return checksumsIndex.contains (hash);
    }

    void addHashInIndex (const QString & hash, const QString & checksum) {
        checksumsIndex.insert (hash, checksum);
    }

    void removeHashFromIndex (const QString & hash) {
        checksumsIndex.remove (hash);
    }

    QString readChecksumFile (const QString & filePath) {
        QString ret;
        QFile file (filePath);
        if (file.open (QFile::ReadOnly)) {
            ret = QString::fromLatin1 (file.readAll ());
            file.close ();
        }
        return ret;
    }

    void writeChecksumFile (const QString & filePath, const QString & checksum) {
        QFile file (filePath);
        if (file.open (QFile::WriteOnly)) {
            file.write (checksum.toLatin1 ());
            file.flush ();
            file.close ();
        }
    }

    bool renderSvgToPng (const QString & svgPath, const QString & pngPath, const QSize & size, const QColor & colorize) {
        bool ret = false;
        QImage image (size.width (), size.height (), QImage::Format_ARGB32);
        QPainter painter (&image);
        image.fill (Qt::transparent);
        painter.setRenderHint (QPainter::Antialiasing,            true);
        painter.setRenderHint (QPainter::SmoothPixmapTransform,   true);
        painter.setRenderHint (QPainter::TextAntialiasing, true);
        renderer.load (svgPath);
        if (renderer.isValid ()) {
            renderer.render (&painter);
            if (colorize.isValid () && colorize.alpha () > 0) {
                QColor tmp (colorize);
                for (int x (0); x < image.width (); x++) {
                    for (int y (0); y < image.height (); y++) {
                        tmp.setAlpha (qAlpha (image.pixel (x, y)));
                        image.setPixel (x, y, tmp.rgba ());
                    }
                }
            }
            ret = image.save (pngPath, "PNG", 0);
        }
        return ret;
    }

private:
    QString basePath;
    QString cachePath;
    QByteArray inotifyBuffer;
    QSvgRenderer renderer;
    QCryptographicHash hasher;
    QHash<QString, int> descriptorsIndex;
    QHash<QString, QString> checksumsIndex;
    QMultiHash<QString, QQuickSvgIconHelper *> svgHelpersIndex;
};

QQuickSvgIconHelper::QQuickSvgIconHelper (QObject * parent)
    : QObject           (parent)
    , m_size            (0)
    , m_ready           (false)
    , m_verticalRatio   (1.0)
    , m_horizontalRatio (1.0)
    , m_color           (Qt::transparent)
    , m_icon            (QString ())
    , m_inhibitTimer    (this)
{
    m_inhibitTimer.setInterval (50);
    m_inhibitTimer.setSingleShot (true);
    connect (&m_inhibitTimer, &QTimer::timeout, this, &QQuickSvgIconHelper::doProcessIcon, Qt::UniqueConnection);
}

QQuickSvgIconHelper::~QQuickSvgIconHelper (void) { }

SvgMetaDataCache & QQuickSvgIconHelper::cache (void) {
    static SvgMetaDataCache ret;
    return ret;
}

void QQuickSvgIconHelper::classBegin (void) {
    m_ready = false;
}

void QQuickSvgIconHelper::componentComplete (void) {
    m_ready = true;
    scheduleRefresh ();
}

void QQuickSvgIconHelper::setTarget (const QQmlProperty & target) {
    m_property = target;
    scheduleRefresh ();
}

void QQuickSvgIconHelper::setBasePath (const QString & basePath) {
    cache ().changeBasePath (basePath);
}

void QQuickSvgIconHelper::setCachePath (const QString & cachePath) {
    cache ().changeCachePath (cachePath);
}

int QQuickSvgIconHelper::getSize (void) const {
    return m_size;
}

qreal QQuickSvgIconHelper::getVerticalRatio (void) const {
    return m_verticalRatio;
}

qreal QQuickSvgIconHelper::getHorizontalRatio (void) const {
    return m_horizontalRatio;
}

const QColor & QQuickSvgIconHelper::getColor (void) const {
    return m_color;
}

const QString & QQuickSvgIconHelper::getIcon (void) const {
    return m_icon;
}

void QQuickSvgIconHelper::setSize (const int size) {
    if (m_size != size) {
        m_size = size;
        scheduleRefresh ();
        emit sizeChanged ();
    }
}

void QQuickSvgIconHelper::setVerticalRatio (const qreal ratio) {
    if (m_verticalRatio != ratio) {
        m_verticalRatio = ratio;
        scheduleRefresh ();
        emit verticalRatioChanged ();
    }
}

void QQuickSvgIconHelper::setHorizontalRatio (const qreal ratio) {
    if (m_horizontalRatio != ratio) {
        m_horizontalRatio = ratio;
        scheduleRefresh ();
        emit horizontalRatioChanged ();
    }
}

void QQuickSvgIconHelper::setColor (const QColor & color) {
    if (m_color != color) {
        m_color = color;
        scheduleRefresh ();
        emit colorChanged ();
    }
}

void QQuickSvgIconHelper::setIcon (const QString & icon) {
    if (m_icon != icon) {
        m_icon = icon;
        scheduleRefresh ();
        emit iconChanged ();
    }
}

void QQuickSvgIconHelper::scheduleRefresh (void) {
    m_inhibitTimer.stop ();
    m_inhibitTimer.start ();
}

void QQuickSvgIconHelper::doForceRegen (void) {
    if (!m_hash.isEmpty ()) {
        cache ().removeHashFromIndex (m_hash);
        scheduleRefresh ();
    }
}

void QQuickSvgIconHelper::doProcessIcon (void) {
    if (m_ready) {
        QUrl url;
        if (!m_icon.isEmpty () && m_size > 0 && m_horizontalRatio > 0.0 && m_verticalRatio > 0.0) {
            const QSize imgSize (int (m_size * m_horizontalRatio),
                                 int (m_size * m_verticalRatio));
            const QString sourcePath = (m_icon.startsWith ("file://")
                                        ? QUrl (m_icon).toLocalFile ()
                                        : (m_icon.startsWith ("qrc:/")
                                           ? QString (m_icon).replace (QRegularExpression ("qrc:/+"), ":/")
                                           : cache ().baseFile (m_icon % ".svg")));
            if (m_sourcePath != sourcePath) {
                m_sourcePath = sourcePath;
            }
            if (QFile::exists (m_sourcePath)) {
                m_hash = cache ().hashData (m_sourcePath.toLatin1 ());
                m_cachedPath = cache ().cacheFile (m_hash
                                                   % "_" % QString::number (imgSize.width ())
                                                   % "x" % QString::number (imgSize.height ())
                                                   % (m_color.alpha () > 0 ? m_color.name () : "")
                                                   % ".png");
                if (!cache ().hasHashInIndex (m_hash)) {
                    const QString checkumPath = cache ().cacheFile (m_hash % ".md5");
                    const QString reference = cache ().readChecksumFile (checkumPath);
                    const QString checksum  = cache ().hashFile (m_sourcePath);
                    if (reference != checksum) {
                        QDirIterator it (cache ().cacheFile (),
                                         QStringList (m_hash % "*.png"),
                                         QDir::Filters (QDir::Files | QDir::NoDotAndDotDot),
                                         QDirIterator::IteratorFlags (QDirIterator::NoIteratorFlags));
                        while (it.hasNext ()) {
                            QFile::remove (it.next ());
                        }
                        cache ().writeChecksumFile (checkumPath, checksum);
                    }
                    cache ().addHashInIndex (m_hash, checksum);
                }
                if (!QFile::exists (m_cachedPath)) {
                    cache ().renderSvgToPng (m_sourcePath, m_cachedPath, imgSize, m_color);
                }
                if (QFile::exists (m_cachedPath)) {
                    url = QUrl::fromLocalFile (m_cachedPath);
                }
            }
            else {
                qWarning () << ">>> QmlSvgIconHelper : Can't render" << m_sourcePath << ", no such file !";
            }
        }
        if (m_property.isValid () && m_property.isWritable ()) {
            m_property.write (QUrl ());
            m_property.write (url);
        }
    }
}
