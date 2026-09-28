#pragma once

#include <functional>
#include <memory>
#include <string>

#include "lifecycle_msgs/msg/state.hpp"
#include "lifecycle_msgs/msg/transition.hpp"
#include "state_machine/lifecycle_interface.h"

namespace detail
{
  struct LifecycleState;
}

namespace lifecycle
{
  /// 根据标签获取 LifecycleState
  detail::LifecycleState stateFromLabel(const std::string& label);

  /// 生命周期节点 — 提供 configure/activate/... 入口 + 回调注册
  class LifecycleNode : public ifs::LifecycleNodeInterface
  {
   public:
    using CallbackReturn = ifs::LifecycleNodeInterface::CallbackReturn;

    LifecycleNode();
    ~LifecycleNode() override;

    /// 获取当前状态
    detail::LifecycleState getCurrentState() const;

    // ── 转换触发接口 ──────────────────────────────────────────

    detail::LifecycleState configure();
    detail::LifecycleState configure(CallbackReturn& cb_return_code);

    detail::LifecycleState cleanup();
    detail::LifecycleState cleanup(CallbackReturn& cb_return_code);

    detail::LifecycleState activate();
    detail::LifecycleState activate(CallbackReturn& cb_return_code);

    detail::LifecycleState deactivate();
    detail::LifecycleState deactivate(CallbackReturn& cb_return_code);

    detail::LifecycleState shutdown();
    detail::LifecycleState shutdown(CallbackReturn& cb_return_code);

    // ── 回调注册 ──────────────────────────────────────────────

    void registerOnConfigure(
        std::function<CallbackReturn(const detail::LifecycleState&)> fcn);

    void registerOnCleanup(
        std::function<CallbackReturn(const detail::LifecycleState&)> fcn);

    void registerOnShutdown(
        std::function<CallbackReturn(const detail::LifecycleState&)> fcn);

    void registerOnActivate(
        std::function<CallbackReturn(const detail::LifecycleState&)> fcn);

    void registerOnDeactivate(
        std::function<CallbackReturn(const detail::LifecycleState&)> fcn);

    void registerOnError(
        std::function<CallbackReturn(const detail::LifecycleState&)> fcn);

   private:
    class Impl;
    std::unique_ptr<Impl> impl_;
  };
}  // namespace lifecycle