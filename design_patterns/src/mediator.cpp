/**
 * @file mediator.h
 * @author cao zhang
 * @brief 设计模式-中介者模式
 * @version 0.1
 * @date 2026-03-08
 */

#include <iostream>
#include <memory>
#include <string>

// 前置声明：解决循环依赖问题
class Component;

// 中介者接口：声明组件通知中介者的方法
class Mediator
{
 public:
  virtual ~Mediator() = default;
  virtual void notify(Component* sender, const std::string& event) = 0;
};

// 组件基类：所有组件都继承此类，持有中介者引用
class Component
{
 protected:
  Mediator* dialog;  // 指向中介者的指针（非所有权，避免循环析构）

 public:
  // 构造函数：初始化中介者
  explicit Component(Mediator* mediator) : dialog(mediator) {}
  virtual ~Component() = default;

  // 点击事件：通知中介者
  virtual void click() { dialog->notify(this, "click"); }

  // 按键事件：通知中介者
  virtual void keypress() { dialog->notify(this, "keypress"); }
};

// 具体组件：按钮（Button）
class Button : public Component
{
 public:
  using Component::Component;  // 继承基类构造函数
  // 按钮无额外特有方法，直接使用基类的 click/keypress
};

// 具体组件：文本框（Textbox）
class Textbox : public Component
{
 private:
  std::string text;  // 文本框内容

 public:
  using Component::Component;

  // 设置文本框内容
  void setText(const std::string& content) { text = content; }

  // 获取文本框内容
  std::string getText() const { return text; }
};

// 具体组件：复选框（Checkbox）
class Checkbox : public Component
{
 private:
  bool checked = false;  // 复选框选中状态

 public:
  using Component::Component;

  // 勾选/取消勾选：通知中介者
  void check()
  {
    checked = !checked;  // 切换状态
    dialog->notify(this, "check");
  }

  // 获取选中状态
  bool isChecked() const { return checked; }
};

// 具体中介者：认证对话框（AuthenticationDialog）
class AuthenticationDialog : public Mediator
{
 private:
  std::string title_;  // 对话框标题
  // 所有组件（使用智能指针管理内存，避免内存泄漏）
  std::unique_ptr<Checkbox> loginOrRegisterChkBx_;
  std::unique_ptr<Textbox> loginUsername_, loginPassword_;
  std::unique_ptr<Textbox> registrationUsername_, registrationPassword_,
      registrationEmail_;
  std::unique_ptr<Button> okBtn_, cancelBtn_;

 public:
  // 构造函数：初始化所有组件，绑定当前中介者
  AuthenticationDialog()
  {
    // 创建组件，将当前对话框（中介者）传入组件构造函数
    loginOrRegisterChkBx_ = std::make_unique<Checkbox>(this);
    loginUsername_ = std::make_unique<Textbox>(this);
    loginPassword_ = std::make_unique<Textbox>(this);
    registrationUsername_ = std::make_unique<Textbox>(this);
    registrationPassword_ = std::make_unique<Textbox>(this);
    registrationEmail_ = std::make_unique<Textbox>(this);
    okBtn_ = std::make_unique<Button>(this);
    cancelBtn_ = std::make_unique<Button>(this);

    // 默认标题和初始状态（显示登录表单）
    title_ = "Log in";
    std::cout << "=== Initial Dialog State ===" << std::endl;
    std::cout << "Title: " << title_ << std::endl;
    std::cout << "Login form visible, Registration form hidden" << std::endl;
  }

  // 实现中介者的 notify 方法：处理组件事件
  void notify(Component* sender, const std::string& event) override
  {
    // 处理复选框勾选事件：切换登录/注册表单
    if (sender == loginOrRegisterChkBx_.get() && event == "check")
    {
      if (loginOrRegisterChkBx_->isChecked())
      {
        title_ = "Register";
        std::cout << "\n=== Switch to Registration Form ===" << std::endl;
        std::cout << "Title: " << title_ << std::endl;
        std::cout << "Registration form visible, Login form hidden"
                  << std::endl;
      }
      else
      {
        title_ = "Log in";
        std::cout << "\n=== Switch to Login Form ===" << std::endl;
        std::cout << "Title: " << title_ << std::endl;
        std::cout << "Login form visible, Registration form hidden"
                  << std::endl;
      }
    }

    // 处理确认按钮点击事件：验证登录/注册
    if (sender == okBtn_.get() && event == "click")
    {
      std::cout << "\n=== OK Button Clicked ===" << std::endl;
      if (loginOrRegisterChkBx_->isChecked())
      {
        // 注册逻辑
        std::string username = registrationUsername_->getText();
        std::string password = registrationPassword_->getText();
        std::string email = registrationEmail_->getText();
        if (username.empty() || password.empty() || email.empty())
        {
          std::cout << "Error: Registration fields cannot be empty!"
                    << std::endl;
        }
        else
        {
          std::cout << "Success: User " << username << " registered with email "
                    << email << std::endl;
          std::cout << "Auto-login this user..." << std::endl;
        }
      }
      else
      {
        // 登录逻辑
        std::string username = loginUsername_->getText();
        std::string password = loginPassword_->getText();
        if (username == "admin" && password == "123456")
        {
          std::cout << "Success: Login as " << username << std::endl;
        }
        else
        {
          std::cout << "Error: Invalid username or password!" << std::endl;
        }
      }
    }
  }

  // 提供对外接口：设置组件内容（模拟用户输入）
  void setLoginUsername(const std::string& username)
  {
    loginUsername_->setText(username);
  }

  void setLoginPassword(const std::string& password)
  {
    loginPassword_->setText(password);
  }

  void setRegistrationInfo(const std::string& username,
                           const std::string& password,
                           const std::string& email)
  {
    registrationUsername_->setText(username);
    registrationPassword_->setText(password);
    registrationEmail_->setText(email);
  }

  // 提供对外接口：触发组件事件（模拟用户操作）
  void toggleLoginRegisterCheckbox() { loginOrRegisterChkBx_->check(); }

  void clickOkButton() { okBtn_->click(); }
};

// 测试代码
int main()
{
  // 创建认证对话框（中介者）
  AuthenticationDialog dialog;

  // 模拟用户操作1：勾选复选框，切换到注册表单
  dialog.toggleLoginRegisterCheckbox();

  // 模拟用户操作2：填写注册信息并点击确认
  dialog.setRegistrationInfo("user123", "pass123", "user123@test.com");
  dialog.clickOkButton();

  // 模拟用户操作3：取消复选框，切换到登录表单
  dialog.toggleLoginRegisterCheckbox();

  // 模拟用户操作4：填写错误的登录信息
  dialog.setLoginUsername("admin");
  dialog.setLoginPassword("wrong");
  dialog.clickOkButton();

  // 模拟用户操作5：填写正确的登录信息
  dialog.setLoginPassword("123456");
  dialog.clickOkButton();

  return 0;
}
