/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
// LICENSE.md for rights.
#include "ux/Layout.hpp"
#include "sys/Printer.hpp"
#include "ux/EventLoop.hpp"

using namespace sgfx;
using namespace ux;

Layout::Layout(const var::String &name, EventLoop *event_loop)
  : ComponentAccess(name) {
  set_event_loop(event_loop);
  m_origin = DrawingPoint(0, 0);
  set_align_left();
  set_align_top();
  set_layout();
}

Layout::Layout(
  const var::String &prefix,
  const var::String &name,
  EventLoop *event_loop)
  : ComponentAccess(prefix + name) {
  set_event_loop(event_loop);
  m_origin = DrawingPoint(0, 0);
  set_align_left();
  set_align_top();
  set_layout();
}

Layout::~Layout() {
  for (Item &component_pointer : m_component_list) {
    if (
      component_pointer.component()
      && component_pointer.component()->is_created()) {
      delete component_pointer.component();
    }
  }
}

void Layout::examine_visibility() {
  if (is_ready_to_draw()) {
    if (is_created() == false) {
      printf(
        "fatal %s was note created using Component::create()\n",
        name().cstring());
      exit(1);
    }

    if (display() == nullptr) {
      set_visible_examine(false);
      return;
    }

    m_reference_drawing_attributes.set_bitmap(*display());
    m_refresh_region = Region(
      Point(0, 0),
      Region(reference_drawing_attributes().calculate_region_on_bitmap())
        .area());

    m_touch_gesture.set_region(
      reference_drawing_attributes().calculate_region_on_bitmap());

    shift_origin(DrawingPoint(0, 0));

    handle_event(SystemEvent(SystemEvent::event_id_entered));

  } else {

    // if layout is enabled and visible -- components are not visible
    for (Item &component_pointer : m_component_list) {
      if (component_pointer.component()) {
        component_pointer.component()->set_visible_examine(false);
      }
    }

    handle_event(SystemEvent(SystemEvent::event_id_exiteded));
  }
}

void Layout::set_refresh_region(const sgfx::Region &region) {
  if (is_ready_to_draw()) {
    sgfx::Region layout_region
      = reference_drawing_attributes().calculate_region_on_bitmap();

    layout_region
      = Region(layout_region.point() + region.point(), region.area());

    for (Item &component_pointer : m_component_list) {
      if (component_pointer.component()) {
        sgfx::Region component_region = component_pointer.component()
                                          ->reference_drawing_attributes()
                                          .calculate_region_on_bitmap();

        component_pointer.component()->set_refresh_region(
          layout_region.overlap(component_region));
      }
    }
  }
  m_refresh_region = region;
}

Layout &Layout::add_component(Component &component) {

  component.set_event_loop(event_loop());
  component.set_parent(this);

  // check to see if a cp has been deleting -- insert the new component there
  for (Item &cp : m_component_list) {
    if (cp.component() == nullptr) {
      cp.set_component(&component);
      return *this;
    }
  }

  m_component_list.push_back(Item(&component));
  return *this;
}

Layout &Layout::delete_component(const var::String &component_name) {

  for (Item &cp : m_component_list) {
    if (cp.component() && (component_name == cp.component()->name())) {
      cp.component()->set_enabled_examine(false);
      API_ASSERT(cp.component()->is_busy() == false);
      delete cp.component();
      cp.set_component(nullptr);
      cp.set_drawing_point(DrawingPoint(0, 0));
      cp.set_drawing_area(DrawingArea(0, 0));
      break;
    }
  }

  return *this;
}

void Layout::update_drawing_point(
  const Component *component,
  const DrawingPoint &point) {
  for (Item &cp : m_component_list) {
    if (cp.component() && component == cp.component()) {
      cp.set_drawing_point(point);
      shift_origin(DrawingPoint(0, 0));
      return;
    }
  }
}

void Layout::update_drawing_area(
  const Component *component,
  const DrawingArea &area) {
  for (Item &cp : m_component_list) {
    if (cp.component() && component == cp.component()) {
      cp.set_drawing_area(area);
      shift_origin(DrawingPoint(0, 0));
      return;
    }
  }
}

