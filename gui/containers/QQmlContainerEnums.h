#ifndef QQMLCONTAINERENUMS_H
#define QQMLCONTAINERENUMS_H

#include <QObject>

#include "QmlEnumHelpers.h"

QML_ENUM_CLASS (VerticalDirections,
                TOP_TO_BOTTOM,
                BOTTOM_TO_TOP,
                )

QML_ENUM_CLASS (HorizontalDirections,
                LEFT_TO_RIGHT,
                RIGHT_TO_LEFT,
                )

QML_ENUM_CLASS (FlowDirections,
                VERTICAL,
                HORIZONTAL,
                )

struct _QQMLCONTAINERENUMS_ { Q_GADGET }; // mock object

#endif // QQMLCONTAINERENUMS_H
