#ifndef QQMLFASTOBJECTLISTMODEL_H
#define QQMLFASTOBJECTLISTMODEL_H

#include <QObject>
#include <QList>

class QQmlFastObjectListModelBase : public QObject {
    Q_OBJECT
    Q_PROPERTY (int       count     READ getCount     NOTIFY countChanged)
    Q_PROPERTY (QObject * firstItem READ getFirstItem NOTIFY firstChanged)
    Q_PROPERTY (QObject * lastItem  READ getLastItem  NOTIFY lastChanged)

public:
    explicit QQmlFastObjectListModelBase (QObject * parent = Q_NULLPTR) : QObject { parent } { }
    virtual ~QQmlFastObjectListModelBase (void) { }

    Q_INVOKABLE virtual int       getCount     (void)      const { return 0; }
    Q_INVOKABLE virtual bool      getContains  (QObject *) const { return false; }
    Q_INVOKABLE virtual QObject * getFirstItem (void)      const { return Q_NULLPTR; }
    Q_INVOKABLE virtual QObject * getLastItem  (void)      const { return Q_NULLPTR; }
    Q_INVOKABLE virtual QObject * getItem      (const int) const { return Q_NULLPTR; }

signals:
    void countChanged (void);
    void firstChanged (void);
    void lastChanged  (void);
    void itemsCleared (void);
    void itemInserted (QObject * item, const int idx);
    void itemRemoved  (QObject * item, const int idx);
};

template<class T> class QQmlFastObjectListModel : public QQmlFastObjectListModelBase {
private:
    QList<T *> m_itemsList { };

public:
    explicit QQmlFastObjectListModel (QObject * parent = Q_NULLPTR) : QQmlFastObjectListModelBase { parent } { }
    virtual ~QQmlFastObjectListModel (void) { }

    using Iterator = typename QList<T *>::const_iterator;
    Iterator begin (void) const {
        return m_itemsList.constBegin ();
    }
    Iterator end (void) const {
        return m_itemsList.constEnd ();
    }

    int count (void) const {
        return m_itemsList.count ();
    }
    int indexOf (T * item) const {
        return (item ? m_itemsList.indexOf (item) : -1);
    }

    bool isEmpty (void) const {
        return m_itemsList.isEmpty ();
    }
    bool contains (T * item) const {
        return (item != Q_NULLPTR && m_itemsList.contains (item));
    }

    T * getFirst (void) const {
        return (!isEmpty () ? m_itemsList.first () : Q_NULLPTR);
    }
    T * getLast  (void) const {
        return (!isEmpty () ? m_itemsList.last () : Q_NULLPTR);
    }
    T * getAt (const int idx) const {
        return (idx >= 0 && idx < count () ? m_itemsList.at (idx) : Q_NULLPTR);
    }

    void append (T * item) {
        if (item != Q_NULLPTR && !contains (item)) {
            const int idx { m_itemsList.count () };
            m_itemsList.append (item);
            emit itemInserted (item, idx);
            emit lastChanged ();
            if (count () == 1) {
                emit firstChanged ();
            }
            emit countChanged ();
        }
    }
    void appendList (const QList<T *> itemsList) {
        if (!itemsList.isEmpty ()) {
            for (T * item : itemsList) {
                int idx { m_itemsList.count () };
                if (item != Q_NULLPTR && !contains (item)) {
                    m_itemsList.append (item);
                    emit itemInserted (item, idx);
                    if (count () == 1) {
                        emit firstChanged ();
                    }
                    ++idx;
                }
            }
            emit lastChanged ();
            emit countChanged ();
        }
    }
    void prepend (T * item) {
        if (item != Q_NULLPTR && !contains (item)) {
            const int idx { 0 };
            m_itemsList.prepend (item);
            emit itemInserted (item, idx);
            emit firstChanged ();
            if (count () == 1) {
                emit lastChanged ();
            }
            emit countChanged ();
        }
    }
    void insert (T * item, const int idx) {
        if (item != Q_NULLPTR && !contains (item)) {
            m_itemsList.insert (idx, item);
            emit itemInserted (item, idx);
            if (idx == 0) {
                emit firstChanged ();
            }
            if (idx == count ()) {
                emit lastChanged ();
            }
            emit countChanged ();
        }
    }
    void remove (T * item) {
        if (item != Q_NULLPTR) {
            const int idx { m_itemsList.indexOf (item) };
            if (idx >= 0 && idx < count ()) {
                m_itemsList.removeAt (idx);
                emit itemRemoved (item, idx);
                if (idx == 0) {
                    emit firstChanged ();
                }
                if (idx == count ()) {
                    emit lastChanged ();
                }
                emit countChanged ();
            }
        }
    }
    void clear (void) {
        m_itemsList.clear ();
        emit itemsCleared ();
        emit firstChanged ();
        emit lastChanged  ();
        emit countChanged ();
    }

protected: // API for QQuickFastObjectListView only
    int getCount (void) const Q_DECL_FINAL {
        return count ();
    }
    bool getContains (QObject * item) const Q_DECL_FINAL {
        return contains (qobject_cast<T *> (item));
    }
    QObject * getItem (const int idx) const Q_DECL_FINAL {
        return getAt (idx);
    }
    QObject * getFirstItem (void) const Q_DECL_FINAL {
        return getFirst ();
    }
    QObject * getLastItem  (void) const Q_DECL_FINAL {
        return getLast ();
    }
};

#define QML_FASTOBJMODEL_PROPERTY(NAME,TYPE) \
    private: Q_PROPERTY (QQmlFastObjectListModelBase * NAME READ get_##NAME CONSTANT) \
    public: QQmlFastObjectListModel<TYPE> NAME { }; \
    public: QQmlFastObjectListModelBase * get_##NAME (void) { return &NAME; } \
    private:

#endif // QQMLFASTOBJECTLISTMODEL_H
