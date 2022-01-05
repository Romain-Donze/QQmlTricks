#ifndef QMLENUMHELPERS_H
#define QMLENUMHELPERS_H

#include <QObject>
#include <QString>
#include <QQmlEngine>
#include <QMetaEnum>
#include <QMetaObject>
#include <qqml.h>

#ifndef Q_ENUM
#   define Q_ENUM Q_ENUMS
#endif

#define QML_ENUM_CLASS(NAME, ...) \
    struct NAME { \
        Q_GADGET \
        public: \
            enum Type { __VA_ARGS__ }; Q_ENUM (Type) \
            static QByteArray asByteArray (const int value) { \
                return QByteArray (staticMetaObject.enumerator (0).valueToKey (value)); \
            } \
            static QString asString (const int value) { \
                return QString::fromLatin1 (asByteArray (value)); \
            } \
            static Type fromByteArray (const QByteArray & str, bool * ok = nullptr) { \
                return NAME::Type (staticMetaObject.enumerator (0).keyToValue (str.constData (), ok)); \
            } \
            static Type fromString (const QString & str, bool * ok = nullptr) { \
                return fromByteArray (str.toLatin1 (), ok); \
            } \
        private: \
            NAME (void) = delete; \
            NAME (const NAME &) = delete; \
            NAME & operator= (const NAME &) = delete; \
    }; \
    Q_DECLARE_METATYPE (NAME::Type)

QML_ENUM_CLASS (_Test_QmlEnumClass_) // NOTE : to avoid "no suitable class found" MOC note

#endif // QMLENUMHELPERS_H
