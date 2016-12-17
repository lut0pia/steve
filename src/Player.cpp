#include "Player.h"

#include "Midi.h"

using namespace Steve;

void Player::start(const Music* music) {
  _music = music;
  _tickTime = _music->tickTime();
  _pos = 0;
  for(uint32_t i(0); i<_music->instruments().size(); i++)
    _midi.setInstrument(i,_music->instruments()[i]->midiId());
}
void Player::play() {
  uint64_t now(std::chrono::steady_clock::now().time_since_epoch().count()/(std::chrono::steady_clock::period::den/1000i64));
  if(!free() && now-_lastTick>=_tickTime) {
    _lastTick = now;
    Notes::const_iterator beg(_music->notes().lower_bound(_pos)),
      end(_music->notes().upper_bound(_pos));
    for(auto it(beg); it!=end; it++)
      if((*it).second.stop)
        _midi.stopNote((*it).second.channel,(*it).second.tone);
    for(auto it(beg); it!=end; it++)
      if(!(*it).second.stop)
        _midi.playNote((*it).second.channel,(*it).second.tone);
    if(++_pos > _music->size())
      stop();
  }
}
void Player::stop() {
  _midi.stopAll();
  _music = nullptr;
}
