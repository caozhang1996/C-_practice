// 状态机框架单元测试
//
// 验证生命周期流转:
//   Unconfigured --configure--> Inactive --activate--> Active
//   Active --deactivate--> Inactive --cleanup--> Unconfigured
//   Unconfigured --shutdown--> Finalized
//
// TestNode 持有一个 lifecycle::LifecycleNode 的 shared_ptr，
// 实现 onConfigure / onActivate 等回调，并通过 registerOnXxx 注册到节点上。

#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "state_machine/lifecycle.h"
#include "state_machine/lifecycle_node.h"

namespace
{
  std::string stateName(lifecycle::LifecycleState state)
  {
    switch (state)
    {
      case lifecycle::LifecycleState::PRIMARY_STATE_UNKNOWN:
        return "unknown";
      case lifecycle::LifecycleState::PRIMARY_STATE_UNCONFIGURED:
        return "unconfigured";
      case lifecycle::LifecycleState::PRIMARY_STATE_INACTIVE:
        return "inactive";
      case lifecycle::LifecycleState::PRIMARY_STATE_ACTIVE:
        return "active";
      case lifecycle::LifecycleState::PRIMARY_STATE_FINALIZED:
        return "finalized";
      case lifecycle::LifecycleState::TRANSITION_STATE_CONFIGURING:
        return "configuring";
      case lifecycle::LifecycleState::TRANSITION_STATE_CLEANINGUP:
        return "cleaningup";
      case lifecycle::LifecycleState::TRANSITION_STATE_SHUTTINGDOWN:
        return "shuttingdown";
      case lifecycle::LifecycleState::TRANSITION_STATE_ACTIVATING:
        return "activating";
      case lifecycle::LifecycleState::TRANSITION_STATE_DEACTIVATING:
        return "deactivating";
      case lifecycle::LifecycleState::TRANSITION_STATE_ERRORPROCESSING:
        return "errorprocessing";
    }

    return "unknown";
  }

  std::string returnCodeName(lifecycle::LifecycleNode::CallbackReturn code)
  {
    using CallbackReturn = lifecycle::LifecycleNode::CallbackReturn;
    switch (code)
    {
      case CallbackReturn::SUCCESS:
        return "SUCCESS";
      case CallbackReturn::FAILURE:
        return "FAILURE";
      case CallbackReturn::ERROR:
        return "ERROR";
    }

    return "ERROR";
  }

  int g_failures = 0;

  // 检查实际状态等于期望状态，失败则输出并计入 g_failures
  void expectState(const std::string& what,
                   lifecycle::LifecycleState actual,
                   lifecycle::LifecycleState expected)
  {
    if (actual != expected)
    {
      ++g_failures;
      std::cout << "  [FAIL] " << what << " 期望 " << stateName(expected)
                << "，实际 " << stateName(actual) << std::endl;
    }
    else
    {
      std::cout << "  [ OK ] " << what << " -> " << stateName(actual)
                << std::endl;
    }
  }

  // 检查非法转换被正确拒绝：状态保持不变（注意这不是"转换成功"，
  // 而是"转换被拒绝"，与 expectState 的 [ OK ] 含义区分开）
  void expectRejected(const std::string& what,
                      lifecycle::LifecycleState actual,
                      lifecycle::LifecycleState expect_unchanged)
  {
    if (actual != expect_unchanged)
    {
      ++g_failures;
      std::cout << "  [FAIL] " << what << " 应该被拒绝，但状态变成了 "
                << stateName(actual) << "（期望保持 " << stateName(expect_unchanged)
                << "）" << std::endl;
    }
    else
    {
      std::cout << "  [通过] " << what << " 已被拒绝，状态保持 "
                << stateName(actual) << "（符合预期）" << std::endl;
    }
  }

}  // namespace

/// 被测类：持有 lifecycle::LifecycleNode，实现并注册各个生命周期回调
class TestNode
{
 public:
  using CallbackReturn = lifecycle::LifecycleNode::CallbackReturn;

  explicit TestNode(const std::string& node_name)
      : node_(std::make_shared<lifecycle::LifecycleNode>(node_name))
  {
    registerCallbacks();
  }

  std::shared_ptr<lifecycle::LifecycleNode> getNode() const { return node_; }

