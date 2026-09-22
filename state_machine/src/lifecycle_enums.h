#pragma once

/**
 * @file lifecycle_enums.h
 * @brief ROS2 生命周期标准的状态 ID 和转换 ID 枚举
 *
 * 枚举值遵循 ROS2 Lifecycle 规范定义：
 *   - Primary State ID: 0 ~ 4
 *   - Transition State ID: 10 ~ 15
 *   - Transition ID: 0, 1, 2, ... 按场景分组
 */

namespace lifecycle
{
  /// 状态 ID 枚举（含 Primary State 和 Transition State）
  namespace state
  {
    /// 主状态 (Primary States)
    enum : unsigned int
    {
      PRIMARY_STATE_UNKNOWN = 0,
      PRIMARY_STATE_UNCONFIGURED = 1,
      PRIMARY_STATE_INACTIVE = 2,
      PRIMARY_STATE_ACTIVE = 3,
      PRIMARY_STATE_FINALIZED = 4,
    };

    /// 过渡状态 (Transition States)
    enum : unsigned int
    {
      TRANSITION_STATE_CONFIGURING = 10,
      TRANSITION_STATE_CLEANINGUP = 11,
      TRANSITION_STATE_SHUTTINGDOWN = 12,
      TRANSITION_STATE_ACTIVATING = 13,
      TRANSITION_STATE_DEACTIVATING = 14,
      TRANSITION_STATE_ERRORPROCESSING = 15,
    };

  }  // namespace state

  /// 转换 ID 枚举
  namespace transition
  {
    enum : unsigned int
    {
      /// 外部触发转换
      TRANSITION_CONFIGURE = 0,
      TRANSITION_CLEANUP = 1,
      TRANSITION_ACTIVATE = 2,
      TRANSITION_DEACTIVATE = 3,
      TRANSITION_UNCONFIGURED_SHUTDOWN = 4,
      TRANSITION_INACTIVE_SHUTDOWN = 5,
      TRANSITION_ACTIVE_SHUTDOWN = 6,

      /// configure 结果
      TRANSITION_ON_CONFIGURE_SUCCESS = 10,
      TRANSITION_ON_CONFIGURE_FAILURE = 11,
      TRANSITION_ON_CONFIGURE_ERROR = 12,

      /// cleanup 结果
      TRANSITION_ON_CLEANUP_SUCCESS = 20,
      TRANSITION_ON_CLEANUP_FAILURE = 21,
      TRANSITION_ON_CLEANUP_ERROR = 22,

      /// activate 结果
      TRANSITION_ON_ACTIVATE_SUCCESS = 30,
      TRANSITION_ON_ACTIVATE_FAILURE = 31,
      TRANSITION_ON_ACTIVATE_ERROR = 32,

      /// deactivate 结果
      TRANSITION_ON_DEACTIVATE_SUCCESS = 40,
      TRANSITION_ON_DEACTIVATE_FAILURE = 41,
      TRANSITION_ON_DEACTIVATE_ERROR = 42,

      /// shutdown 结果
      TRANSITION_ON_SHUTDOWN_SUCCESS = 50,
      TRANSITION_ON_SHUTDOWN_FAILURE = 51,
      TRANSITION_ON_SHUTDOWN_ERROR = 52,

      /// error 结果
      TRANSITION_ON_ERROR_SUCCESS = 60,
      TRANSITION_ON_ERROR_FAILURE = 61,
      TRANSITION_ON_ERROR_ERROR = 62,
    };

  }  // namespace transition
}  // namespace lifecycle