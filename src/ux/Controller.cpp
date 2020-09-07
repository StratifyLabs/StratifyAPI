#include "ux/EventLoop.hpp"
#include "ux/Model.hpp"

#include "ux/Controller.hpp"

using namespace ux;

Model &Controller::model() { return m_event_loop.model(); }
const Model &Controller::model() const { return m_event_loop.model(); }

void Controller::transition(Layout &next_component) {
  m_next_layout = &next_component;
}

void Controller::distribute_event(const ux::Event &event) {
  handle_event(event);

  if (m_current_layout) {
    m_current_layout->distribute_event(event);
    refresh_drawing(m_current_layout);
  }

  if (m_next_layout && (event.type() == SystemEvent::event_type())) {

    sys::Printer p;

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

void Controller::refresh_drawing(Layout *layout) {
  // draw components
  for (Layout::Item &item : layout->component_list()) {
    if (
      item.component() && item.component()->is_enabled()
      && item.component()->is_refresh_drawing_pending()) {
      if (item.component()->is_layout() == false) {
        item.component()->refresh_drawing();
      }
    }
  }

  // draw layouts
  for (Layout::Item &item : layout->component_list()) {
    if (
      item.component() && item.component()->is_enabled()
      && item.component()->is_refresh_drawing_pending()) {

      if (item.component()->is_layout()) {
        this->refresh_drawing(item.component()->reinterpret<Layout>());
        item.component()->clear_refresh_drawing_pending();
      }
    }
  }
}
