#include <functional>
#include <stdexcept>

#include "lifecycle_node_impl.h"

namespace lifecycle
{
  std::string kPubStateTopic = "/transition_event";
  std::string kChangeStateSrv = "/change_state";
  std::string kGetStateSrv = "/get_state";
  std::string kGetAvailableStateSrv = "/get_available_state";
  std::string kGetAvailableTransitionSrv = "/get_available_transitions";
  std::string kGetTransitionGraphSrv = "/get_transition_graph";

  LifecycleNode::LifecycleNodeImpl::LifecycleNodeImpl(
      LifecycleNode* parent_node)
      : node_(parent_node)
  {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    state_machine_.initDefaultStateMachine();
    current_state_ = state_machine_.current_state;

    const auto& node_name = parent_node->get_name();

    {
      std::string topic_name = node_name + kPubStateTopic;
      pub_transition_event_ = node_->create_publisher<TransitionEventMsg>(
          topic_name, rclcpp::QoS(10));
    }

    {
      std::string service_name = node_name + kChangeStateSrv;
      srv_change_state_ = node_->create_service<ChangeStateSrv>(
          service_name,
          std::bind(&LifecycleNodeImpl::onChangeState, this,
                    std::placeholders::_1, std::placeholders::_2));
    }

    {
      std::string service_name = node_name + kGetStateSrv;
      srv_get_state_ = node_->create_service<GetStateSrv>(
          service_name,
          std::bind(&LifecycleNodeImpl::onGetState, this, std::placeholders::_1,
                    std::placeholders::_2));
    }

    {
      std::string service_name = node_name + kGetAvailableStateSrv;
      srv_get_available_states_ = node_->create_service<GetAvailableStatesSrv>(
          service_name,
          std::bind(&LifecycleNodeImpl::onGetAvailableStates, this,
                    std::placeholders::_1, std::placeholders::_2));
    }

    {
      std::string service_name = node_name + kGetAvailableTransitionSrv;
      srv_get_available_transitions_ =
          node_->create_service<GetAvailableTransitionsSrv>(
              service_name,
              std::bind(&LifecycleNodeImpl::onGetAvailableTransitions, this,
                        std::placeholders::_1, std::placeholders::_2));
    }

    {
      std::string service_name = node_name + kGetTransitionGraphSrv;
      srv_get_transition_graph_ =
          node_->create_service<GetAvailableTransitionsSrv>(
              service_name,
              std::bind(&LifecycleNodeImpl::onGetTransitionGraph, this,
                        std::placeholders::_1, std::placeholders::_2));
    }
  }

  void LifecycleNode::LifecycleNodeImpl::registerCallback(
      uint8_t transition_id,
      std::function<CallbackReturn(const LifecycleState&)>& cb)
  {
    if (!cb)
    {
      std::cout << "LifecycleNode::registerCallback: callback is empty"
                << std::endl;
      return;
    }

    cb_map_[transition_id] = cb;
  }

  const detail::LifecycleState&
  LifecycleNode::LifecycleNodeImpl::getCurrentState() const
  {
    return current_state_;
  }

  const detail::LifecycleState&
  LifecycleNode::LifecycleNodeImpl::triggerTransition(uint8_t transition_id)
  {
    CallbackReturn error;
    return triggerTransition(transition_id, error);
  }

  const detail::LifecycleState&
  LifecycleNode::LifecycleNodeImpl::triggerTransition(
      uint8_t transition_id, CallbackReturn& cb_return_code)
  {
    changeState(transition_id, cb_return_code);
    return getCurrentState();
  }

  const detail::LifecycleState&
  LifecycleNode::LifecycleNodeImpl::triggerTransition(
      const std::string& transition_label)
  {
    CallbackReturn error;
    return triggerTransition(transition_label, error);
  }

  const detail::LifecycleState&
  LifecycleNode::LifecycleNodeImpl::triggerTransition(
      const std::string& transition_label, CallbackReturn& cb_return_code)
  {
    std::lock_guard<std::recursive_mutex> lock(mutex_);

    std::optional<detail::LifecycleTransition> transition =
        getTransitionByLabel(state_machine_.current_state, transition_label);
    if (transition)
    {
      changeState(static_cast<uint8_t>(transition->id), cb_return_code);
    }

    return getCurrentState();
  }

