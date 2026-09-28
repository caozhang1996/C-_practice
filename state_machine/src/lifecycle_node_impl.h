#pragma once

#include <functional>
#include <map>
#include <mutex>
#include <optional>
#include <string>

#include "data_types.h"
#include "lifecycle_msgs/msg/state.hpp"
#include "lifecycle_msgs/msg/transition.hpp"
#include "lifecycle_state_machine.h"
#include "state_machine/lifecycle_node.h"

namespace lifecycle
{
  /// LifecycleNode 的 PIMPL 实现
  class LifecycleNode::Impl
  {
   public:
    using CallbackReturn = ifs::LifecycleNodeInterface::CallbackReturn;

    Impl();
    ~Impl() = default;

    /// 获取当前状态
    const detail::LifecycleState& getCurrentState() const;

    /// 注册对应 transition 的回调
    void registerCallback(uint8_t transition_id,
                          std::function<CallbackReturn(const detail::LifecycleState&)> cb);

    /// 根据转换 ID 触发转换（单参数版）
    const detail::LifecycleState& triggerTransition(uint8_t transition_id);

    /// 根据转换 ID 触发转换（带回传返回码）
    const detail::LifecycleState& triggerTransition(
        uint8_t transition_id, CallbackReturn& cb_return_code);

   private:
    /// core：两段式转换
    int changeState(uint8_t transition_id, CallbackReturn& cb_return_code);

    /// 执行回调
    CallbackReturn executeCallback(
        unsigned int cb_id,
        const detail::LifecycleState& previous_state) const;

    /// 从当前状态找合法转换
    std::optional<detail::LifecycleTransition> getTransitionById(
        const detail::LifecycleState& state, uint8_t id);
    std::optional<detail::LifecycleTransition> getTransitionByLabel(
        const detail::LifecycleState& state, const std::string& label);

    /// 执行一次 transition（改 current_state + publish）
    int triggerOneTransition(LifecycleStateMachine& state_machine,
                             const detail::LifecycleTransition& transition);
    int triggerTransitionById(LifecycleStateMachine& state_machine, uint8_t id);
    int triggerTransitionByLabel(LifecycleStateMachine& state_machine,
                                 const std::string& label);

    /// 根据回调返回码取 transition label
    static const std::string& labelForReturnCode(CallbackReturn code);

   private:
    mutable std::recursive_mutex mutex_;
    LifecycleStateMachine state_machine_;
    detail::LifecycleState current_state_;

    std::map<uint8_t, std::function<CallbackReturn(const detail::LifecycleState&)>>
        cb_map_;
  };
}  // namespace lifecycle