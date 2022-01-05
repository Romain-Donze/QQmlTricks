#ifndef QQMLLISTPROPERTYHELPER
#define QQMLLISTPROPERTYHELPER

#include <QVector>
#include <QQmlListProperty>

template<class T> class QQmlSmartListWrapper : public QQmlListProperty<T> {
public:
    using cpp_list_type = QVector<T *>;
    using qml_list_type = QQmlListProperty<T>;

    explicit QQmlSmartListWrapper (QObject * object, const int reserve = 0)
        : qml_list_type {
            object,
            &m_items,
            [] (qml_list_type * prop, T * obj) {
                static_cast<cpp_list_type *> (prop->data)->append (obj);
            },
            [] (qml_list_type * prop) -> int {
                return static_cast<cpp_list_type *> (prop->data)->count ();
            },
            [] (qml_list_type * prop, int idx) -> T * {
                return static_cast<cpp_list_type *> (prop->data)->at (idx);
            },
            [] (qml_list_type * prop) {
                static_cast<cpp_list_type *> (prop->data)->clear ();
            }
        }
{
    if (reserve > 0) {
        m_items.reserve (reserve);
    }
}

    using const_iterator = typename cpp_list_type::const_iterator;

    inline const_iterator begin      (void) const { return m_items.begin ();      }
    inline const_iterator end        (void) const { return m_items.end ();        }
    inline const_iterator constBegin (void) const { return m_items.constBegin (); }
    inline const_iterator constEnd   (void) const { return m_items.constEnd ();   }

    inline int length (void) const { return m_items.length (); }

    inline T * operator[] (const int idx) const {
        return (idx >= 0 && idx < m_items.length () ? m_items.at (idx) : nullptr);
    }

    inline cpp_list_type toVector (void) const { return m_items; }

private:
    cpp_list_type m_items;
};

#define QML_LIST_PROPERTY(NAME, TYPE) \
    private: Q_PROPERTY (QQmlListProperty<TYPE> NAME READ get_##NAME CONSTANT) \
    public: const QQmlSmartListWrapper<TYPE> & get_##NAME (void) const { return m_##NAME; } \
    private: QQmlSmartListWrapper<TYPE> m_##NAME;

#define QML_DEFAULT_PROPERTY(NAME) \
    private: Q_CLASSINFO ("DefaultProperty", #NAME)

#endif // QQMLLISTPROPERTYHELPER