  int LifecycleNode::LifecycleNodeImpl::changeState(
      uint8_t transition_id, CallbackReturn& cb_return_code)
  {
    LifecycleState initial_state;
    unsigned int current_state_id;

    {
      std::lock_guard<std::recursive_mutex> lock(mutex_);

      // 保存上一个主状态
      initial_state = stateFromLabel(state_machine_.current_state.label);

      // Phase A：主状态 → 过渡状态
      if (triggerTransitionById(state_machine_, transition_id) != 0)
      {
        std::cout << "Unable to start transition {} from current state {}"
                  << std::endl;
        return -1;
      }

      current_state_id = state_machine_.current_state.id;
    }

    // 更新内部 current_state_
    current_state_ = state_machine_.current_state;

    auto get_label_for_return_code =
        [](CallbackReturn cb_return_code) -> const std::string& {
      auto cb_id = static_cast<uint8_t>(cb_return_code);
      if (cb_id == lifecycle_msgs::msg::Transition::TRANSITION_CALLBACK_SUCCESS)
      {
        return kTransitionSuccess;
      }
      else if (cb_id ==
               lifecycle_msgs::msg::Transition::TRANSITION_CALLBACK_FAILURE)
      {
        return kTransitionFailure;
      }

      return kTransitionError;
    };

    // Phase B：执行回调（用 transition state 找 cb_map_）
    cb_return_code = executeCallback(current_state_id, initial_state);
    const auto& transition_label = get_label_for_return_code(cb_return_code);

    {
      std::lock_guard<std::recursive_mutex> lock(mutex_);

      // Phase C：过渡状态 → 最终主状态（取决于回调返回值）
      if (triggerTransitionByLabel(state_machine_, transition_label) != 0)
      {
        std::cout << "Failed to finish transition {}. Current state is now: {}"
                  << std::endl;
        return -1;
      }

      current_state_id = state_machine_.current_state.id;
    }

    // 更新 internal current_state_
    current_state_ = state_machine_.current_state;

    // Phase D：如果回调返回 ERROR → 进入 error 处理路径
    if (cb_return_code == CallbackReturn::ERROR)
    {
      std::cout << "Error occurred while doing error handling." << std::endl;

      // 此时 current_state_id 已经是 errorprocessing 的状态 ID
      auto error_cb_code = executeCallback(current_state_id, initial_state);
      const auto& error_label = get_label_for_return_code(error_cb_code);

      std::lock_guard<std::recursive_mutex> lock(mutex_);
      if (triggerTransitionByLabel(state_machine_, error_label) != 0)
      {
        std::cout << "Failed to call cleanup on error state" << std::endl;
        return -1;
      }
    }

    // 更新 current_state_
    current_state_ = state_machine_.current_state;

    return 0;
  }

  LifecycleNode::LifecycleNodeImpl::CallbackReturn
  LifecycleNode::LifecycleNodeImpl::executeCallback(
      unsigned int cb_id, const LifecycleState& previous_state) const
  {
    // in case no callback was attached
    auto cb_success = CallbackReturn::SUCCESS;

    auto it = cb_map_.find(static_cast<uint8_t>(cb_id));
    if (it != cb_map_.end())
    {
      auto callback = it->second;

      try
      {
        cb_success = callback(previous_state);
      }
      catch (const std::exception& e)
      {
        std::cout << "Caught exception in callback for transition {}"
                  << std::endl;
        std::cout << "Original error: {}" << std::endl;

        cb_success = CallbackReturn::ERROR;
      }
    }

    return cb_success;
  }

  // ── Service 回调 ───────────────────────────────────────────

  void LifecycleNode::LifecycleNodeImpl::onChangeState(
      ChangeStateSrv::Request::ConstSharedPtr req,
      ChangeStateSrv::Response::SharedPtr resp)
  {
    CallbackReturn cb_return_code;
    int ret = changeState(req->transition.id, cb_return_code);
    resp->success = (ret == 0);
  }

  void LifecycleNode::LifecycleNodeImpl::onGetState(
      GetStateSrv::Request::ConstSharedPtr req,
      GetStateSrv::Response::SharedPtr resp) const
  {
    (void)req;
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    resp->current_state.id = current_state_.id;
    resp->current_state.label = current_state_.label;
  }

