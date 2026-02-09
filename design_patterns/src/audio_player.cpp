#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// 仅前向声明上下文类（避免循环依赖）
class AudioPlayer;

// 状态基类（抽象类）- 完整定义
class State
{
 protected:
  AudioPlayer* player;

 public:
  explicit State(AudioPlayer* p) : player(p) {}
  virtual ~State() = default;

  // 只声明纯虚函数，实现放在所有类定义之后
  virtual void clickLock() = 0;
  virtual void clickPlay() = 0;
  virtual void clickNext() = 0;
  virtual void clickPrevious() = 0;
};

// 音频播放器类（上下文）- 完整定义
class AudioPlayer
{
 private:
  std::unique_ptr<State> state;
  int volume{50};
  std::vector<std::string> playlist{"Song 1", "Song 2", "Song 3"};
  std::string currentSong{""};
  bool isPlaying{false};

 public:
  AudioPlayer();  // 构造函数声明，实现放最后

  // 状态切换核心方法
  void changeState(std::unique_ptr<State> newState)
  {
    state = std::move(newState);
    std::cout << "[State Changed] Current state updated" << std::endl;
  }

  // UI 交互方法（委托给状态）
  void clickLock() { state->clickLock(); }
  void clickPlay() { state->clickPlay(); }
  void clickNext() { state->clickNext(); }
  void clickPrevious() { state->clickPrevious(); }

  // 核心业务方法（供状态类调用）
  void startPlayback()
  {
    if (!isPlaying && !currentSong.empty())
    {
      isPlaying = true;
      std::cout << "[Playback] Start playing: " << currentSong << std::endl;
    }
    else if (currentSong.empty())
    {
      currentSong = playlist[0];
      isPlaying = true;
      std::cout << "[Playback] Start playing first song: " << currentSong
                << std::endl;
    }
  }

  void stopPlayback()
  {
    if (isPlaying)
    {
      isPlaying = false;
      std::cout << "[Playback] Stop playing: " << currentSong << std::endl;
    }
  }

  void nextSong()
  {
    auto it = std::find(playlist.begin(), playlist.end(), currentSong);
    if (it != playlist.end() && std::next(it) != playlist.end())
    {
      currentSong = *std::next(it);
      std::cout << "[Song] Switch to next song: " << currentSong << std::endl;
      if (isPlaying) startPlayback();
    }
    else
    {
      std::cout << "[Song] Already at last song" << std::endl;
    }
  }

  void previousSong()
  {
    auto it = std::find(playlist.begin(), playlist.end(), currentSong);
    if (it != playlist.begin())
    {
      currentSong = *std::prev(it);
      std::cout << "[Song] Switch to previous song: " << currentSong
                << std::endl;
      if (isPlaying) startPlayback();
    }
    else
    {
      std::cout << "[Song] Already at first song" << std::endl;
    }
  }

  void fastForward(int seconds)
  {
    std::cout << "[Playback] Fast forward " << seconds << " seconds"
              << std::endl;
  }

  void rewind(int seconds)
  {
    std::cout << "[Playback] Rewind " << seconds << " seconds" << std::endl;
  }

  // 供状态类判断播放状态
  bool isPlayingState() const { return isPlaying; }
  void setCurrentSong(const std::string& song) { currentSong = song; }
};

// 先声明所有状态类（仅声明，不定义）
class LockedState;
class ReadyState;
class PlayingState;

// ==================== 状态类完整定义 ====================
// 1. 就绪状态（ReadyState）
class ReadyState : public State
{
 public:
  using State::State;

  // 实现纯虚函数（此时所有状态类已声明，AudioPlayer 已完整定义）
  void clickLock() override;
  void clickPlay() override;
  void clickNext() override;
  void clickPrevious() override;
};

// 2. 播放状态（PlayingState）
class PlayingState : public State
{
 public:
  using State::State;

  void clickLock() override;
  void clickPlay() override;
  void clickNext() override;
  void clickPrevious() override;
};

// 3. 锁定状态（LockedState）
class LockedState : public State
{
 public:
  using State::State;

  void clickLock() override;
  void clickPlay() override;
  void clickNext() override;
  void clickPrevious() override;
};

// ==================== 状态类方法实现（关键！放所有类定义之后）
// ==================== ReadyState 方法实现
void ReadyState::clickLock()
{
  std::cout << "[ReadyState] Click Lock: Locking player" << std::endl;
  player->changeState(std::unique_ptr<State>(new LockedState(player)));
}

void ReadyState::clickPlay()
{
  std::cout << "[ReadyState] Click Play: Starting playback" << std::endl;
  player->startPlayback();
  player->changeState(std::unique_ptr<State>(new PlayingState(player)));
}

void ReadyState::clickNext()
{
  std::cout << "[ReadyState] Click Next: Switch to next song" << std::endl;
  player->nextSong();
}

void ReadyState::clickPrevious()
{
  std::cout << "[ReadyState] Click Previous: Switch to previous song"
            << std::endl;
  player->previousSong();
}

// PlayingState 方法实现
void PlayingState::clickLock()
{
  std::cout << "[PlayingState] Click Lock: Locking player" << std::endl;
  player->changeState(std::unique_ptr<State>(new LockedState(player)));
}

void PlayingState::clickPlay()
{
  std::cout << "[PlayingState] Click Play: Stopping playback" << std::endl;
  player->stopPlayback();
  player->changeState(std::unique_ptr<State>(new ReadyState(player)));
}

void PlayingState::clickNext()
{
  std::cout
      << "[PlayingState] Click Next: Double click detected, switch to next song"
      << std::endl;
  player->nextSong();
}

void PlayingState::clickPrevious()
{
  std::cout << "[PlayingState] Click Previous: Double click detected, switch "
               "to previous song"
            << std::endl;
  player->previousSong();
}

// LockedState 方法实现
void LockedState::clickLock()
{
  std::cout << "[LockedState] Click Lock: Unlocking player" << std::endl;
  if (player->isPlayingState())
  {
    player->changeState(std::unique_ptr<State>(new PlayingState(player)));
  }
  else
  {
    player->changeState(std::unique_ptr<State>(new ReadyState(player)));
  }
}

void LockedState::clickPlay()
{
  std::cout << "[LockedState] Click Play: Player is locked, do nothing"
            << std::endl;
}

void LockedState::clickNext()
{
  std::cout << "[LockedState] Click Next: Player is locked, do nothing"
            << std::endl;
}

void LockedState::clickPrevious()
{
  std::cout << "[LockedState] Click Previous: Player is locked, do nothing"
            << std::endl;
}

// ==================== AudioPlayer 构造函数实现（最后） ====================
AudioPlayer::AudioPlayer()
{
  changeState(std::unique_ptr<State>(new ReadyState(this)));
}

// ==================== 测试代码 ====================
int main()
{
  AudioPlayer player;

  std::cout << "===== Step 1: Click Play =====" << std::endl;
  player.clickPlay();

  std::cout << "\n===== Step 2: Click Next =====" << std::endl;
  player.clickNext();

  std::cout << "\n===== Step 3: Click Lock =====" << std::endl;
  player.clickLock();

  std::cout << "\n===== Step 4: Click Play (Locked) =====" << std::endl;
  player.clickPlay();

  std::cout << "\n===== Step 5: Unlock Player =====" << std::endl;
  player.clickLock();

  std::cout << "\n===== Step 6: Pause Playback =====" << std::endl;
  player.clickPlay();

  return 0;
}