void Layout::shift_origin(DrawingPoint shift) {
  m_origin += shift;

  if (is_ready_to_draw()) {
    // determine scroll ends
    generate_layout_positions();

    sgfx::Region layout_region
      = reference_drawing_attributes().calculate_region_on_bitmap();

    layout_region = Region(
      layout_region.point() + m_refresh_region.point(),
      m_refresh_region.area());

    for (Item &item : m_component_list) {
      if (item.component()) {
        // reference attributes are the location within the compound component
        // translate reference attributes based on compound component attributes
        item.component()->reference_drawing_attributes()
          = reference_drawing_attributes() + m_origin + item.drawing_point()
            + item.drawing_area();
        sgfx::Region component_region = item.component()
                                          ->reference_drawing_attributes()
                                          .calculate_region_on_bitmap();

        sgfx::Region overlap = layout_region.overlap(component_region);

        if ((overlap.width() * overlap.height()) > 0) {
          // this calculates if only part of the element should be refreshed
          // (the mask)

          item.component()->set_refresh_drawing_pending();

          if (item.component()->is_visible()) {
            item.component()->touch_drawing_attributes();
          } else {
            item.component()->set_visible_examine(true);
          }

          item.component()->set_refresh_region(overlap);

        } else {
          item.component()->set_visible_examine(false);
        }
      }
    }
  }
}

DrawingPoint Layout::calculate_next_point(
  const DrawingPoint &point,
  const DrawingArea &area) {
  // depending on the layout, calculate the point of the next component
  DrawingPoint result(0, 0);
  for (Item &component_pointer : m_component_list) {
    if (component_pointer.component()) {
      if (m_flow == flow_vertical) {
        result += DrawingPoint::Y(component_pointer.drawing_area().height());
      } else if (m_flow == flow_horizontal) {
        result += DrawingPoint::X(component_pointer.drawing_area().width());
      }
    }
  }

  switch (m_flow) {
  case flow_free:
    result = point;
    break;
  case flow_vertical:
    // left,right,center alignment
    if (is_align_left()) {
      result.set_x(0);
    } else if (is_align_right()) {
      result.set_x(Drawing::scale() - area.width());
    } else {
      // center
      result.set_x((Drawing::scale() - area.width()) / 2);
    }
    break;
  case flow_horizontal:
    // top,bottom,middle alignment
    if (is_align_top()) {
      result.set_y(0);
    } else if (is_align_bottom()) {
      result.set_y(Drawing::scale() - area.height());
    } else {
      // middle
      result.set_y((Drawing::scale() - area.height()) / 2);
    }
    break;
  }

  return result;
}

void Layout::scroll(DrawingPoint value) {
  if (is_ready_to_draw()) {
    shift_origin(value);
  }
}

void Layout::draw(const DrawingAttributes &attributes) {
  MCU_UNUSED_ARGUMENT(attributes);
  printf("draw layout %s\n", name().cstring());
  for (const Item &item : m_component_list) {
    if (item.component() && item.component()->is_focus()) {
      item.component()->redraw();
    }
  }
}

Layout &Layout::set_focus(bool value) {
  set_focus_internal(value);
  for (const Item &item : m_component_list) {
    if (item.component()) {
      printf("set %s focus to %d\n", item.component()->name().cstring(), value);
      if (item.component()->is_layout()) {
        item.component()->reinterpret<Layout>()->set_focus(value);
      } else {
        item.component()->set_focus_internal(value);
      }
    }
  }
  return *this;
}

void Layout::distribute_event(const ux::Event &event) {
  // handle scrolling -- pass events to specific components
  set_busy();
  TouchContext *touch_context = TouchContext::match_component(event);
  if (touch_context) {
    enum TouchContext::event_ids touch_context_event_id
      = m_touch_gesture.process(event);
    switch (touch_context_event_id) {
    default:
      break;
    case TouchContext::event_id_dragged:
      drawing_int_t vertical_drawing_scroll;
      drawing_int_t horizontal_drawing_scroll;
      horizontal_drawing_scroll
        = handle_vertical_scroll(m_touch_gesture.drag().x());

      vertical_drawing_scroll
        = handle_vertical_scroll(m_touch_gesture.drag().y());

      if (vertical_drawing_scroll || horizontal_drawing_scroll) {
        this->scroll(
          DrawingPoint(horizontal_drawing_scroll, vertical_drawing_scroll));

        event_loop()->trigger_event(
          touch_context->event(TouchContext::event_id_dragged));
      }
      break;
    }
  }

  if (is_focus()) {
    handle_event(event);

    for (Item &item : m_component_list) {
      if (
        item.component() && item.component()->is_enabled()
        && item.component()->is_focus()) {
        if (item.component()->is_layout()) {
          item.component()->reinterpret<Layout>()->distribute_event(event);
        } else {
          item.component()->handle_event(event);

          if (item.component()->is_refresh_drawing_pending()) {
            item.component()->refresh_drawing();
          }
        }
      }
    }
  }

#if 0
  for (Item &item : m_component_list) {
    if (
      item.component() && item.component()->is_enabled()
      && item.component()->is_refresh_drawing_pending()) {

    }
  }
#endif

  if (event == SystemEvent(SystemEvent::event_id_exiteded)) {
    for (Item &component_pointer : m_component_list) {
      if (component_pointer.component()) {
        component_pointer.component()->set_visible_examine(false);
      }
    }
  }

  set_busy(false);
}

