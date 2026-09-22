#include "lifecycle_state_machine.h"

// primary state labels
constexpr auto kUnkonwnLabel = "unknown";
constexpr auto kUnConfiguredLabel = "unconfigured";
constexpr auto kInactiveLabel = "inactive";
constexpr auto kActiveLabel = "active";
constexpr auto kFinalizedLabel = "finalized";

// transition state labels
constexpr auto kConfiguringLabel = "configuring";
constexpr auto kCleaningUpLabel = "cleaningup";
constexpr auto kShuttingDownLabel = "shuttingdown";
constexpr auto kActivatingLabel = "activating";
constexpr auto kDeactivatingLabel = "deactivating";
constexpr auto kErrorProcessingLabel = "errorprocessing";

// external trigger labels
constexpr auto kConfigureLabel = "configure";
constexpr auto kCleanUpLabel = "cleanup";
constexpr auto kActivateLabel = "activate";
constexpr auto kDeactivateLabel = "deactivate";
constexpr auto kShutdownLabel = "shutdown";

using LifecycleState = detail::LifecycleState;
using LifecycleTransition = detail::LifecycleTransition;

void LifecycleStateMachine::initDefaultStateMachine()
{
  // register all primary states
  registerAllPrimaryStates();
  // register all transition states
  registerAllTransitions();
  // register all transitions
  registerTransitions();
}

void LifecycleStateMachine::registerAllPrimaryStates()
{
  // id: 0 ~ 4
  transition_map.states.emplace_back(
      LifecycleState{kUnkonwnLabel, lifecycle::state::PRIMARY_STATE_UNKNOWN});

  transition_map.states.emplace_back(LifecycleState{
      kUnConfiguredLabel, lifecycle::state::PRIMARY_STATE_UNCONFIGURED});

  transition_map.states.emplace_back(
      LifecycleState{kInactiveLabel, lifecycle::state::PRIMARY_STATE_INACTIVE});

  transition_map.states.emplace_back(
      LifecycleState{kActiveLabel, lifecycle::state::PRIMARY_STATE_ACTIVE});

  transition_map.states.emplace_back(LifecycleState{
      kFinalizedLabel, lifecycle::state::PRIMARY_STATE_FINALIZED});
}

void LifecycleStateMachine::registerAllTransitions()
{
  // id: 10 ~ 15
  transition_map.states.emplace_back(LifecycleState{
      kConfiguringLabel, lifecycle::state::TRANSITION_STATE_CONFIGURING});

  transition_map.states.emplace_back(LifecycleState{
      kCleaningUpLabel, lifecycle::state::TRANSITION_STATE_CLEANINGUP});

  transition_map.states.emplace_back(LifecycleState{
      kShuttingDownLabel, lifecycle::state::TRANSITION_STATE_SHUTTINGDOWN});

  transition_map.states.emplace_back(LifecycleState{
      kActivatingLabel, lifecycle::state::TRANSITION_STATE_ACTIVATING});

  transition_map.states.emplace_back(LifecycleState{
      kDeactivatingLabel, lifecycle::state::TRANSITION_STATE_DEACTIVATING});

  transition_map.states.emplace_back(
      LifecycleState{kErrorProcessingLabel,
                     lifecycle::state::TRANSITION_STATE_ERRORPROCESSING});
}

