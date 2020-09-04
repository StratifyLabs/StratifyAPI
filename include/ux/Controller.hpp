#ifndef SAPI_UX_CONTROLLER_HPP
#define SAPI_UX_CONTROLLER_HPP

#include "../api/WorkObject.hpp"
#include "Event.hpp"
#include "Layout.hpp"

namespace ux {

class EventLoop;
class Layout;

class Controller : public api::WorkObject {
public:
  explicit Controller(EventLoop *event_loop) : m_event_loop(event_loop) {}

  void transition(Layout &next_layout);
  void distribute_event(const ux::Event &event);
  virtual void handle_event(const ux::Event &event) {}

private:
  Layout *m_current_layout = nullptr;
  Layout *m_next_layout = nullptr;
  EventLoop *m_event_loop = nullptr;
};

} // namespace ux
#endif // SAPI_UX_CONTROLLER_HPP
