#include "types.h"

#include <qmetaobject.h>
#include <QRegularExpression>

QString roleToString(int role) {
    return (role <= Qt::UserRole) ? QMetaEnum::fromType<Qt::ItemDataRole>().valueToKey(role) : QMetaEnum::fromType<UserType::ERoles>().valueToKey(role) ;
}
