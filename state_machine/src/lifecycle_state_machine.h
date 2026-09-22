// IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
// File Name        : lifecycle_state_machine.h
// Author           : xxx
// Version          : V2.0.0
// Date             : 2026/9/22
// Description      :
// This software is provided by the copyright holders and contributors "as is"
// and any express or implied warranties, including, but not limited to, the
// implied warranties of merchantability and fitness for a particular purpose
// are disclaimed. In no event shall the UBTECH Corporation or contributors be
// liable for any direct, indirect, incidental, special, exemplary, or
// consequential damages (including, but not limited to, procurement of
// substitute goods or services; loss of use, data, or profits; or business
// interruption) however caused and on any theory of liability, whether in
// contract, strict liability, or tort (including negligence or otherwise)
// arising in any way out of the use of this software, even if advised of the
// possibility of such damage.

#pragma once
#include <string>
#include <vector>

#include "data_types.h"
#include "lifecycle_enums.h"

inline const std::string kTransitionSuccess = "transition_success";
inline const std::string kTransitionFailure = "transition_failure";
inline const std::string kTransitionError = "transition_error";

/// 状态转换表：包含所有注册的状态和转换
struct LifecycleTransitionMap
{
  /// 注册的完整状态列表
  std::vector<detail::LifecycleState> states;

  /// 注册的完整转换列表
  std::vector<detail::LifecycleTransition> transitions;
};

/// 生命周期状态机
struct LifecycleStateMachine
{
 public:
  /**
   * @brief 初始化默认状态机
   * It registers all: primary states, transition states, transitions and the
   * initial state. The primary state is unconfigured.
   *
   * States: unknown, unconfigured, inactive, active and finalized.
   * Transition states: configuring, cleaningup, activating, deactivating,
   *                    errorprocessing and shuttingdown.
   *
   * Transitions:
   *    - unconfigured to configuring
   *    - unconfigured to shuttingdown
   *    - configuring to inactive
   *    - configuring to unconfigured
   *    - configuring to errorprocessing
   *    - inactive to activating
   *    - inactive to cleaningup
   *    - inactive to shuttingdown
   *    - cleaningup to unconfigured
   *    - cleaningup to inactive
   *    - cleaniningup to errorprocessing
   *    - activating to active
   *    - activating to inactive
   *    - activating to errorprocessing
   *    - active to deactivating
   *    - active to shuttingdown
   *    - deactivating to inactive
   *    - deactivating to active
   *    - deactivating to errorprocessing
   *    - shutting down to finalized
   *    - shutting down to finalized
   *    - shutting down to errorprocessing
   *    - errorprocessing to uncofigured
   *    - errorprocessing to finalized
   *    - errorprocessing to finalized
   */
  void initDefaultStateMachine();

 private:
  /**
   * @brief 注册所有 primary states
   *
   */
  void registerAllPrimaryStates();

  /**
   * @brief 注册所有 transition states
   *
   */
  void registerAllTransitions();

  /**
   * @brief 注册所有转换
   *
   */
  void registerTransitions();

 public:
  /// Current state of the state machine
  detail::LifecycleState current_state;

  /// Map/Associated array of registered states and transitions
  LifecycleTransitionMap transition_map;
};