// IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
// File Name        : lifecycle.h
// Author           : xxx
// Version          : V2.0.0
// Date             : 2026/9/29
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

namespace lifecycle
{
  /**
   * 该枚举类型用于给外部调用方展示状态，和 detail::LifecycleState 结构体有区分
   */
  enum class LifecycleState
  {
    PRIMARY_STATE_UNKNOWN = 0,
    PRIMARY_STATE_UNCONFIGURED = 1,
    PRIMARY_STATE_INACTIVE = 2,
    PRIMARY_STATE_ACTIVE = 3,
    PRIMARY_STATE_FINALIZED = 4,
    TRANSITION_STATE_CONFIGURING = 10,
    TRANSITION_STATE_CLEANINGUP = 11,
    TRANSITION_STATE_SHUTTINGDOWN = 12,
    TRANSITION_STATE_ACTIVATING = 13,
    TRANSITION_STATE_DEACTIVATING = 14,
    TRANSITION_STATE_ERRORPROCESSING = 15
  };

  /**
   * @brief 根据标签获取生命周期状态对应的枚举
   *
   * @param label
   * @return ROSA_PUBLIC
   */
  LifecycleState stateFromLabel(const std::string& label);
}  // namespace lifecycle
