#pragma once

// 包含必要的标准库头文件
#include <memory>
#include <string>
#include <vector>

// 前向声明（避免循环依赖，减少编译依赖）
class State;

// 音频播放器类（上下文）- 声明
class AudioPlayer : public std::enable_shared_from_this<AudioPlayer>
{
 public:
  AudioPlayer() = default;  // 构造函数声明

  void init();

  // 状态切换核心方法
  void changeState(std::unique_ptr<State> new_state);

  // UI 交互方法（委托给状态）
  void clickLock();
  void clickPlay();
  void clickNext();
  void clickPrevious();

  // 核心业务方法（供状态类调用）
  void startPlayback();
  void stopPlayback();
  void nextSong();
  void previousSong();
  void fastForward(int seconds);
  void rewind(int seconds);

  // 供状态类判断/设置状态的接口
  bool isPlayingState() const;
  void setCurrentSong(const std::string& song);

 private:
  std::unique_ptr<State> state_;
  int volume_{50};
  std::vector<std::string> play_list_{"Song 1", "Song 2", "Song 3"};
  std::string current_song_{""};
  bool is_playing_{false};
};

// 状态基类（抽象类）- 声明
class State
{
 public:
  explicit State(std::shared_ptr<AudioPlayer> p);
  virtual ~State() = default;

  // 纯虚函数声明
  virtual void clickLock() = 0;
  virtual void clickPlay() = 0;
  virtual void clickNext() = 0;
  virtual void clickPrevious() = 0;

 protected:
  std::weak_ptr<AudioPlayer> player_;
};

/**
 * using State::State; 的语法含义这是 C++11 引入的继承构造函数（Inheriting
 * Constructors） 语法，作用是：让派生类（如
 * ReadyState/PlayingState/LockedState）直接继承基类（State）的构造函数，无需手动重写和转发参数。
 */
// 具体状态类声明（前向声明 + 类定义）
class LockedState : public State
{
 public:
  using State::State;

  // 重写纯虚函数声明
  void clickLock() override;
  void clickPlay() override;
  void clickNext() override;
  void clickPrevious() override;
};

class ReadyState : public State
{
 public:
  using State::State;

  void clickLock() override;
  void clickPlay() override;
  void clickNext() override;
  void clickPrevious() override;
};

class PlayingState : public State
{
 public:
  using State::State;

  void clickLock() override;
  void clickPlay() override;
  void clickNext() override;
  void clickPrevious() override;
};
