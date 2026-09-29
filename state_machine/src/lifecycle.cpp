#include "state_machine/lifecycle.h"
#include <unordered_map>

namespace lifecycle
{
  LifecycleState stateFromLabel(const std::string& label)
  {
    static const std::unordered_map<std::string, LifecycleState> labelToState{
        {"unknown", LifecycleState::PRIMARY_STATE_UNKNOWN},
        {"unconfigured", LifecycleState::PRIMARY_STATE_UNCONFIGURED},
        {"inactive", LifecycleState::PRIMARY_STATE_INACTIVE},
        {"active", LifecycleState::PRIMARY_STATE_ACTIVE},
        {"finalized", LifecycleState::PRIMARY_STATE_FINALIZED},
        {"configuring", LifecycleState::TRANSITION_STATE_CONFIGURING},
        {"cleaningup", LifecycleState::TRANSITION_STATE_CLEANINGUP},
        {"shuttingdown", LifecycleState::TRANSITION_STATE_SHUTTINGDOWN},
        {"activating", LifecycleState::TRANSITION_STATE_ACTIVATING},
        {"deactivating", LifecycleState::TRANSITION_STATE_DEACTIVATING},
        {"errorprocessing", LifecycleState::TRANSITION_STATE_ERRORPROCESSING},
    };

    auto it = labelToState.find(label);
    if (it != labelToState.end())
    {
      return it->second;
    }

    return LifecycleState::PRIMARY_STATE_UNKNOWN;
  };
}  // namespace lifecycle
