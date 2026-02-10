#include <algorithm>
#include <iostream>

#include "design_patterns/audio_player.h"

// ==================== State 基类构造函数实现 ====================
State::State(std::shared_ptr<AudioPlayer> p) : player_(p)
{
}

// 初始化默认状态为 ReadyState
void AudioPlayer::init()
{
  changeState(std::unique_ptr<State>(new ReadyState(shared_from_this())));
  std::cout << "[AudioPlayer] Initialized with ReadyState" << std::endl;
}

// ==================== AudioPlayer 成员函数实现 ====================
void AudioPlayer::changeState(std::unique_ptr<State> new_state)
{
  state_ = std::move(new_state);
  std::cout << "[State Changed] Current state updated" << std::endl;
}

void AudioPlayer::clickLock()
{
  state_->clickLock();
}
void AudioPlayer::clickPlay()
{
  state_->clickPlay();
}
void AudioPlayer::clickNext()
{
  state_->clickNext();
}
void AudioPlayer::clickPrevious()
{
  state_->clickPrevious();
}

void AudioPlayer::startPlayback()
{
  if (!is_playing_ && !current_song_.empty())
  {
    is_playing_ = true;
    std::cout << "[Playback] Start playing: " << current_song_ << std::endl;
  }
  else if (current_song_.empty())
  {
    current_song_ = play_list_[0];
    is_playing_ = true;
    std::cout << "[Playback] Start playing first song: " << current_song_
              << std::endl;
  }
}

void AudioPlayer::stopPlayback()
{
  if (is_playing_)
  {
    is_playing_ = false;
    std::cout << "[Playback] Stop playing: " << current_song_ << std::endl;
  }
}

void AudioPlayer::nextSong()
{
  auto it = std::find(play_list_.begin(), play_list_.end(), current_song_);
  if (it != play_list_.end() && std::next(it) != play_list_.end())
  {
    current_song_ = *std::next(it);
    std::cout << "[Song] Switch to next song: " << current_song_ << std::endl;
    if (is_playing_) startPlayback();
  }
  else
  {
    std::cout << "[Song] Already at last song" << std::endl;
  }
}

void AudioPlayer::previousSong()
{
  auto it = std::find(play_list_.begin(), play_list_.end(), current_song_);
  if (it != play_list_.begin())
  {
    current_song_ = *std::prev(it);
    std::cout << "[Song] Switch to previous song: " << current_song_
              << std::endl;
    if (is_playing_) startPlayback();
  }
  else
  {
    std::cout << "[Song] Already at first song" << std::endl;
  }
}

void AudioPlayer::fastForward(int seconds)
{
  std::cout << "[Playback] Fast forward " << seconds << " seconds" << std::endl;
}

void AudioPlayer::rewind(int seconds)
{
  std::cout << "[Playback] Rewind " << seconds << " seconds" << std::endl;
}

bool AudioPlayer::isPlayingState() const
{
  return is_playing_;
}
void AudioPlayer::setCurrentSong(const std::string& song)
{
  current_song_ = song;
}

// ==================== 具体状态类成员函数实现 ====================
// 1. ReadyState 实现
void ReadyState::clickLock()
{
  // 锁定 weak_ptr，检查 AudioPlayer 是否有效
  if (auto p = player_.lock())
  {
    std::cout << "[ReadyState] Click Lock: Locking player" << std::endl;
    p->changeState(std::unique_ptr<State>(new LockedState(p)));
  }
  else
  {
    std::cout << "[ReadyState] AudioPlayer has been destroyed!" << std::endl;
  }
}

void ReadyState::clickPlay()
{
  if (auto p = player_.lock())
  {
    std::cout << "[ReadyState] Click Play: Starting playback" << std::endl;
    p->startPlayback();
    p->changeState(std::unique_ptr<State>(new PlayingState(p)));
  }
  else
  {
    std::cout << "[ReadyState] AudioPlayer has been destroyed!" << std::endl;
  }
}

void ReadyState::clickNext()
{
  if (auto p = player_.lock())
  {
    std::cout << "[ReadyState] Click Next: Switch to next song" << std::endl;
    p->nextSong();
  }
  else
  {
    std::cout << "[ReadyState] AudioPlayer has been destroyed!" << std::endl;
  }
}

