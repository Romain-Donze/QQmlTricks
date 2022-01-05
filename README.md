http://gitlab.unique-conception.org/qt-qml-tricks/ui-elements-ng

Qt/QML Tricks library (next-gen)
================================


## Macros for C++11 (exposable to QML 2.x)

A set of one-line C++ macros to simplify the creation of reccurent things (like Qt Meta Properties) so that doing them in C++ is not harder than in QML, and requires no boilerplate glue-code.


### For one-line creation of QML properties

* `QML_WRITABLE_***_PROPERTY` : a macro that takes a type and a name, and creates automatically the member attribute, the public getter and setter, and the Qt signal for notifier, and allow use in QML by exposing a read/write `Q_PROPERTY`.

* `QML_READONLY_***_PROPERTY` : another macro that does almost the same as `QML_WRITABLE_PROPERTY` except that the property is not modifiable from the QML side, only C++ can access the setter.

* `QML_CONSTANT_***_PROPERTY` : a simplified version of the previous macros, that exposes a constant property with no getter and no setter, from C++ or QML side.

The `***` can be either `VAR`, `PTR`, `CSTREF`. The three first are simple macros that you use by simply passing the non-qualified type (`T`) and it'll add the qualifiers for var (none), pointer (`*`), or constant-reference (`const &`) where needed.


### For simple enum class that can be used in C++ and QML

* `QML_ENUM_CLASS` : a macro to declare a "gadget" struct that only contains a `Q_ENUM` and can be exposed as is to QML. It can be used with very similar syntax from both C++ and QML (`<Name>::<Key>` in C++ vs `<Name>.<Key>` in QML/JS).


## Models for C++11 (exposable to QML 2.x)

Additional data models aimed to bring more power to QML applications by using useful C++ models in back-end.

* `QQmlObjectListModel` : a much nicer way to expose C++ list to QML than the quick & dirty `QList<QObject*>` property . Supports all the strong model features of `QAbstractListModel` while showing the simple and well know API of QList.


## Extra anchors for QtQuick 2.x

* `ExtraAnchors` : an attached object which adds a lot of new convenience anchors (these anchors are only oneliners for things that can be acheived with classic anchors in multiple lines). Basically, it allows to anchors left/right, or top/bottom, or top/left/right, or top/left, etc, in a given object (parent or sibling).


## Containers for QtQuick 2.x

These are invisible items that resize and reposition their children according to predefined behavior :

* `RowContainer` : an horizontal layout, that positions its childrens side-by-side, setting their width in consequence of their implicit width hint (or `Container.forcedWidth` attached property with non-0 value), and using remaining space in the layout to distribute it between all items that use `Container.horizontalStretch` attached property with non-0 value.

* `ColumnContainer` : an vertical layout, that positions its childrens one under the other, setting their height in consequence of their implicit height hint (or `Container.forcedHeight` attached property with non-0 value), and using remaining space in the layout to distribute it between all items that use `Container.verticalStretch` attached property with non-0 value.

* `GridContainer` : a smart grid that dimensions itself according to the sum/max of its children's implicit size hints, and then distributes regularly the available space between all children, positioned against a column/row model.

> NOTE : all those containers can change their spacings, orientation, and understand the use of `Container.ignored` for children which don't want to be layouted.