 private:
  void registerCallbacks()
  {
    // 用生命周期节点的 registerOnXxx 注册本类实现的回调
    node_->registerOnConfigure(
        std::bind(&TestNode::onConfigure, this, std::placeholders::_1));

    node_->registerOnCleanup(
        std::bind(&TestNode::onCleanup, this, std::placeholders::_1));

    node_->registerOnShutdown(
        std::bind(&TestNode::onShutdown, this, std::placeholders::_1));

    node_->registerOnActivate(
        std::bind(&TestNode::onActivate, this, std::placeholders::_1));

    node_->registerOnDeactivate(
        std::bind(&TestNode::onDeactivate, this, std::placeholders::_1));

    node_->registerOnError(
        std::bind(&TestNode::onError, this, std::placeholders::_1));
  }

  // ── 生命周期回调 ──────────────────────────────────────────

  CallbackReturn onConfigure(const lifecycle::LifecycleState& previous_state)
  {
    std::cout << "  [TestNode] onConfigure 前一个状态: "
              << stateName(previous_state) << std::endl;
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn onCleanup(const lifecycle::LifecycleState& previous_state)
  {
    std::cout << "  [TestNode] onCleanup 前一个状态: "
              << stateName(previous_state) << std::endl;
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn onShutdown(const lifecycle::LifecycleState& previous_state)
  {
    std::cout << "  [TestNode] onShutdown 前一个状态: "
              << stateName(previous_state) << std::endl;
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn onActivate(const lifecycle::LifecycleState& previous_state)
  {
    std::cout << "  [TestNode] onActivate 前一个状态: "
              << stateName(previous_state) << std::endl;
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn onDeactivate(const lifecycle::LifecycleState& previous_state)
  {
    std::cout << "  [TestNode] onDeactivate 前一个状态: "
              << stateName(previous_state) << std::endl;
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn onError(const lifecycle::LifecycleState& previous_state)
  {
    std::cout << "  [TestNode] onError 前一个状态: "
              << stateName(previous_state) << std::endl;
    return CallbackReturn::SUCCESS;
  }

 private:
  std::shared_ptr<lifecycle::LifecycleNode> node_;
};

int main(int argc, char** argv)
{
  rclcpp::init(argc, argv);

  {
    TestNode test_node("test_lifecycle_node");
    auto node = test_node.getNode();

    std::cout << "\n=== 初始状态 ===" << std::endl;
    expectState("初始状态", node->getCurrentState(),
                lifecycle::LifecycleState::PRIMARY_STATE_UNCONFIGURED);

    std::cout << "\n=== configure: Unconfigured -> Inactive ===" << std::endl;
    {
      lifecycle::LifecycleNode::CallbackReturn rc;
      auto state = node->configure(rc);  // 带回调返回码的版本
      std::cout << "  回调返回码: " << returnCodeName(rc) << std::endl;
      expectState("configure()", state,
                  lifecycle::LifecycleState::PRIMARY_STATE_INACTIVE);
    }

    std::cout << "\n=== activate: Inactive -> Active ===" << std::endl;
    expectState("activate()", node->activate(),
                lifecycle::LifecycleState::PRIMARY_STATE_ACTIVE);

    std::cout << "\n=== deactivate: Active -> Inactive ===" << std::endl;
    expectState("deactivate()", node->deactivate(),
                lifecycle::LifecycleState::PRIMARY_STATE_INACTIVE);

    std::cout << "\n=== cleanup: Inactive -> Unconfigured ===" << std::endl;
    expectState("cleanup()", node->cleanup(),
                lifecycle::LifecycleState::PRIMARY_STATE_UNCONFIGURED);

    // 非法转换：unconfigured 状态下不能 activate，状态应保持不变
    std::cout << "\n=== 非法转换: Unconfigured 时直接 activate ==="
              << std::endl;
    expectRejected("activate()(非法)", node->activate(),
                   lifecycle::LifecycleState::PRIMARY_STATE_UNCONFIGURED);

    std::cout << "\n=== shutdown: Unconfigured -> Finalized ===" << std::endl;
    expectState("shutdown()", node->shutdown(),
                lifecycle::LifecycleState::PRIMARY_STATE_FINALIZED);

    // Finalized 之后再触发转换：均非法，状态保持 Finalized
    std::cout << "\n=== Finalized 状态 ===" << std::endl;
    expectRejected("cleanup()(非法)", node->cleanup(),
                   lifecycle::LifecycleState::PRIMARY_STATE_FINALIZED);
  }

  rclcpp::shutdown();

  std::cout << "\n==========================================" << std::endl;
  if (g_failures == 0)
  {
    std::cout << "全部测试通过 ✓" << std::endl;
    return 0;
  }
  std::cout << g_failures << " 项测试失败 ✗" << std::endl;
  return 1;
}