drawing_int_t Layout::handle_vertical_scroll(sg_int_t scroll) {
  // convert scroll to drawing scale

  drawing_int_t drawing_scroll
    = scroll * Drawing::scale()
      / reference_drawing_attributes().calculate_height_on_bitmap();

  if (drawing_scroll < 0) {

    drawing_int_t max_scroll = m_area.height() - 1000 + m_origin.y();
    if (drawing_scroll * -1 > max_scroll) {
      drawing_scroll = -1 * max_scroll;
    }

    if (m_origin.y() + m_area.height() <= 1000) {
      drawing_scroll = 0;
    }

  } else {
    if (drawing_scroll > m_origin.y() * -1) {
      drawing_scroll = m_origin.y() * -1;
    }
  }

  return drawing_scroll;
}

drawing_int_t Layout::handle_horizontal_scroll(sg_int_t scroll) { return 0; }

void Layout::generate_layout_positions() {
  switch (m_flow) {
  default:
  case flow_free:
    generate_free_layout_positions();
    return;
  case flow_vertical:
    generate_vertical_layout_positions();
    return;
  case flow_horizontal:
    generate_horizontal_layout_positions();
    return;
  }
}

void Layout::generate_free_layout_positions() {
  drawing_int_t x_max = 0;
  drawing_int_t y_max = 0;

  for (Item &component : m_component_list) {
    if (
      component.component()
      && (component.drawing_point().x() + component.drawing_area().width() > x_max)) {
      x_max = component.drawing_point().x() + component.drawing_area().width();
    }

    if (
      component.component()
      && (component.drawing_point().y() + component.drawing_area().height() > y_max)) {
      x_max = component.drawing_point().y() + component.drawing_area().height();
    }
  }

  m_area = DrawingArea(x_max, y_max);
}

void Layout::generate_vertical_layout_positions() {
  drawing_int_t drawing_cursor = 0;
  sg_int_t bitmap_cursor = 0;

  for (Item &item : m_component_list) {
    if (item.component()) {
      const DrawingPoint point(0, drawing_cursor);
      const DrawingArea area(1000, item.drawing_area().height());
      item.set_drawing_point(point);

      DrawingAttributes tmp_attributes
        = reference_drawing_attributes() + m_origin + point + area;

      sg_size_t height_on_bitmap
        = tmp_attributes.calculate_area_on_bitmap().height;

      bitmap_cursor += height_on_bitmap - 1;

      drawing_cursor
        = reference_drawing_attributes().calculate_height_on_drawing(
          bitmap_cursor);
    }
  }

  m_area.set_height(drawing_cursor);
}

void Layout::generate_horizontal_layout_positions() {
  drawing_int_t drawing_cursor = 0;
  sg_int_t bitmap_cursor = 0;

  for (Item &component : m_component_list) {
    if (component.component()) {

      const DrawingPoint point(drawing_cursor, 0);
      const DrawingArea area(component.drawing_area().width(), 1000);
      component.set_drawing_point(point);

      DrawingAttributes tmp_attributes = reference_drawing_attributes() + area;
      sg_size_t width_on_bitmap
        = tmp_attributes.calculate_area_on_bitmap().width;

      bitmap_cursor += width_on_bitmap - 1;
      drawing_cursor
        = reference_drawing_attributes().calculate_width_on_drawing(
          bitmap_cursor);
    }
  }

  m_area.set_width(drawing_cursor);
}