  void LifecycleNode::LifecycleNodeImpl::onGetAvailableStates(
      GetAvailableStatesSrv::Request::ConstSharedPtr req,
      GetAvailableStatesSrv::Response::SharedPtr resp) const
  {
    (void)req;
    lifecycle_msgs::msg::State state_msg;

    state_msg.id = lifecycle_msgs::msg::State::PRIMARY_STATE_UNCONFIGURED;
    state_msg.label = "unconfigured";
    resp->available_states.push_back(state_msg);

    state_msg.id = lifecycle_msgs::msg::State::PRIMARY_STATE_INACTIVE;
    state_msg.label = "inactive";
    resp->available_states.push_back(state_msg);

    state_msg.id = lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE;
    state_msg.label = "active";
    resp->available_states.push_back(state_msg);

    state_msg.id = lifecycle_msgs::msg::State::PRIMARY_STATE_FINALIZED;
    state_msg.label = "finalized";
    resp->available_states.push_back(state_msg);
  }

  void LifecycleNode::LifecycleNodeImpl::onGetAvailableTransitions(
      GetAvailableTransitionsSrv::Request::ConstSharedPtr req,
      GetAvailableTransitionsSrv::Response::SharedPtr resp) const
  {
    (void)req;
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    const std::vector<detail::LifecycleTransition>& transitions =
        state_machine_.current_state.valid_transitions;
    for (const detail::LifecycleTransition& t : transitions)
    {
      lifecycle_msgs::msg::TransitionDescription desc;
      desc.transition.id = static_cast<uint8_t>(t.id);
      desc.transition.label = t.label;
      desc.start_state.id = static_cast<uint8_t>(t.start.id);
      desc.start_state.label = t.start.label;
      desc.goal_state.id = static_cast<uint8_t>(t.goal.id);
      desc.goal_state.label = t.goal.label;
      resp->available_transitions.push_back(desc);
    }
  }

  void LifecycleNode::LifecycleNodeImpl::onGetTransitionGraph(
      GetAvailableTransitionsSrv::Request::ConstSharedPtr req,
      GetAvailableTransitionsSrv::Response::SharedPtr resp) const
  {
    (void)req;
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    const std::vector<detail::LifecycleTransition>& all_transitions =
        state_machine_.transition_map.transitions;
    for (const detail::LifecycleTransition& t : all_transitions)
    {
      lifecycle_msgs::msg::TransitionDescription desc;
      desc.transition.id = static_cast<uint8_t>(t.id);
      desc.transition.label = t.label;
      desc.start_state.id = static_cast<uint8_t>(t.start.id);
      desc.start_state.label = t.start.label;
      desc.goal_state.id = static_cast<uint8_t>(t.goal.id);
      desc.goal_state.label = t.goal.label;
      resp->available_transitions.push_back(desc);
    }
  }

  // ── 查询合法转换 ───────────────────────────────────────────

  std::optional<detail::LifecycleTransition>
  LifecycleNode::LifecycleNodeImpl::getTransitionById(
      const detail::LifecycleState& state, uint8_t id)
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
  LifecycleNode::LifecycleNodeImpl::getTransitionByLabel(
      const detail::LifecycleState& state, const std::string& label)
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

  int LifecycleNode::LifecycleNodeImpl::triggerOneTransition(
      LifecycleStateMachine& state_machine,
      const detail::LifecycleTransition& transition)
  {
    state_machine.current_state = transition.goal;
    return 0;
  }

  int LifecycleNode::LifecycleNodeImpl::triggerTransitionById(
      LifecycleStateMachine& state_machine, uint8_t id)
  {
    auto transition = getTransitionById(state_machine.current_state, id);
    if (!transition)
    {
      return -1;
    }
    return triggerOneTransition(state_machine, *transition);
  }

  int LifecycleNode::LifecycleNodeImpl::triggerTransitionByLabel(
      LifecycleStateMachine& state_machine, const std::string& label)
  {
    auto transition = getTransitionByLabel(state_machine.current_state, label);
    if (!transition)
    {
      return -1;
    }
    return triggerOneTransition(state_machine, *transition);
  }

  const std::string& LifecycleNode::LifecycleNodeImpl::labelForReturnCode(
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