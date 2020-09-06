/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#ifndef SAPI_UX_COMPONENT_HPP
#define SAPI_UX_COMPONENT_HPP

#include "../api/WorkObject.hpp"
#include "../fs/Stat.hpp"
#include "../hal/Display.hpp"
#include "../sgfx/Theme.hpp"
#include "../var/String.hpp"
#include "Drawing.hpp"
#include "Event.hpp"
#include "Model.hpp"

namespace ux {

class EventLoop;
class Layout;

#define COMPONENT_ACCESS_CREATE()                                              \
  template <typename... Args> static T &create(Args... args) {                 \
    T *result = new T(args...);                                                \
    if (result == nullptr) {                                                   \
      printf("fatal: create failed!!!\n");                                     \
    }                                                                          \
    result->set_created();                                                     \
    return *result;                                                            \
  }

class Component : public Drawing {
  enum flags {
    flag_visible = (1 << 0),
    flag_enabled = (1 << 1),
    flag_created = (1 << 2),
    flag_busy = (1 << 3),
    flag_layout = (1 << 4),
    flag_refresh_drawing_pending = (1 << 5),
    flag_antialias = (1 << 6),
    flag_focus = (1 << 7)
  };

public:
  Component(const var::String &name) {
    set_name(name);
    set_drawing_point(DrawingPoint(0, 0));
    set_drawing_area(DrawingArea(1000, 1000));
    m_flags = (flag_enabled | flag_focus);
  }
  virtual ~Component();

  template <typename T> T *reinterpret() { return static_cast<T *>(this); }
  template <typename T> const T *reinterpret_const() const {
    return static_cast<const T *>(this);
  }

  static u32 whatis_signature() { return 0; }

  enum sgfx::Theme::styles theme_style() const { return m_theme_style; }

  enum sgfx::Theme::states theme_state() const { return m_theme_state; }

  bool is_visible() const { return m_flags & (flag_visible); }
  bool is_created() const { return m_flags & (flag_created); }
  bool is_busy() const { return m_flags & (flag_busy); }
  bool is_enabled() const { return m_flags & (flag_enabled); }
  bool is_focus() const { return m_flags & (flag_focus); }
  bool is_layout() const { return m_flags & (flag_layout); }
  bool is_antialias() const { return m_flags & (flag_antialias); }
  bool is_refresh_drawing_pending() const {
    return m_flags & (flag_refresh_drawing_pending);
  }

  const sgfx::Theme *theme() const;
  const hal::Display *display() const;
  hal::Display *display();

  // update the location of the component (allow animations)

  virtual void handle_event(const ux::Event &event) {}

  const var::String &name() const { return m_name; }

  void apply_antialias_filter(const DrawingAttributes &attributes);
  void apply_antialias_filter(const DrawingScaledAttributes &attributes);

  void redraw() {
    if (is_ready_to_draw()) {
      draw(local_drawing_attributes());
      set_refresh_drawing_pending();
    }
  }

  sgfx::Region region() const {
    return m_reference_drawing_attributes.calculate_region_on_bitmap();
  }

  DrawingPoint translate_point(const sgfx::Point &point);

  void erase();

  void set_refresh_drawing_pending() {
    m_flags |= (flag_refresh_drawing_pending);
  }

  EventLoop *event_loop() { return m_event_loop; }

  const EventLoop *event_loop() const { return m_event_loop; }

  Layout *parent() { return m_parent; }

  const Layout *parent() const { return m_parent; }

  const var::String &lookup_model_value();
  const var::String &lookup_model_value(const var::String &key);

  void update_model(const var::String &value);
  void update_model(const Model::Entry &entry);

  void set_drawing_area(const DrawingArea &drawing_area) {
    m_reference_drawing_attributes.set_area(drawing_area);
  }

  void set_drawing_point(const DrawingPoint &drawing_point) {
    m_reference_drawing_attributes.set_point(drawing_point);
  }

  void set_theme_style(enum sgfx::Theme::styles value) {
    m_theme_style = value;
  }

  void set_theme_state(enum sgfx::Theme::states value) {
    m_theme_state = value;
  }

  const DrawingAttributes &reference_drawing_attributes() const {
    return m_reference_drawing_attributes;
  }

  DrawingAttributes &reference_drawing_attributes() {
    return m_reference_drawing_attributes;
  }

  var::String get_class_name() const {
    return fs::FileInfo::parent_directory(name());
  }

  var::String get_instance_name() const { return fs::FileInfo::name(name()); }

protected:
  void set_visible(bool value = true) {
    value ? m_flags |= (flag_visible) : m_flags &= ~(flag_visible);
  }
  void set_busy(bool value = true) {
    value ? m_flags |= (flag_busy) : m_flags &= ~(flag_busy);
  }
  void set_created(bool value = true) {
    value ? m_flags |= (flag_created) : m_flags &= ~(flag_created);
  }

  void set_layout(bool value = true) {
    value ? m_flags |= (flag_layout) : m_flags &= ~(flag_layout);
  }
  void set_antialias(bool value = true) {
    value ? m_flags |= (flag_antialias) : m_flags &= ~(flag_antialias);
  }

