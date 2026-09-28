#include <stdexcept>

#include "lifecycle_node_impl.h"

namespace lifecycle
{
  // ═══════════════════════════════════════════════════════════
  // stateFromLabel
  // ═══════════════════════════════════════════════════════════

  detail::LifecycleState stateFromLabel(const std::string& label)
  {
    static const std::pair<const char*, unsigned int> kMap[] = {
        {"unknown", lifecycle::state::PRIMARY_STATE_UNKNOWN},
        {"unconfigured", lifecycle::state::PRIMARY_STATE_UNCONFIGURED},
        {"inactive", lifecycle::state::PRIMARY_STATE_INACTIVE},
        {"active", lifecycle::state::PRIMARY_STATE_ACTIVE},
        {"finalized", lifecycle::state::PRIMARY_STATE_FINALIZED},
        {"configuring", lifecycle::state::TRANSITION_STATE_CONFIGURING},
        {"cleaningup", lifecycle::state::TRANSITION_STATE_CLEANINGUP},
        {"shuttingdown", lifecycle::state::TRANSITION_STATE_SHUTTINGDOWN},
        {"activating", lifecycle::state::TRANSITION_STATE_ACTIVATING},
        {"deactivating", lifecycle::state::TRANSITION_STATE_DEACTIVATING},
        {"errorprocessing", lifecycle::state::TRANSITION_STATE_ERRORPROCESSING},
    };

    for (const auto& entry : kMap)
    {
      if (label == entry.first)
      {
        return detail::LifecycleState(label, entry.second);
      }
    }

    return detail::LifecycleState("unknown", lifecycle::state::PRIMARY_STATE_UNKNOWN);
  }

  // ═══════════════════════════════════════════════════════════
  // LifecycleNode
  // ═══════════════════════════════════════════════════════════

  LifecycleNode::LifecycleNode() : impl_(std::make_unique<Impl>())
  {
    // 将虚函数回调注册到 cb_map_
    registerOnConfigure(
        [this](const detail::LifecycleState& s) { return this->onConfigure(s); });
    registerOnCleanup(
        [this](const detail::LifecycleState& s) { return this->onCleanup(s); });
    registerOnShutdown(
        [this](const detail::LifecycleState& s) { return this->onShutdown(s); });
    registerOnActivate(
        [this](const detail::LifecycleState& s) { return this->onActivate(s); });
    registerOnDeactivate(
        [this](const detail::LifecycleState& s) { return this->onDeactivate(s); });
    registerOnError(
        [this](const detail::LifecycleState& s) { return this->onError(s); });
  }

  LifecycleNode::~LifecycleNode() = default;

  detail::LifecycleState LifecycleNode::getCurrentState() const
  {
    return stateFromLabel(impl_->getCurrentState().label);
  }

  detail::LifecycleState LifecycleNode::configure()
  {
    auto state = impl_->triggerTransition(
        lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE);
    return stateFromLabel(state.label);
  }

  detail::LifecycleState LifecycleNode::configure(CallbackReturn& cb_return_code)
  {
    auto state = impl_->triggerTransition(
        lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE, cb_return_code);
    return stateFromLabel(state.label);
  }

  detail::LifecycleState LifecycleNode::cleanup()
  {
    auto state = impl_->triggerTransition(
        lifecycle_msgs::msg::Transition::TRANSITION_CLEANUP);
    return stateFromLabel(state.label);
  }

  detail::LifecycleState LifecycleNode::cleanup(CallbackReturn& cb_return_code)
  {
    auto state = impl_->triggerTransition(
        lifecycle_msgs::msg::Transition::TRANSITION_CLEANUP, cb_return_code);
    return stateFromLabel(state.label);
  }

  detail::LifecycleState LifecycleNode::activate()
  {
    auto state = impl_->triggerTransition(
        lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE);
    return stateFromLabel(state.label);
  }

  detail::LifecycleState LifecycleNode::activate(CallbackReturn& cb_return_code)
  {
    auto state = impl_->triggerTransition(
        lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE, cb_return_code);
    return stateFromLabel(state.label);
  }

