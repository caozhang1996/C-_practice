#pragma once

#include <functional>
#include <map>
#include <mutex>
#include <optional>
#include <string>

#include "data_types.h"
#include "lifecycle_msgs/msg/transition_event.hpp"
#include "lifecycle_msgs/srv/change_state.hpp"
#include "lifecycle_msgs/srv/get_available_states.hpp"
#include "lifecycle_msgs/srv/get_available_transitions.hpp"
#include "lifecycle_msgs/srv/get_state.hpp"
#include "lifecycle_state_machine.h"
#include "rclcpp/publisher.hpp"
#include "rclcpp/service.hpp"
#include "state_machine/lifecycle_node.h"

namespace lifecycle
{
  /// LifecycleNode 的 PIMPL 实现
  class LifecycleNode::LifecycleNodeImpl final
  {
    using CallbackReturn = LifecycleNodeInterface::CallbackReturn;

    using ChangeStateSrv = lifecycle_msgs::srv::ChangeState;
    using GetStateSrv = lifecycle_msgs::srv::GetState;
    using GetAvailableStatesSrv = lifecycle_msgs::srv::GetAvailableStates;
    using GetAvailableTransitionsSrv =
        lifecycle_msgs::srv::GetAvailableTransitions;
    using TransitionEventMsg = lifecycle_msgs::msg::TransitionEvent;

   public:
    LifecycleNodeImpl(LifecycleNode* parent_node);
    ~LifecycleNodeImpl() = default;

    /**
     * @brief 注册对应 transition 的回调函数
     *
     * @param transition_id: transition 对应的 id
     * @param cb
     * @return void
     */
    void registerCallback(
        uint8_t transition_id,
        std::function<CallbackReturn(const LifecycleState&)>& cb);

    /**
     * @brief 获得当前状态
     *
     * @return const LifecycleState &
     */
    const detail::LifecycleState& getCurrentState() const;

    /**
     * @brief 根据转换 ID 触发转换
     *
     * @param transition_id
     * @return const LifecycleState &
     */
    const detail::LifecycleState& triggerTransition(uint8_t transition_id);

    /**
     * @brief 根据转换 ID 触发转换
     *
     * @param transition_id
     * @param cb_return_code
     * @return const LifecycleState &
     */
    const detail::LifecycleState& triggerTransition(
        uint8_t transition_id, CallbackReturn& cb_return_code);

    /**
     * @brief 根据转换标签触发转换
     *
     * @param transition_label
     * @return const LifecycleState &
     */
    const detail::LifecycleState& triggerTransition(
        const std::string& transition_label);

    /**
     * @brief 根据转换标签触发转换
     *
     * @param transition_label
     * @param cb_return_code
     * @return const LifecycleState &
     */
    const detail::LifecycleState& triggerTransition(
        const std::string& transition_label, CallbackReturn& cb_return_code);

   private:
    LifecycleNodeImpl(const LifecycleNodeImpl&) = delete;
    LifecycleNodeImpl& operator=(const LifecycleNodeImpl&) = delete;

    /**
     * @brief srv_change_state_ 的回调函数
     *
     * @param req
     * @param resp
     */
    void onChangeState(ChangeStateSrv::Request::ConstSharedPtr req,
                       ChangeStateSrv::Response::SharedPtr resp);

    /**
     * @brief srv_get_state_ 的回调函数
     *
     * @param req
     * @param resp
     */
    void onGetState(GetStateSrv::Request::ConstSharedPtr req,
                    GetStateSrv::Response::SharedPtr resp) const;

    /**
     * @brief srv_get_available_states_ 的回调函数
     *
     * @param req
     * @param resp
     */
    void onGetAvailableStates(
        GetAvailableStatesSrv::Request::ConstSharedPtr req,
        GetAvailableStatesSrv::Response::SharedPtr resp) const;

    /**
     * @brief srv_get_available_transitions_ 的回调函数
     *
     * @param req
     * @param resp
     */
    void onGetAvailableTransitions(
        GetAvailableTransitionsSrv::Request::ConstSharedPtr req,
        GetAvailableTransitionsSrv::Response::SharedPtr resp) const;

    /**
     * @brief srv_get_transition_graph_ 的回调函数
     *
     * @param req
     * @param resp
     */
    void onGetTransitionGraph(
        GetAvailableTransitionsSrv::Request::ConstSharedPtr req,
        GetAvailableTransitionsSrv::Response::SharedPtr resp) const;

    /**
     * @brief 根据 transition_id 执行相应的状态转换
     *
     * @param transition_id
     * @param cb_return_code
     * @return int
     */
    int changeState(uint8_t transition_id, CallbackReturn& cb_return_code);

    /**
     * @brief 执行 cb_id 对应的回调函数
     *
     * @param cb_id
     * @param previous_state
     * @return CallbackReturn
     */
    CallbackReturn executeCallback(unsigned int cb_id,
                                   const LifecycleState& previous_state) const;

    /**
     * @brief 根据 transition id 从 state 找到对应的 transition
     *
     * @param state
     * @param id
     * @return std::optional<LifecycleTransition>
     */
    std::optional<detail::LifecycleTransition> getTransitionById(
        const detail::LifecycleState& state, uint8_t id);

    /**
     * @brief 根据 label 从 state 找到对应的 transition
     *
     * @param state
     * @param label
     * @return std::optional<LifecycleTransition>
     */
    std::optional<detail::LifecycleTransition> getTransitionByLabel(
        const detail::LifecycleState& state, const std::string& label);

    /// 执行一次 transition（改 current_state + publish）
    int triggerOneTransition(LifecycleStateMachine& state_machine,
                             const detail::LifecycleTransition& transition);

    /**
     * @brief 根据 transition_id 执行相应的状态转换
     *
     * @param state_machine
     * @param id
     * @return int
     */
    int triggerTransitionById(LifecycleStateMachine& state_machine, uint8_t id);

    /**
     * @brief 根据 label 执行相应的状态转换
     *
     * @param state_machine
     * @param label
     * @return int
     */
    int triggerTransitionByLabel(LifecycleStateMachine& state_machine,
                                 const std::string& label);

    /// 根据回调返回码取 transition label
    static const std::string& labelForReturnCode(CallbackReturn code);

   private:
    using TransitionEventPtr = rclcpp::Publisher<TransitionEventMsg>::SharedPtr;
    using ChangeStateSrvPtr = rclcpp::Service<ChangeStateSrv>::SharedPtr;
    using GetStateSrvPtr = rclcpp::Service<GetStateSrv>::SharedPtr;
    using GetAvailableStatesSrvPtr =
        rclcpp::Service<GetAvailableStatesSrv>::SharedPtr;
    using GetAvailableTransitionsSrvPtr =
        rclcpp::Service<GetAvailableTransitionsSrv>::SharedPtr;
    using GetTransitionGraphSrvPtr =
        rclcpp::Service<GetAvailableTransitionsSrv>::SharedPtr;

    TransitionEventPtr pub_transition_event_;
    ChangeStateSrvPtr srv_change_state_;
    GetStateSrvPtr srv_get_state_;
    GetAvailableStatesSrvPtr srv_get_available_states_;
    GetAvailableTransitionsSrvPtr srv_get_available_transitions_;
    GetTransitionGraphSrvPtr srv_get_transition_graph_;

    mutable std::recursive_mutex mutex_;
    LifecycleStateMachine state_machine_;
    detail::LifecycleState current_state_;

    std::map<uint8_t, std::function<CallbackReturn(const LifecycleState&)>>
        cb_map_;

    LifecycleNode* node_;
  };
}  // namespace lifecycle