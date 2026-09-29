#include <memory>
#include <stdexcept>

#include "lifecycle_node_impl.h"
#include "state_machine/lifecycle_node.h"

namespace lifecycle
{
  LifecycleNode::LifecycleNode(const std::string& node_name,
                               const rclcpp::NodeOptions& options)
      : Node(node_name, options),
        impl_(std::make_unique<LifecycleNodeImpl>(this))
  {
    registerOnConfigure(std::bind(&LifecycleNodeInterface::onConfigure, this,
                                  std::placeholders::_1));
    registerOnCleanup(std::bind(&LifecycleNodeInterface::onCleanup, this,
                                std::placeholders::_1));
    registerOnShutdown(std::bind(&LifecycleNodeInterface::onShutdown, this,
                                 std::placeholders::_1));
    registerOnActivate(std::bind(&LifecycleNodeInterface::onActivate, this,
                                 std::placeholders::_1));
    registerOnDeactivate(std::bind(&LifecycleNodeInterface::onDeactivate, this,
                                   std::placeholders::_1));
    registerOnError(std::bind(&LifecycleNodeInterface::onError, this,
                              std::placeholders::_1));

    // TODO
    // this->dewakeup();
    // enable_activate().store(false);
  }

  LifecycleNode::~LifecycleNode() = default;

  LifecycleState LifecycleNode::getCurrentState() const
  {
    return stateFromLabel(impl_->getCurrentState().label);
  }

  LifecycleState LifecycleNode::configure()
  {
    auto state = impl_->triggerTransition(
        lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE);
    return stateFromLabel(state.label);
  }

  LifecycleState LifecycleNode::configure(CallbackReturn& cb_return_code)
  {
    auto state = impl_->triggerTransition(
        lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE, cb_return_code);
    return stateFromLabel(state.label);
  }

  LifecycleState LifecycleNode::cleanup()
  {
    auto state = impl_->triggerTransition(
        lifecycle_msgs::msg::Transition::TRANSITION_CLEANUP);
    return stateFromLabel(state.label);
  }

  LifecycleState LifecycleNode::cleanup(CallbackReturn& cb_return_code)
  {
    auto state = impl_->triggerTransition(
        lifecycle_msgs::msg::Transition::TRANSITION_CLEANUP, cb_return_code);
    return stateFromLabel(state.label);
  }

  LifecycleState LifecycleNode::activate()
  {
    auto state = impl_->triggerTransition(
        lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE);
    return stateFromLabel(state.label);
  }

  LifecycleState LifecycleNode::activate(CallbackReturn& cb_return_code)
  {
    auto state = impl_->triggerTransition(
        lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE, cb_return_code);
    return stateFromLabel(state.label);
  }

  LifecycleState LifecycleNode::deactivate()
  {
    auto state = impl_->triggerTransition(
        lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
    return stateFromLabel(state.label);
  }

  LifecycleState LifecycleNode::deactivate(CallbackReturn& cb_return_code)
  {
    auto state = impl_->triggerTransition(
        lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE, cb_return_code);
    return stateFromLabel(state.label);
  }

  LifecycleState LifecycleNode::shutdown()
  {
    auto state = impl_->triggerTransition("shutdown");
    return stateFromLabel(state.label);
  }

  LifecycleState LifecycleNode::shutdown(CallbackReturn& cb_return_code)
  {
    auto state = impl_->triggerTransition("shutdown", cb_return_code);
    return stateFromLabel(state.label);
  }

  // ── 回调注册 ──────────────────────────────────────────────

  void LifecycleNode::registerOnConfigure(
      std::function<CallbackReturn(const LifecycleState&)> fcn)
  {
    impl_->registerCallback(
        lifecycle_msgs::msg::State::TRANSITION_STATE_CONFIGURING, fcn);
  }

  void LifecycleNode::registerOnCleanup(
      std::function<CallbackReturn(const LifecycleState&)> fcn)
  {
    impl_->registerCallback(
        lifecycle_msgs::msg::State::TRANSITION_STATE_CLEANINGUP, fcn);
  }

  void LifecycleNode::registerOnShutdown(
      std::function<CallbackReturn(const LifecycleState&)> fcn)
  {
    impl_->registerCallback(
        lifecycle_msgs::msg::State::TRANSITION_STATE_SHUTTINGDOWN, fcn);
  }

  void LifecycleNode::registerOnActivate(
      std::function<CallbackReturn(const LifecycleState&)> fcn)
  {
    impl_->registerCallback(
        lifecycle_msgs::msg::State::TRANSITION_STATE_ACTIVATING, fcn);
  }

  void LifecycleNode::registerOnDeactivate(
      std::function<CallbackReturn(const LifecycleState&)> fcn)
  {
    impl_->registerCallback(
        lifecycle_msgs::msg::State::TRANSITION_STATE_DEACTIVATING, fcn);
  }

  void LifecycleNode::registerOnError(
      std::function<CallbackReturn(const LifecycleState&)> fcn)
  {
    impl_->registerCallback(
        lifecycle_msgs::msg::State::TRANSITION_STATE_ERRORPROCESSING, fcn);
  }

}  // namespace lifecycle
