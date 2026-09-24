// IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
// File Name        : lifecycle_node_interface.h
// Author           : xxx
// Version          : V2.0.0
// Date             : 2024/7/29
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

#include "lifecycle_msgs/msg/Transition.h"

enum class LifecycleState;

namespace ifs
{
  class LifecycleNodeInterface
  {
   protected:
    LifecycleNodeInterface() {}

   public:
    enum class CallbackReturn : uint8_t
    {
      SUCCESS = lifecycle_msgs::msg::Transition::TRANSITION_CALLBACK_SUCCESS,
      FAILURE = lifecycle_msgs::msg::Transition::TRANSITION_CALLBACK_FAILURE,
      ERROR = lifecycle_msgs::msg::Transition::TRANSITION_CALLBACK_ERROR
    };

    /**
     * @brief 用于 configure transition 的回调函数
     *
     * @param previous_state
     * @param extra_msg
     * @return CallbackReturn, default is SUCCESS
     */

    virtual CallbackReturn onConfigure(const LifecycleState& previous_state,
                                       const std::string& extra_msg);

    /**
     * @brief 用于 cleanup transition 的回调函数
     *
     * @param previous_state
     * @param extra_msg
     * @return CallbackReturn, default is SUCCESS
     */

    virtual CallbackReturn onCleanup(const LifecycleState& previous_state,
                                     const std::string& extra_msg);

    /**
     * @brief 用于 shutdown transition 的回调函数
     *
     * @param previous_state
     * @param extra_msg
     * @return CallbackReturn, default is SUCCESS
     */

    virtual CallbackReturn onShutdown(const LifecycleState& previous_state,
                                      const std::string& extra_msg);

    /**
     * @brief 用于 activate transition 的回调函数
     *
     * @param previous_state
     * @param extra_msg
     * @return CallbackReturn, default is SUCCESS
     */

    virtual CallbackReturn onActivate(const LifecycleState& previous_state,
                                      const std::string& extra_msg);

    /**
     * @brief 用于 deactivate transition 的回调函数
     *
     * @param previous_state
     * @param extra_msg
     * @return CallbackReturn, default is SUCCESS
     */

    virtual CallbackReturn onDeactivate(const LifecycleState& previous_state,
                                        const std::string& extra_msg);

    /**
     * @brief 用于 deactivate transition 的回调函数
     *
     * @param previous_state
     * @param extra_msg
     * @return CallbackReturn, default is SUCCESS
     */

    virtual CallbackReturn onError(const LifecycleState& previous_state,
                                   const std::string& extra_msg);

    virtual ~LifecycleNodeInterface() {}
  };
}  // namespace ifs