  detail::LifecycleState LifecycleNode::deactivate()
  {
    auto state = impl_->triggerTransition(
        lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
    return stateFromLabel(state.label);
  }

  detail::LifecycleState LifecycleNode::deactivate(CallbackReturn& cb_return_code)
  {
    auto state = impl_->triggerTransition(
        lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE, cb_return_code);
    return stateFromLabel(state.label);
  }

  detail::LifecycleState LifecycleNode::shutdown()
  {
    // 从当前状态决定用哪个 shutdown transition
    auto& current = impl_->getCurrentState();
    uint8_t transition_id;

    if (current.id == lifecycle::state::PRIMARY_STATE_UNCONFIGURED)
    {
      transition_id = lifecycle_msgs::msg::Transition::TRANSITION_UNCONFIGURED_SHUTDOWN;
    }
    else if (current.id == lifecycle::state::PRIMARY_STATE_INACTIVE)
    {
      transition_id = lifecycle_msgs::msg::Transition::TRANSITION_INACTIVE_SHUTDOWN;
    }
    else
    {
      transition_id = lifecycle_msgs::msg::Transition::TRANSITION_ACTIVE_SHUTDOWN;
    }

    auto state = impl_->triggerTransition(transition_id);
    return stateFromLabel(state.label);
  }

  detail::LifecycleState LifecycleNode::shutdown(CallbackReturn& cb_return_code)
  {
    auto& current = impl_->getCurrentState();
    uint8_t transition_id;

    if (current.id == lifecycle::state::PRIMARY_STATE_UNCONFIGURED)
    {
      transition_id = lifecycle_msgs::msg::Transition::TRANSITION_UNCONFIGURED_SHUTDOWN;
    }
    else if (current.id == lifecycle::state::PRIMARY_STATE_INACTIVE)
    {
      transition_id = lifecycle_msgs::msg::Transition::TRANSITION_INACTIVE_SHUTDOWN;
    }
    else
    {
      transition_id = lifecycle_msgs::msg::Transition::TRANSITION_ACTIVE_SHUTDOWN;
    }

    auto state = impl_->triggerTransition(transition_id, cb_return_code);
    return stateFromLabel(state.label);
  }

  // ── 回调注册 ──────────────────────────────────────────────

  void LifecycleNode::registerOnConfigure(
      std::function<CallbackReturn(const detail::LifecycleState&)> fcn)
  {
    impl_->registerCallback(
        lifecycle_msgs::msg::State::TRANSITION_STATE_CONFIGURING, std::move(fcn));
  }

  void LifecycleNode::registerOnCleanup(
      std::function<CallbackReturn(const detail::LifecycleState&)> fcn)
  {
    impl_->registerCallback(
        lifecycle_msgs::msg::State::TRANSITION_STATE_CLEANINGUP, std::move(fcn));
  }

  void LifecycleNode::registerOnShutdown(
      std::function<CallbackReturn(const detail::LifecycleState&)> fcn)
  {
    impl_->registerCallback(
        lifecycle_msgs::msg::State::TRANSITION_STATE_SHUTTINGDOWN, std::move(fcn));
  }

  void LifecycleNode::registerOnActivate(
      std::function<CallbackReturn(const detail::LifecycleState&)> fcn)
  {
    impl_->registerCallback(
        lifecycle_msgs::msg::State::TRANSITION_STATE_ACTIVATING, std::move(fcn));
  }

  void LifecycleNode::registerOnDeactivate(
      std::function<CallbackReturn(const detail::LifecycleState&)> fcn)
  {
    impl_->registerCallback(
        lifecycle_msgs::msg::State::TRANSITION_STATE_DEACTIVATING, std::move(fcn));
  }

  void LifecycleNode::registerOnError(
      std::function<CallbackReturn(const detail::LifecycleState&)> fcn)
  {
    impl_->registerCallback(
        lifecycle_msgs::msg::State::TRANSITION_STATE_ERRORPROCESSING,
        std::move(fcn));
  }

  // ═══════════════════════════════════════════════════════════
  // LifecycleNode::Impl
  // ═══════════════════════════════════════════════════════════

  LifecycleNode::Impl::Impl()
  {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    state_machine_.initDefaultStateMachine();
    current_state_ = state_machine_.current_state;
  }

  const detail::LifecycleState& LifecycleNode::Impl::getCurrentState() const
  {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    return current_state_;
  }

  void LifecycleNode::Impl::registerCallback(
      uint8_t transition_id,
      std::function<CallbackReturn(const detail::LifecycleState&)> cb)
  {
    if (!cb)
    {
      return;
    }
    cb_map_[transition_id] = std::move(cb);
  }

  // ── triggerTransition ─────────────────────────────────────

  const detail::LifecycleState& LifecycleNode::Impl::triggerTransition(
      uint8_t transition_id)
  {
    CallbackReturn unused;
    return triggerTransition(transition_id, unused);
  }

  const detail::LifecycleState& LifecycleNode::Impl::triggerTransition(
      uint8_t transition_id, CallbackReturn& cb_return_code)
  {
    changeState(transition_id, cb_return_code);
    return current_state_;
  }

  // ── changeState（核心：两段式转换） ─────────────────────────

  int LifecycleNode::Impl::changeState(uint8_t transition_id,
                                        CallbackReturn& cb_return_code)
  {
    detail::LifecycleState initial_state;
    unsigned int current_state_id;

    {
      std::lock_guard<std::recursive_mutex> lock(mutex_);

      // 保存上一个主状态
      initial_state = stateFromLabel(state_machine_.current_state.label);

      // Phase A：主状态 → 过渡状态
      if (triggerTransitionById(state_machine_, transition_id) != 0)
      {
        return -1;
      }

      current_state_id = state_machine_.current_state.id;
    }

    // 更新内部 current_state_
    current_state_ = state_machine_.current_state;

    // Phase B：执行回调（用 transition state 找 cb_map_）
    cb_return_code = executeCallback(current_state_id, initial_state);
    const auto& transition_label = labelForReturnCode(cb_return_code);

    {
      std::lock_guard<std::recursive_mutex> lock(mutex_);

      // Phase C：过渡状态 → 最终主状态（取决于回调返回值）
      if (triggerTransitionByLabel(state_machine_, transition_label) != 0)
      {
        return -1;
      }

      current_state_id = state_machine_.current_state.id;
    }

    // 更新 internal current_state_
    current_state_ = state_machine_.current_state;

    // Phase D：如果回调返回 ERROR → 进入 error 处理路径
    if (cb_return_code == CallbackReturn::ERROR)
    {
      // 此时 current_state_id 已经是 errorprocessing 的状态 ID
      auto error_cb_code =
          executeCallback(current_state_id, initial_state);
      const auto& error_label = labelForReturnCode(error_cb_code);

      std::lock_guard<std::recursive_mutex> lock(mutex_);
      if (triggerTransitionByLabel(state_machine_, error_label) != 0)
      {
        return -1;
      }
    }

    // 更新 current_state_
    current_state_ = state_machine_.current_state;

    return 0;
  }

  // ── executeCallback ───────────────────────────────────────

  LifecycleNode::Impl::CallbackReturn LifecycleNode::Impl::executeCallback(
      unsigned int cb_id, const detail::LifecycleState& previous_state) const
  {
    auto it = cb_map_.find(static_cast<uint8_t>(cb_id));
    if (it != cb_map_.end())
    {
      try
      {
        return it->second(previous_state);
      }
      catch (const std::exception&)
      {
        return CallbackReturn::ERROR;
      }
    }

    // 没有注册回调 → 默认 SUCCESS
    return CallbackReturn::SUCCESS;
  }

  // ── 查询合法转换 ───────────────────────────────────────────

  std::optional<detail::LifecycleTransition>
  LifecycleNode::Impl::getTransitionById(const detail::LifecycleState& state,
                                          uint8_t id)
  {
    for (const auto& t : state.valid_transitions)
    {
      if (t.id == id)
      {
        return t;
      }
    }
    return std::nullopt;
  }

  std::optional<detail::LifecycleTransition>
  LifecycleNode::Impl::getTransitionByLabel(const detail::LifecycleState& state,
                                              const std::string& label)
  {
    for (const auto& t : state.valid_transitions)
    {
      if (t.label == label)
      {
        return t;
      }
    }
    return std::nullopt;
  }

  // ── 执行一次 transition ────────────────────────────────────

  int LifecycleNode::Impl::triggerOneTransition(
      LifecycleStateMachine& state_machine,
      const detail::LifecycleTransition& transition)
  {
    state_machine.current_state = transition.goal;
    return 0;
  }

  int LifecycleNode::Impl::triggerTransitionById(
      LifecycleStateMachine& state_machine, uint8_t id)
  {
    auto transition = getTransitionById(state_machine.current_state, id);
    if (!transition)
    {
      return -1;
    }
    return triggerOneTransition(state_machine, *transition);
  }

  int LifecycleNode::Impl::triggerTransitionByLabel(
      LifecycleStateMachine& state_machine, const std::string& label)
  {
    auto transition = getTransitionByLabel(state_machine.current_state, label);
    if (!transition)
    {
      return -1;
    }
    return triggerOneTransition(state_machine, *transition);
  }

  // ── labelForReturnCode ────────────────────────────────────

  const std::string& LifecycleNode::Impl::labelForReturnCode(
      CallbackReturn code)
  {
    if (code == CallbackReturn::SUCCESS)
    {
      return kTransitionSuccess;
    }
    else if (code == CallbackReturn::FAILURE)
    {
      return kTransitionFailure;
    }
    return kTransitionError;
  }
}  // namespace lifecycle