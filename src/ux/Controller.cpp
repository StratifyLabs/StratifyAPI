#include "ux/Controller.hpp"

using namespace ux;

void Controller::transition(Layout &next_component) {
  m_next_layout = &next_component;
}

void Controller::distribute_event(const ux::Event &event) {
  handle_event(event);

  if (m_current_layout) {
    m_current_layout->distribute_event(event);
  }

  if (m_next_layout && (event.type() == SystemEvent::event_type())) {

    if (m_current_layout) {
      // option to do transition animations here
      m_current_layout->set_enabled(false);
      if (m_current_layout->is_created()) {
        delete m_current_layout;
      }
    }

    m_current_layout = m_next_layout;
    m_next_layout = nullptr;
    m_current_layout->set_enabled_examine(true);
    m_current_layout->set_visible_examine(true);
  }
}