  void set_focus_internal(bool value = true) {
    value ? m_flags |= (flag_focus) : m_flags &= ~(flag_focus);
  }

  void clear_refresh_drawing_pending() {
    m_flags &= ~(flag_refresh_drawing_pending);
  }

  virtual void examine_visibility();

  virtual void touch_drawing_attributes() {}

  virtual void set_refresh_region(const sgfx::Region &region) {
    m_refresh_region = region;
    if (region.width() * region.height() == 0) {
      set_visible_examine(false);
    }
  }

  virtual void set_enabled_examine(bool value = true) {
    if (is_enabled() != value) {
      if (value) {
        m_flags |= (flag_enabled);
      } else {
        m_flags &= ~(flag_enabled);
      }
      examine_visibility();
    }
  }

  virtual void set_visible_examine(bool value = true) {
    if (is_visible() != value) {
      if (value) {
        m_flags |= (flag_visible);
      } else {
        m_flags &= ~(flag_visible);
      }
      examine_visibility();
    }
  }

  void refresh_drawing();
  friend class Controller;
  friend class Layout;
  friend class LayoutComponent;

  const DrawingAttributes &local_drawing_attributes() const {
    return m_local_drawing_attributes;
  }

  void set_event_loop(EventLoop *value) { m_event_loop = value; }

  void set_parent(Layout *value) { m_parent = value; }

  void trigger_event(u32 event_type, u32 event_id);

  bool is_ready_to_draw() const { return is_enabled() && is_visible(); }

private:
  var::String m_name;
  // needs to know where on the display it is drawn
  DrawingAttributes m_reference_drawing_attributes;
  DrawingAttributes m_local_drawing_attributes;
  sgfx::Bitmap m_local_bitmap;
  enum sgfx::Theme::styles m_theme_style = sgfx::Theme::style_brand_primary;
  enum sgfx::Theme::states m_theme_state = sgfx::Theme::state_default;
  sgfx::Region m_refresh_region;
  Layout *m_parent;
  u32 m_flags;

  EventLoop *m_event_loop = nullptr;

  void set_name(const var::String &name) { m_name = name; }
};

#define COMPONENT_ACCESS_DERIVED(B, T)                                         \
  API_ACCESS_DERIVED_COMPOUND(B, T, DrawingArea, drawing_area)                 \
  API_ACCESS_DERIVED_COMPOUND(B, T, DrawingPoint, drawing_point)               \
  API_ACCESS_DERIVED_FUNDAMETAL(B, T, enum sgfx::Theme::styles, theme_style)   \
  API_ACCESS_DERIVED_FUNDAMETAL(B, T, enum sgfx::Theme::states, theme_state)   \
  API_ACCESS_DERIVED_FUNDAMETAL(B, T, Layout *, parent)                        \
  static T *match_component(const Event &event) {                              \
    if (event.type() == event_type()) {                                        \
      return reinterpret_cast<T *>(event.context());                           \
    }                                                                          \
    return nullptr;                                                            \
  }                                                                            \
  static T *match_component(const Event &event, u32 id) {                      \
    if ((event.type() == event_type()) && event.id() == id) {                  \
      return reinterpret_cast<T *>(event.context());                           \
    }                                                                          \
    return nullptr;                                                            \
  }                                                                            \
  T &set_enabled(bool value = true) {                                          \
    B::set_enabled_examine(value);                                             \
    return static_cast<T &>(*this);                                            \
  }                                                                            \
  T &set_drawing_area(drawing_size_t width, drawing_size_t height) {           \
    B::set_drawing_area(DrawingArea(width, height));                           \
    return static_cast<T &>(*this);                                            \
  }                                                                            \
  T &set_drawing_point(drawing_int_t x, drawing_int_t y) {                     \
    B::set_drawing_point(DrawingPoint(x, y));                                  \
    return static_cast<T &>(*this);                                            \
  }                                                                            \
  static u32 event_type() { return reinterpret_cast<u32>(event_type); }        \
  T &trigger_event(u32 id) {                                                   \
    B::trigger_event(event_type(), id);                                        \
    return static_cast<T &>(*this);                                            \
  }                                                                            \
  T &forward_event(bool is_forward, u32 id) {                                  \
    if (is_forward) {                                                          \
      return trigger_event(id);                                                \
    }                                                                          \
    return static_cast<T &>(*this);                                            \
  }

template <class T>
class ComponentAccess : public Component, public DrawingComponentProperties<T> {
public:
  ComponentAccess(const var::String &name) : Component(name) {
  }

  COMPONENT_ACCESS_DERIVED(Component, T)

  bool contains(const sgfx::Point &point) {
    return DrawingComponentProperties<T>::calculate_region_inside_margin(
             reference_drawing_attributes().calculate_region_on_bitmap())
      .contains(point);
  }

  COMPONENT_ACCESS_CREATE()

protected:
};

} // namespace ux

#endif // SAPI_UX_COMPONENT_HPP
