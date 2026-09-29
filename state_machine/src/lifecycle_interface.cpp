#include "state_machine/lifecycle_interface.h"

namespace lifecycle
{
  LifecycleNodeInterface::CallbackReturn LifecycleNodeInterface::onConfigure(
      const LifecycleState &)
  {
    return LifecycleNodeInterface::CallbackReturn::SUCCESS;
  }

  LifecycleNodeInterface::CallbackReturn LifecycleNodeInterface::onCleanup(
      const LifecycleState &)
  {
    return LifecycleNodeInterface::CallbackReturn::SUCCESS;
  }

  LifecycleNodeInterface::CallbackReturn LifecycleNodeInterface::onShutdown(
      const LifecycleState &)
  {
    return LifecycleNodeInterface::CallbackReturn::SUCCESS;
  }

  LifecycleNodeInterface::CallbackReturn LifecycleNodeInterface::onActivate(
      const LifecycleState &)
  {
    return LifecycleNodeInterface::CallbackReturn::SUCCESS;
  }

  LifecycleNodeInterface::CallbackReturn LifecycleNodeInterface::onDeactivate(
      const LifecycleState &)
  {
    return LifecycleNodeInterface::CallbackReturn::SUCCESS;
  }

  LifecycleNodeInterface::CallbackReturn LifecycleNodeInterface::onError(
      const LifecycleState &)
  {
    return LifecycleNodeInterface::CallbackReturn::SUCCESS;
  }

}  // namespace lifecycle
