#include "state_machine/lifecycle_node_interface.h"

namespace ifs
{
  LifecycleNodeInterface::CallbackReturn LifecycleNodeInterface::onConfigure(
      const LifecycleState &, const std::string &)
  {
    return LifecycleNodeInterface::CallbackReturn::SUCCESS;
  }

  LifecycleNodeInterface::CallbackReturn LifecycleNodeInterface::onCleanup(
      const LifecycleState &, const std::string &)
  {
    return LifecycleNodeInterface::CallbackReturn::SUCCESS;
  }

  LifecycleNodeInterface::CallbackReturn LifecycleNodeInterface::onShutdown(
      const LifecycleState &, const std::string &)
  {
    return LifecycleNodeInterface::CallbackReturn::SUCCESS;
  }

  LifecycleNodeInterface::CallbackReturn LifecycleNodeInterface::onActivate(
      const LifecycleState &, const std::string &)
  {
    return LifecycleNodeInterface::CallbackReturn::SUCCESS;
  }

  LifecycleNodeInterface::CallbackReturn LifecycleNodeInterface::onDeactivate(
      const LifecycleState &, const std::string &)
  {
    return LifecycleNodeInterface::CallbackReturn::SUCCESS;
  }

  LifecycleNodeInterface::CallbackReturn LifecycleNodeInterface::onError(
      const LifecycleState &, const std::string &)
  {
    return LifecycleNodeInterface::CallbackReturn::SUCCESS;
  }

}  // namespace ifs
