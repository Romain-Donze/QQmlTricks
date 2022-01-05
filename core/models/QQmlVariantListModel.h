#ifndef QQMLVARIANTLISTMODEL_H
#define QQMLVARIANTLISTMODEL_H

#include <QObject>
#include <QVariant>
#include <QHash>
#include <QList>
#include <QAbstractListModel>
#include <QDebug>

class QQmlVariantListModelBase : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY (int size   READ count NOTIFY countChanged)
    Q_PROPERTY (int count  READ count NOTIFY countChanged)
    Q_PROPERTY (int length READ count NOTIFY countChanged)

public:
    explicit QQmlVariantListModelBase (QObject * parent = nullptr) : QAbstractListModel { parent } { }
    virtual ~QQmlVariantListModelBase (void) { }

    virtual int count (void) const = 0;

    Q_INVOKABLE virtual bool contains (const QVariant & value) const = 0;

    Q_INVOKABLE virtual QVariant get (const int idx) const = 0;

signals:
    void countChanged (void);
};

template<typename T> class QQmlVariantListModel : public QQmlVariantListModelBase {
public:
    explicit QQmlVariantListModel (std::initializer_list<T> list, QObject * parent = nullptr)
        : QQmlVariantListModelBase { parent }
        , m_items { list }
    { }
    explicit QQmlVariantListModel (const QList<T> & list, QObject * parent = nullptr)
        : QQmlVariantListModelBase { parent }
        , m_items { list }
    { }
    explicit QQmlVariantListModel (QObject * parent = nullptr)
        : QQmlVariantListModel { { }, parent }
    { }
    virtual ~QQmlVariantListModel (void) { }

    using const_iterator = typename QList<T>::const_iterator;
    inline const_iterator begin      (void) const { return m_items.constBegin (); }
    inline const_iterator end        (void) const { return m_items.constEnd ();   }
    inline const_iterator constBegin (void) const { return m_items.constBegin (); }
    inline const_iterator constEnd   (void) const { return m_items.constEnd ();   }

    QVariant data (const QModelIndex & index, const int role) const final {
        QVariant ret { };
        if (!index.parent ().isValid () &&
            index.column () == 0 &&
            index.row () >= 0 &&
            index.row () < m_items.count () &&
            role == Qt::UserRole) {
            ret.setValue (m_items.at (index.row ()));
        }
        return ret;
    }
    bool setData (const QModelIndex & index, const QVariant & data, const int role) final {
        bool ret { false };
        if (!index.parent ().isValid () &&
            index.column () == 0 &&
            index.row () >= 0 &&
            index.row () < m_items.count () &&
            role == Qt::UserRole) {
            m_items.replace (index.row (), data.value<T> ());
            emit dataChanged (index, index, QVector<int> { });
            ret = true;
        }
        return ret;
    }
    int rowCount (const QModelIndex & parent) const final {
        return (!parent.isValid () ? m_items.count () : 0);
    }
    QHash<int, QByteArray> roleNames (void) const final {
        static const QHash<int, QByteArray> ret {
            { Qt::UserRole, QByteArrayLiteral ("modelData") },
        };
        return ret;
    }
    int count (void) const final {
        return m_items.count ();
    }
    int indexOf (const T & item, const int from = 0) const {
        return m_items.indexOf (item, from);
    }
    int lastIndexOf (const T & item, const int from = -1) const {
        return m_items.lastIndexOf (item, from);
    }
    bool isEmpty (void) const {
        return m_items.isEmpty ();
    }
    bool contains (const T & item) const {
        return m_items.contains (item);
    }
    void append (const T & item) {
        beginInsertRows (ROOT (), m_items.count (), m_items.count ());
        m_items.append (item);
        endInsertRows ();
        emit countChanged ();
    }
    void append (const QList<T> & itemsList) {
        if (!itemsList.isEmpty ()) {
            beginInsertRows (ROOT (), m_items.count (), m_items.count () + itemsList.count () -1);
            for (const T & item : itemsList) {
                m_items.append (item);
            }
            endInsertRows ();
            emit countChanged ();
        }
    }
    void prepend (const T & item) {
        beginInsertRows (ROOT (), 0, 0);
        m_items.prepend (item);
        endInsertRows ();
        emit countChanged ();
    }
    void insert (const int idx, const T & item) {
        if (idx >= 0 && idx <= m_items.count ()) {
            beginInsertRows (ROOT (), idx, idx);
            m_items.insert (idx, item);
            endInsertRows ();
            emit countChanged ();
        }
    }
    void replace (const int idx, const T & item) {
        if (idx >= 0 && idx <= m_items.count ()) {
            m_items.replace (idx, item);
            emit dataChanged (index (idx), index (idx), QVector<int> { });
        }
    }
    void remove (const int idx) {
        if (idx >= 0 && idx < m_items.count ()) {
            beginRemoveRows (ROOT (), idx, idx);
            m_items.removeAt (idx);
            endRemoveRows ();
            emit countChanged ();
        }
    }
    void clear (void) {
        if (!m_items.isEmpty ()) {
            beginResetModel ();
            m_items.clear ();
            endResetModel ();
            emit countChanged ();
        }
    }
    const T & at (const int idx) const {
        return (idx >= 0 && idx < m_items.count () ? m_items.at (idx) : EMPTY ());
    }
    QVariant get (const int idx) const final {
        return QVariant::fromValue (at (idx));
    }
    bool contains (const QVariant & var) const final {
      return (m_items.contains (var.value<T> ()));
    }
    const QList<T> & toList (void) const {
        return m_items;
    }

protected:
    static const QModelIndex & ROOT (void) {
        static const QModelIndex ret { };
        return ret;
    }
    static const T & EMPTY (void) {
        static const T ret { };
        return ret;
    }

private:
    QList<T> m_items;
};

#define QML_VARMODEL_PROPERTY(NAME, T) \
    private: Q_PROPERTY (QQmlVariantListModelBase * NAME READ get_##NAME CONSTANT) \
    public: QQmlVariantListModel<T> * get_##NAME (void) { return &m_##NAME; } \
    private: QQmlVariantListModel<T> m_##NAME;

#endif // QQMLVARIANTLISTMODEL_H
