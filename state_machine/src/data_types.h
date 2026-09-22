// IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
// File Name        : data_types.h
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
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace detail
{
  struct LifecycleTransition;

  /**
   * 生命周期状态
   * 每个状态不仅知道自己叫什么名字、ID
   * 是多少，还知道自己能从当前状态跳转到哪些目标状态（valid_transitions）。这相当于每个状态自带一份"允许的出口列表"。
   */
  struct LifecycleState
  {
    /// 状态名，如 "unconfigured","inactivate", "activate"
    std::string label;

    /// 状态数字ID
    unsigned int id;

    /// 从当前状态可以发起的合法转换
    std::vector<LifecycleTransition> valid_transitions;

    LifecycleState() = default;

    LifecycleState(std::string _label, unsigned int _id);

    LifecycleState(const LifecycleState& other);

    LifecycleState& operator=(const LifecycleState& other);
  };

  /**
   * 状态转换/迁移
   */
  struct LifecycleTransition
  {
    /// String with transition name: configuring, cleaningup, activating,
    /// deactivating, errorprocessing or shuttingdown.
    std::string label;

    /// Identifier of the transition
    unsigned int id;

    /// The value where the transition is initialized
    LifecycleState& start;

    /// The objective of the transition
    LifecycleState& goal;

    LifecycleTransition(const std::string& _label,
                        unsigned int _id,
                        LifecycleState& startState,
                        LifecycleState& goalState);

    LifecycleTransition(const LifecycleTransition& other);

    LifecycleTransition& operator=(const LifecycleTransition& other);
  };
}  // namespace detail
