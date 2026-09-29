#pragma once

#include <functional>
#include <memory>
#include <string>

#include "lifecycle.h"
#include "lifecycle_interface.h"
#include "lifecycle_msgs/msg/state.hpp"
#include "lifecycle_msgs/msg/transition.hpp"
#include "rclcpp/node.hpp"

namespace lifecycle
{
  /// 生命周期节点 — 提供 configure/activate/... 入口 + 回调注册
  class LifecycleNode : public rclcpp::Node, public LifecycleNodeInterface
  {
   public:
    using CallbackReturn = LifecycleNodeInterface::CallbackReturn;

    LifecycleNode(const std::string& node_name,
                  const rclcpp::NodeOptions& options = rclcpp::NodeOptions());

    virtual ~LifecycleNode();

    /**
     * @brief 获取当前状态
     *
     * @return LifecycleState
     */
    LifecycleState getCurrentState() const;

    /**
     * @brief 触发 configure 转换
     *
     * @return  LifecycleState
     */
    LifecycleState configure();

    /**
     * @brief 触发 configure 转换
     *
     * @param cb_return_code
     * @return  LifecycleState
     */
    LifecycleState configure(CallbackReturn& cb_return_code);

    /**
     * @brief 触发 cleanup 转换
     *
     * @return  LifecycleState
     */
    LifecycleState cleanup();

    /**
     * @brief 触发 cleanup 转换
     *
     * @param cb_return_code
     * @return  LifecycleState
     */
    LifecycleState cleanup(CallbackReturn& cb_return_code);

    /**
     * @brief 触发 activate 转换
     *
     * @return  LifecycleState
     */
    LifecycleState activate();

    /**
     * @brief 触发 activate 转换
     *
     * @param cb_return_code
     * @return  LifecycleState
     */
    LifecycleState activate(CallbackReturn& cb_return_code);

    /**
     * @brief 触发 deactivate 转换
     *
     * @return  LifecycleState
     */
    LifecycleState deactivate();

    /**
     * @brief 触发 deactivate 转换
     *
     * @param cb_return_code
     * @return  LifecycleState
     */
    LifecycleState deactivate(CallbackReturn& cb_return_code);

    /**
     * @brief 触发 shutdown 转换
     *
     * @return  LifecycleState
     */
    LifecycleState shutdown();

    /**
     * @brief 触发 shutdown 转换
     *
     * @param cb_return_code
     * @return  LifecycleState
     */
    LifecycleState shutdown(CallbackReturn& cb_return_code);

    /**
     * @brief 注册 configure 回调，当触发向此状态的转换时，将调用此回调
     *
     * @param fcn
     * @return void
     */
    void registerOnConfigure(
        std::function<CallbackReturn(const LifecycleState&)> fcn);

    /**
     * @brief 注册 cleanup 回调，当触发向此状态的转换时，将调用此回调
     *
     * @param fcn
     * @return void
     */
    void registerOnCleanup(
        std::function<CallbackReturn(const LifecycleState&)> fcn);

    /**
     * @brief 注册 shutdown 回调，当触发向此状态的转换时，将调用此回调
     *
     * @param fcn
     * @return void
     */
    void registerOnShutdown(
        std::function<CallbackReturn(const LifecycleState&)> fcn);

    /**
     * @brief 注册 activate 回调，当触发向此状态的转换时，将调用此回调
     *
     * @param fcn
     * @return void
     */
    void registerOnActivate(
        std::function<CallbackReturn(const LifecycleState&)> fcn);

    /**
     * @brief 注册 deactivate 回调，当触发向此状态的转换时，将调用此回调
     *
     * @param fcn
     * @return void
     */
    void registerOnDeactivate(
        std::function<CallbackReturn(const LifecycleState&)> fcn);

    /**
     * @brief 注册 deactivate 回调，当触发向此状态的转换时，将调用此回调
     *
     * @param fcn
     * @return void
     */
    void registerOnError(
        std::function<CallbackReturn(const LifecycleState&)> fcn);

   private:
    LifecycleNode(const LifecycleNode&) = delete;
    LifecycleNode& operator=(const LifecycleNode&) = delete;

    class LifecycleNodeImpl;

    std::unique_ptr<LifecycleNodeImpl> impl_;
  };
}  // namespace lifecycle