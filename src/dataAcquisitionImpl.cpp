/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include "../include/nds3impl/dataAcquisitionImpl.h"
#include "../include/nds3impl/stateMachineImpl.h"
#include "../include/nds3impl/pvVariableInImpl.h"
#include "../include/nds3impl/pvVariableOutImpl.h"
#include "../include/nds3/definitions.h"

namespace nds
{

template<typename T>
DataAcquisitionImpl<T>::DataAcquisitionImpl(
        const std::string& name,
        size_t maxElements,
        stateChange_t switchOnFunction,
        stateChange_t switchOffFunction,
        stateChange_t startFunction,
        stateChange_t stopFunction,
        stateChange_t recoverFunction,
        allowChange_t allowStateChangeFunction):
    NodeImpl(name, nodeType_t::dataSourceChannel),
    m_onStartDelegate(startFunction),
    m_startTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
{
    // Add the children PVs
    m_dataPV.reset(new PVVariableInImpl<T>("Data"));
    m_dataPV->setMaxElements(maxElements);
    m_dataPV->setDescription("Acquired data");
    m_dataPV->setScanType(scanType_t::interrupt, 0);
    addChild(m_dataPV);

    m_frequencyPV.reset(new PVVariableOutImpl<double>("Frequency"));
    m_frequencyPV->setDescription("Acquisition frequency");
    m_frequencyPV->setScanType(scanType_t::passive, 0);
    m_frequencyPV->write(getTimestamp(), (double)1);
    addChild(m_frequencyPV);

    m_durationPV.reset(new PVVariableOutImpl<double>("Duration"));
    m_durationPV->setDescription("Acquisition duration");
    m_durationPV->setScanType(scanType_t::passive, 0);
    addChild(m_durationPV);

    m_decimationPV.reset(new PVVariableOutImpl<std::int32_t>("decimation"));
    m_decimationPV->setDescription("Decimation");
    m_decimationPV->setScanType(scanType_t::passive, 0);
    m_decimationPV->write(getTimestamp(), (std::int32_t)1);
    addChild(m_decimationPV);

    // Add state machine
    m_stateMachine.reset(new StateMachineImpl(true,
                                   switchOnFunction,
                                   switchOffFunction,
                                   std::bind(&DataAcquisitionImpl::onStart, this),
                                   stopFunction,
                                   recoverFunction,
                                   allowStateChangeFunction));
    addChild(m_stateMachine);
}

template<typename T>
double DataAcquisitionImpl<T>::getFrequencyHz()
{
    double frequency;
    timespec timestamp;
    m_frequencyPV->read(&timestamp, &frequency);
    return frequency;
 }

template<typename T>
double DataAcquisitionImpl<T>::getDurationSeconds()
{
    double duration;
    timespec timestamp;
    m_durationPV->read(&timestamp, &duration);
    return duration;
}

template<typename T>
size_t DataAcquisitionImpl<T>::getMaxElements()
{
    return m_dataPV->getMaxElements();
}

template<typename T>
size_t DataAcquisitionImpl<T>::getDecimation()
{
    std::int32_t decimation;
    timespec timestamp;
    m_decimationPV->read(&timestamp, &decimation);
    return (size_t)decimation;
}

template<typename T>
timespec DataAcquisitionImpl<T>::getStartTimestamp() const
{
    return m_startTime;
}

template<typename T>
void DataAcquisitionImpl<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    m_startTimestampFunction = timestampDelegate;
}

template<typename T>
void DataAcquisitionImpl<T>::push(const timespec& timestamp, const T& data)
{
    m_dataPV->push(timestamp, data);
}

template<typename T>
void DataAcquisitionImpl<T>::onStart()
{
    m_startTime = m_startTimestampFunction();
    m_dataPV->setDecimation((std::uint32_t)(m_decimationPV->getValue()));
    m_onStartDelegate();
}


template class DataAcquisitionImpl<std::int32_t>;
template class DataAcquisitionImpl<double>;
template class DataAcquisitionImpl<std::vector<std::int8_t> >;
template class DataAcquisitionImpl<std::vector<std::uint8_t> >;
template class DataAcquisitionImpl<std::vector<std::int32_t> >;
template class DataAcquisitionImpl<std::vector<double> >;
template class DataAcquisitionImpl<std::string >;


}
