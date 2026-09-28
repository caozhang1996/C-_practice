#include "state_machine/lifecycle_interface.h"

namespace ifs
{
  LifecycleNodeInterface::CallbackReturn LifecycleNodeInterface::onConfigure(
      const detail::LifecycleState &)
  {
    return LifecycleNodeInterface::CallbackReturn::SUCCESS;
  }

  LifecycleNodeInterface::CallbackReturn LifecycleNodeInterface::onCleanup(
      const detail::LifecycleState &)
  {
    return LifecycleNodeInterface::CallbackReturn::SUCCESS;
  }

  LifecycleNodeInterface::CallbackReturn LifecycleNodeInterface::onShutdown(
      const detail::LifecycleState &)
  {
    return LifecycleNodeInterface::CallbackReturn::SUCCESS;
  }

  LifecycleNodeInterface::CallbackReturn LifecycleNodeInterface::onActivate(
      const detail::LifecycleState &)
  {
    return LifecycleNodeInterface::CallbackReturn::SUCCESS;
  }

  LifecycleNodeInterface::CallbackReturn LifecycleNodeInterface::onDeactivate(
      const detail::LifecycleState &)
  {
    return LifecycleNodeInterface::CallbackReturn::SUCCESS;
  }

  LifecycleNodeInterface::CallbackReturn LifecycleNodeInterface::onError(
      const detail::LifecycleState &)
  {
    return LifecycleNodeInterface::CallbackReturn::SUCCESS;
  }

}  // namespace ifs
