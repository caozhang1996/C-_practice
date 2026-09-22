#include "data_types.h"

namespace detail
{
  LifecycleState::LifecycleState(std::string _label, unsigned int _id)
  {
    label = _label;
    id = _id;
  }

  LifecycleState::LifecycleState(const LifecycleState& other) { *this = other; }

  LifecycleState& LifecycleState::operator=(const LifecycleState& other)
  {
    if (this != &other)
    {
      // 复制基本类型成员
      label = other.label;
      id = other.id;

      // 将目标状态添加到 valid_transitions 中
      valid_transitions.clear();
      for (const LifecycleTransition& transition : other.valid_transitions)
      {
        valid_transitions.push_back(transition);
      }
    }

    return *this;
  }

  LifecycleTransition::LifecycleTransition(const std::string& _label,
                                           unsigned int _id,
                                           LifecycleState& startState,
                                           LifecycleState& goalState)
      : label(_label), id(_id), start(startState), goal(goalState)
  {
  }

  LifecycleTransition::LifecycleTransition(const LifecycleTransition& other)
      : label(other.label), id(other.id), start(other.start), goal(other.goal)
  {
  }

  LifecycleTransition& LifecycleTransition::operator=(
      const LifecycleTransition& other)
  {
    if (this != &other)
    {
      label = other.label;
      id = other.id;
      // Copying references is not allowed, so do nothing for start and goal
    }

    return *this;
  }

}  // namespace detail
