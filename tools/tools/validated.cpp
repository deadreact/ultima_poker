#include "validated.h"
#include "QtGlobal"

//#include "../../poker_client/src/interface/initable.h"


//template <> CountingObject Validated<Initable>::data = CountingObject();

CountingObject::~CountingObject() {
    Q_ASSERT((m_objCreated == m_objDeleted) && (m_memAllocated == m_memDeallocated));
}

