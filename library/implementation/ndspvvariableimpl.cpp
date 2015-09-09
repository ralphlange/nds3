#include "ndspvvariableimpl.h"

namespace nds
{

template <typename T>
PVVariableImpl<T>::PVVariableImpl(const std::string& name): PVBaseImpl(name)
{
    m_timestamp.tv_sec = 0;
    m_timestamp.tv_nsec = 0;

}

template <typename T>
void PVVariableImpl<T>::read(timespec* pTimestamp, T* pValue)
{
    *pValue = m_value;
    *pTimestamp = m_timestamp;
}

template <typename T>
void PVVariableImpl<T>::write(const timespec timestamp, const T& value)
{
    m_value =value;
    m_timestamp = timestamp;
}

template <typename T>
dataType_t PVVariableImpl<T>::getDataType()
{
    return getDataTypeForCPPType<T>();
}

template class PVVariableImpl<std::int32_t>;
template class PVVariableImpl<double>;
template class PVVariableImpl<std::vector<std::int32_t> >;

}