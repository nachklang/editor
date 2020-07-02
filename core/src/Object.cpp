#include "Object.h"

bool operator==(const Object &lhs, const Object &rhs)
{
    return lhs.m_name == rhs.m_name &&
            lhs.m_iconName == rhs.m_iconName &&
            lhs.m_singlePlaceable == rhs.m_singlePlaceable/* &&
                    m_properties == other.m_properties;*/;
}
