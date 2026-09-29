#include <stdexcept>

#include "lifecycle_node_impl.h"

namespace lifecycle
{
  LifecycleNodeImpl::LifecycleNodeImpl(LifecycleNode* parent_node)
  {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    state_machine_.initDefaultStateMachine();
    current_state_ = state_machine_.current_state;
  }

  const detail::LifecycleState& LifecycleNodeImpl::getCurrentState() const
  {
    return current_state_;
  }

  void LifecycleNodeImpl::registerCallback(
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

  const detail::LifecycleState& LifecycleNodeImpl::triggerTransition(
      uint8_t transition_id)
  {
    CallbackReturn unused;
    return triggerTransition(transition_id, unused);
  }

  const detail::LifecycleState& LifecycleNodeImpl::triggerTransition(
      uint8_t transition_id, CallbackReturn& cb_return_code)
  {
    changeState(transition_id, cb_return_code);
    return current_state_;
  }

  // ── changeState（核心：两段式转换） ─────────────────────────

  int LifecycleNodeImpl::changeState(uint8_t transition_id,
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
      auto error_cb_code = executeCallback(current_state_id, initial_state);
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

  LifecycleNodeImpl::CallbackReturn LifecycleNodeImpl::executeCallback(
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
  LifecycleNodeImpl::getTransitionById(const detail::LifecycleState& state,
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
  LifecycleNodeImpl::getTransitionByLabel(const detail::LifecycleState& state,
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

  int LifecycleNodeImpl::triggerOneTransition(
      LifecycleStateMachine& state_machine,
      const detail::LifecycleTransition& transition)
  {
    state_machine.current_state = transition.goal;
    return 0;
  }

  int LifecycleNodeImpl::triggerTransitionById(
      LifecycleStateMachine& state_machine, uint8_t id)
  {
    auto transition = getTransitionById(state_machine.current_state, id);
    if (!transition)
    {
      return -1;
    }
    return triggerOneTransition(state_machine, *transition);
  }

  int LifecycleNodeImpl::triggerTransitionByLabel(
      LifecycleStateMachine& state_machine, const std::string& label)
  {
    auto transition = getTransitionByLabel(state_machine.current_state, label);
    if (!transition)
    {
      return -1;
    }
    return triggerOneTransition(state_machine, *transition);
  }

  const std::string& LifecycleNodeImpl::labelForReturnCode(CallbackReturn code)
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