void LifecycleStateMachine::registerTransitions()
{
  auto lifecycle_get_state = [&](std::uint8_t state_id) -> LifecycleState& {
    auto& states = transition_map.states;
    for (unsigned int i = 0; i < states.size(); ++i)
    {
      if (states[i].id == state_id)
      {
        return states[i];
      }
    }

    // 如果没有找到状态，抛出异常
    throw std::runtime_error("State not found with ID: " +
                             std::to_string(state_id));
  };

  auto& unconfigured_state =
      lifecycle_get_state(lifecycle::state::PRIMARY_STATE_UNCONFIGURED);

  auto& inactive_state =
      lifecycle_get_state(lifecycle::state::PRIMARY_STATE_INACTIVE);

  auto& active_state =
      lifecycle_get_state(lifecycle::state::PRIMARY_STATE_ACTIVE);

  auto& finalized_state =
      lifecycle_get_state(lifecycle::state::PRIMARY_STATE_FINALIZED);

  auto& configuring_state =
      lifecycle_get_state(lifecycle::state::TRANSITION_STATE_CONFIGURING);

  auto& activating_state =
      lifecycle_get_state(lifecycle::state::TRANSITION_STATE_ACTIVATING);

  auto& deactivating_state =
      lifecycle_get_state(lifecycle::state::TRANSITION_STATE_DEACTIVATING);

  auto& cleaningup_state =
      lifecycle_get_state(lifecycle::state::TRANSITION_STATE_CLEANINGUP);

  auto& shuttingdown_state =
      lifecycle_get_state(lifecycle::state::TRANSITION_STATE_SHUTTINGDOWN);

  auto& errorprocessing_state =
      lifecycle_get_state(lifecycle::state::TRANSITION_STATE_ERRORPROCESSING);

  auto lifecycle_register_transition = [&](LifecycleTransition& transition) {
    auto& transitions = transition_map.transitions;
    transitions.emplace_back(transition);

    auto& state = lifecycle_get_state(transition.start.id);
    state.valid_transitions.emplace_back(transition);
  };

  // register transition from unconfigured to configuring
  LifecycleTransition transition_configure(
      kConfigureLabel, lifecycle::transition::TRANSITION_CONFIGURE,
      unconfigured_state, configuring_state);
  lifecycle_register_transition(transition_configure);

  // register transition from configuring to inactive
  LifecycleTransition transition_on_configure_success(
      kTransitionSuccess,
      lifecycle::transition::TRANSITION_ON_CONFIGURE_SUCCESS, configuring_state,
      inactive_state);
  lifecycle_register_transition(transition_on_configure_success);

  // register transition from configuring to unconfigured
  LifecycleTransition transition_on_configure_failure(
      kTransitionFailure,
      lifecycle::transition::TRANSITION_ON_CONFIGURE_FAILURE, configuring_state,
      unconfigured_state);
  lifecycle_register_transition(transition_on_configure_failure);

  // register transition from configuring to errorprocessing
  LifecycleTransition transition_on_configure_error(
      kTransitionError, lifecycle::transition::TRANSITION_ON_CONFIGURE_ERROR,
      configuring_state, errorprocessing_state);
  lifecycle_register_transition(transition_on_configure_error);

  // register transition from inactive to cleaningup
  LifecycleTransition transition_cleanup(
      kCleanUpLabel, lifecycle::transition::TRANSITION_CLEANUP, inactive_state,
      cleaningup_state);
  lifecycle_register_transition(transition_cleanup);

  // register transition from cleaningup to unconfigured
  LifecycleTransition transition_on_cleanup_success(
      kTransitionSuccess, lifecycle::transition::TRANSITION_ON_CLEANUP_SUCCESS,
      cleaningup_state, unconfigured_state);
  lifecycle_register_transition(transition_on_cleanup_success);

  // register transition from cleaningup to inactive
  LifecycleTransition transition_on_cleanup_failure(
      kTransitionFailure, lifecycle::transition::TRANSITION_ON_CLEANUP_FAILURE,
      cleaningup_state, inactive_state);
  lifecycle_register_transition(transition_on_cleanup_failure);

  // register transition from cleaniningup to errorprocessing
  LifecycleTransition transition_on_cleanup_error(
      kTransitionError, lifecycle::transition::TRANSITION_ON_CLEANUP_ERROR,
      cleaningup_state, errorprocessing_state);
  lifecycle_register_transition(transition_on_cleanup_error);

  // register transition from inactive to activating
  LifecycleTransition transition_activate(
      kActivateLabel, lifecycle::transition::TRANSITION_ACTIVATE,
      inactive_state, activating_state);
  lifecycle_register_transition(transition_activate);

  // register transition from activating to active
  LifecycleTransition transition_on_activate_success(
      kTransitionSuccess, lifecycle::transition::TRANSITION_ON_ACTIVATE_SUCCESS,
      activating_state, active_state);
  lifecycle_register_transition(transition_on_activate_success);

  // register transition from activating to inactive
  LifecycleTransition transition_on_activate_failure(
      kTransitionFailure, lifecycle::transition::TRANSITION_ON_ACTIVATE_FAILURE,
      activating_state, inactive_state);
  lifecycle_register_transition(transition_on_activate_failure);

  // register transition from activating to errorprocessing
  LifecycleTransition transition_on_activate_error(
      kTransitionError, lifecycle::transition::TRANSITION_ON_ACTIVATE_ERROR,
      activating_state, errorprocessing_state);
  lifecycle_register_transition(transition_on_activate_error);

  // register transition from active to deactivating
  LifecycleTransition transition_deactivate(
      kDeactivateLabel, lifecycle::transition::TRANSITION_DEACTIVATE,
      active_state, deactivating_state);
  lifecycle_register_transition(transition_deactivate);

  // register transition from deactivating to inactive
  LifecycleTransition transition_on_deactivate_success(
      kTransitionSuccess,
      lifecycle::transition::TRANSITION_ON_DEACTIVATE_SUCCESS,
      deactivating_state, inactive_state);
  lifecycle_register_transition(transition_on_deactivate_success);

  // register transition from deactivating to active
  LifecycleTransition transition_on_deactivate_failure(
      kTransitionFailure,
      lifecycle::transition::TRANSITION_ON_DEACTIVATE_FAILURE,
      deactivating_state, active_state);
  lifecycle_register_transition(transition_on_deactivate_failure);

  // register transition from deactivating to errorprocessing
  LifecycleTransition transition_on_deactivate_error(
      kTransitionError, lifecycle::transition::TRANSITION_ON_DEACTIVATE_ERROR,
      deactivating_state, errorprocessing_state);
  lifecycle_register_transition(transition_on_deactivate_error);

  // register transition from unconfigured to shuttingdown
  LifecycleTransition transition_unconfigured_shutdown(
      kShutdownLabel, lifecycle::transition::TRANSITION_UNCONFIGURED_SHUTDOWN,
      unconfigured_state, shuttingdown_state);
  lifecycle_register_transition(transition_unconfigured_shutdown);

  // register transition from inactive to shuttingdown
  LifecycleTransition transition_inactive_shutdown(
      kShutdownLabel, lifecycle::transition::TRANSITION_INACTIVE_SHUTDOWN,
      inactive_state, shuttingdown_state);
  lifecycle_register_transition(transition_inactive_shutdown);

  // register transition from active to shuttingdown
  LifecycleTransition transition_active_shutdown(
      kShutdownLabel, lifecycle::transition::TRANSITION_ACTIVE_SHUTDOWN,
      active_state, shuttingdown_state);
  lifecycle_register_transition(transition_active_shutdown);

  // register transition from shutting down to finalized
  LifecycleTransition transition_on_shutdown_success(
      kTransitionSuccess, lifecycle::transition::TRANSITION_ON_SHUTDOWN_SUCCESS,
      shuttingdown_state, finalized_state);
  lifecycle_register_transition(transition_on_shutdown_success);

  // register transition from shutting down to finalized
  LifecycleTransition transition_on_shutdown_failure(
      kTransitionFailure, lifecycle::transition::TRANSITION_ON_SHUTDOWN_FAILURE,
      shuttingdown_state, finalized_state);
  lifecycle_register_transition(transition_on_shutdown_failure);

  // register transition from shutting down to errorprocessing
  LifecycleTransition transition_on_shutdown_error(
      kTransitionError, lifecycle::transition::TRANSITION_ON_SHUTDOWN_ERROR,
      shuttingdown_state, errorprocessing_state);
  lifecycle_register_transition(transition_on_shutdown_error);

  // register transition from errorprocessing to uncofigured
  LifecycleTransition transition_on_error_success(
      kTransitionSuccess, lifecycle::transition::TRANSITION_ON_ERROR_SUCCESS,
      errorprocessing_state, unconfigured_state);
  lifecycle_register_transition(transition_on_error_success);

  // register transition from errorprocessing to finalized
  LifecycleTransition transition_on_error_failure(
      kTransitionFailure, lifecycle::transition::TRANSITION_ON_ERROR_FAILURE,
      errorprocessing_state, finalized_state);
  lifecycle_register_transition(transition_on_error_failure);

  // register transition from errorprocessing to finalized
  LifecycleTransition transition_on_error_error(
      kTransitionError, lifecycle::transition::TRANSITION_ON_ERROR_ERROR,
      errorprocessing_state, finalized_state);
  lifecycle_register_transition(transition_on_error_error);

  current_state =
      lifecycle_get_state(lifecycle::state::PRIMARY_STATE_UNCONFIGURED);
}