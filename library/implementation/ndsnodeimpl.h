#ifndef NDSNODEIMPL_H
#define NDSNODEIMPL_H

#include "ndsbaseimpl.h"
#include "../include/nds3/definitions.h"
#include <list>

namespace nds
{

class Node;
class StateMachineImpl;

/**
 * @brief Represents a node (channel or channelGroup in the old NDS) which can contain
 *        other nodes.
 *
 * Can contain other nodes (including AsynPort) or PVs.
 */
class NodeImpl: public BaseImpl
{
public:
    /**
     * @brief Construct the node
     *
     * @param name  node's name (e.g. "CHANNEL0" or "CHANNELGROUP1")
     */
    NodeImpl(const std::string& name);

    void addChild(std::shared_ptr<BaseImpl> pChild);

    virtual void initialize();

    virtual state_t getLocalState();
    virtual state_t getGlobalState();
    state_t getChildrenState();


private:
    typedef std::map<std::string, std::shared_ptr<BaseImpl> > tChildren;
    tChildren m_children;

    std::shared_ptr<StateMachineImpl> m_pStateMachine;
};

}

#endif // NDSNODEIMPL_H