void ReadyState::clickPrevious()
{
  if (auto p = player_.lock())
  {
    std::cout << "[ReadyState] Click Previous: Switch to previous song"
              << std::endl;
    p->previousSong();
  }
  else
  {
    std::cout << "[ReadyState] AudioPlayer has been destroyed!" << std::endl;
  }
}

// 2. PlayingState 实现
void PlayingState::clickLock()
{
  if (auto p = player_.lock())
  {
    std::cout << "[PlayingState] Click Lock: Locking player" << std::endl;
    p->changeState(std::unique_ptr<State>(new LockedState(p)));
  }
  else
  {
    std::cout << "[PlayingState] AudioPlayer has been destroyed!" << std::endl;
  }
}

void PlayingState::clickPlay()
{
  if (auto p = player_.lock())
  {
    std::cout << "[PlayingState] Click Play: Stopping playback" << std::endl;
    p->stopPlayback();
    p->changeState(std::unique_ptr<State>(new ReadyState(p)));
  }
  else
  {
    std::cout << "[PlayingState] AudioPlayer has been destroyed!" << std::endl;
  }
}

void PlayingState::clickNext()
{
  if (auto p = player_.lock())
  {
    std::cout << "[PlayingState] Click Next: Double click detected, switch to "
                 "next song"
              << std::endl;
    p->nextSong();
  }
  else
  {
    std::cout << "[PlayingState] AudioPlayer has been destroyed!" << std::endl;
  }
}

void PlayingState::clickPrevious()
{
  if (auto p = player_.lock())
  {
    std::cout << "[PlayingState] Click Previous: Double click detected, switch "
                 "to previous song"
              << std::endl;
    p->previousSong();
  }
  else
  {
    std::cout << "[PlayingState] AudioPlayer has been destroyed!" << std::endl;
  }
}

// 3. LockedState 实现
void LockedState::clickLock()
{
  if (auto p = player_.lock())
  {
    std::cout << "[LockedState] Click Lock: Unlocking player" << std::endl;
    if (p->isPlayingState())
    {
      p->changeState(std::unique_ptr<State>(new PlayingState(p)));
    }
    else
    {
      p->changeState(std::unique_ptr<State>(new ReadyState(p)));
    }
  }
  else
  {
    std::cout << "[LockedState] AudioPlayer has been destroyed!" << std::endl;
  }
}

void LockedState::clickPlay()
{
  if (auto p = player_.lock())
  {
    std::cout << "[LockedState] Click Play: Player is locked, do nothing"
              << std::endl;
  }
  else
  {
    std::cout << "[LockedState] AudioPlayer has been destroyed!" << std::endl;
  }
}

void LockedState::clickNext()
{
  if (auto p = player_.lock())
  {
    std::cout << "[LockedState] Click Next: Player is locked, do nothing"
              << std::endl;
  }
  else
  {
    std::cout << "[LockedState] AudioPlayer has been destroyed!" << std::endl;
  }
}

void LockedState::clickPrevious()
{
  if (auto p = player_.lock())
  {
    std::cout << "[LockedState] Click Previous: Player is locked, do nothing"
              << std::endl;
  }
  else
  {
    std::cout << "[LockedState] AudioPlayer has been destroyed!" << std::endl;
  }
}

// ==================== 测试代码 ====================
int main()
{
  std::shared_ptr<AudioPlayer> player_ = std::make_shared<AudioPlayer>();
  player_->init();

  std::cout << "===== Step 1: Click Play =====" << std::endl;
  player_->clickPlay();

  std::cout << "\n===== Step 2: Click Next =====" << std::endl;
  player_->clickNext();

  std::cout << "\n===== Step 3: Click Lock =====" << std::endl;
  player_->clickLock();

  std::cout << "\n===== Step 4: Click Play (Locked) =====" << std::endl;
  player_->clickPlay();

  std::cout << "\n===== Step 5: Unlock Player =====" << std::endl;
  player_->clickLock();

  std::cout << "\n===== Step 6: Pause Playback =====" << std::endl;
  player_->clickPlay();

  return 0;
}