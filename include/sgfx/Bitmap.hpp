/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef SAPI_SGFX_BITMAP_HPP_
#define SAPI_SGFX_BITMAP_HPP_

#include <sapi/sg.h>

#include "../api/SgfxObject.hpp"
#include "../chrono/MicroTime.hpp"
#include "../fs/File.hpp"
#include "../var/Data.hpp"
#include "../var/Vector.hpp"
#include "Palette.hpp"
#include "Pen.hpp"
#include "Region.hpp"

namespace sgfx {

class AntiAliasFilter {
public:
  int initialize(var::Array<u8, 8> contrast_map);

  const sg_antialias_filter_t *filter() const { return &m_filter; }

private:
  sg_antialias_filter_t m_filter
#if defined __win32

#else
    = {0}
#endif
    ;
};

/*! \brief Bitmap Class
 * \details This class implements a bitmap and is
 * powered by the sgfx library.
 */
class Bitmap : public var::Data, public api::SgfxObject {
public:
  using BitsPerPixel = arg::Argument<u8, struct BitmapBitsPerPixel>;
  using ReadOnlyBuffer = var::Reference::ReadOnlyBuffer;
  using ReadWriteBuffer = var::Reference::ReadWriteBuffer;

  using IsReadOnly = arg::Argument<bool, struct BitmapIsReadOnlyTag>;

  /*! \details Constructs an empty bitmap. */
  Bitmap();
  virtual ~Bitmap();

  u8 bits_per_pixel() const { return m_bmap.bits_per_pixel; }

  int set_bits_per_pixel(u8 bits_per_pixel);

  operator const sg_bmap_t *() const { return &m_bmap; }

  /*! \details Constructs a bitmap using an existing sg_bmap_header_t.
   *
   * @param hdr A pointer to the existing bitmap structure
   * @param readonly True if the data is stored in read-only memory
   */
  Bitmap(
    const sg_bmap_header_t *hdr,
    IsReadOnly is_read_only = IsReadOnly(false)); // read/write bitmap

  /*! \details Constructs a bitmap using an existing
   * read-only memory buffer. The button can be
   * nullptr in which case, the Bitmap will just
   * be an area reference that cannot be drawn on.
   *
   */
  Bitmap(
    ReadOnlyBuffer buffer,
    const Area &area,
    BitsPerPixel bpp = BitsPerPixel(1));

  /*! \details Constructs a bitmap using an existing
   * read-write memory buffer.
   */
  Bitmap(
    ReadWriteBuffer buffer,
    const Area &area,
    BitsPerPixel bpp = BitsPerPixel(1));

  /*! \details Constructs a new bitmap (dynamic memory allocation).
   *
   * @param area Dimensions of the bitmap
   * @param bits_per_pixel Number of bits per pixel (if not fixed by the
   * library)
   */
  Bitmap(const Area &area, BitsPerPixel bits_per_pixel);

  Bitmap(const Bitmap &bitmap) : var::Data(bitmap) {
    m_bmap = bitmap.m_bmap;
    m_bmap.data = to<sg_bmap_data_t>();
  }

  Bitmap &operator=(const Bitmap &bitmap) {
    copy_contents(bitmap);
    m_bmap = bitmap.m_bmap;
    m_bmap.data = to<sg_bmap_data_t>();
    return *this;
  }

  Bitmap(Bitmap &&bitmap) : var::Data(std::move(bitmap)) {
    std::swap(m_bmap, bitmap.m_bmap);
  }

  Bitmap &operator<<(const Pen &pen) {
    m_bmap.pen = pen;
    return *this;
  }

  Bitmap create_reference(const Region &region);

  /*! \details Assigns a pen to the bitmap.
   *
   * The pen needs to be assigned before doing
   * any draw operations. The pen affects
   * how objects are drawn on the bitmap.
   *
   */
  Bitmap &set_pen(const Pen &pen) {
    m_bmap.pen = pen.pen();
    return *this;
  }

  const Pen pen() const { return Pen(m_bmap.pen); }

  Region get_viewable_region() const;

  /*! \details Sets data pointer and size for bitmap */
  void refer_to(
    const sg_bmap_header_t *hdr,
    IsReadOnly is_read_only = IsReadOnly(false));

  /*! \details Sets the data pointer based on the
   * area of the read-only bitmap.
   *
   */
  void refer_to(ReadOnlyBuffer buffer, const Area &area, BitsPerPixel bpp);

  /*! \details Sets the data pointer based on the
   * area of the bitmap.
   *
   */
  void refer_to(ReadWriteBuffer buffer, const Area &area, BitsPerPixel bpp);

  /*! \details Changes effective size without free/allocate sequence */
  bool resize(const Area &area);

  /*! \details Returns the number of bytes used to store a Bitmap of specified
   * size
   *
   * @param w Width used for calculation
   * @param h Height used for calculation
   */
  u32 calculate_size(const Area &area) const {
    return api()->calc_bmap_size(bmap(), area);
  }

  /*! \details Returns the number of bytes used to store the Bitmap. */
  u32 calculate_size() const { return calculate_size(area()); }

  Point center() const;

  /*! \details Returns the maximum x value. */
  sg_int_t x_max() const { return width() - 1; }
  /*! \details Returns the maximum y value. */
  sg_int_t y_max() const { return height() - 1; }

  static Area load_area(const var::String &path);

  /*! \details Loads a bitmap from a file.
   *
   * @param path The path to the bitmap file name
   * @return Zero on success
   */
  int load(const var::String &path);

  /*! \details Saves a bitmap to a file.
   *
   * @param path The path for the new file
   * @return Zero on success
   *
   * If the file already exists, it will be overwritten.
   *
   */
  int save(const var::String &path) const;

  /*! \details Allocates memory for the bitmap data using the specified
   * width and height.  If the bitmap already has a memory associated
   * with it, it will be freed before the new memory is assigned.
   */
  int allocate(const Area &d, BitsPerPixel bpp = BitsPerPixel(1));

  /*! \details Free memory associated with bitmap (auto freed on ~Bitmap) */
  int free();

  void transform_flip_x() const { api()->transform_flip_x(bmap()); }
  void transform_flip_y() const { api()->transform_flip_y(bmap()); }
  void transform_flip_xy() const { api()->transform_flip_xy(bmap()); }

  /*! \details Performs a shift operation on an area of the bitmap.
   *
   * @param shift The amount to shift in each direction
   * @param region The region to shift
   *
   * The shifting must respect the dimensions of the bitmap. If you want to
   * shift the entire bitmap to the left 8 pixels, do this:
   *
   * \code
   * 	Bitmap bmap(64,64);
   * 	bmap.transform_shift(sg_point(-8,0), sg_point(8,0), sg_dim(bmap.width()
   * - 8, bmap.height)); \endcode
   *
   *
   */
  void transform_shift(sg_point_t shift, const sg_region_t &region) const {
    api()->transform_shift(bmap(), shift, &region);
  }
  void transform_shift(sg_point_t shift, sg_point_t p, sg_area_t d) const {
    transform_shift(shift, Region(p, d));
  }

  /*! \details Gets the value of the pixel at the specified point.
   *
   * @param p The point to get the pixel color
   * @return The color of the pixel at \a p
   */
  sg_color_t get_pixel(const Point &p) const {
    return api()->get_pixel(bmap(), p);
  }

  /*! \details Draws a pixel at the specified point.
   *
   * @param p The point where to draw the pixel
   *
   * The color of the pixel is determined by the pen settings.
   *
   * \sa set_pen_color()
   */
  void draw_pixel(const Point &p) const { api()->draw_pixel(bmap(), p); }

  /*! \details Draws a line on the bitmap.
   *
   * @param p1 Starting point
   * @param p2 Ending point
   *
   * The bitmap's pen will determine the color, thickness, and drawing mode.
   *
   */
  void draw_line(const Point &p1, const Point &p2) const {
    api()->draw_line(bmap(), p1, p2);
  }

  void draw_quadratic_bezier(
    const Point &p1,
    const Point &p2,
    const Point &p3,
    sg_point_t *corners = 0) const {
    api()->draw_quadratic_bezier(bmap(), p1, p2, p3, corners);
  }

  void draw_cubic_bezier(
    const Point &p1,
    const Point &p2,
    const Point &p3,
    const Point &p4,
    sg_point_t *corners = 0) const {
    api()->draw_cubic_bezier(bmap(), p1, p2, p3, p4, corners);
  }

  void draw_arc(
    const Region &region,
    s16 start,
    s16 end,
    s16 rotation = 0,
    sg_point_t *corners = 0) const {
    api()->draw_arc(bmap(), &region.region(), start, end, rotation, corners);
  }

  void
  draw_arc(const Point &p, const Area &d, s16 start, s16 end, s16 rotation = 0)
    const {
    draw_arc(Region(p, d), start, end, rotation);
  }

  /*! \details Draws a rectangle on the bitmap.
   *
   * @param region The region to draw in
   *
   * The bitmap's pen color and drawing mode will affect how the rectangle is
   * drawn. This method affects every pixel in the rectangle not just the
   * border.
   */
  void draw_rectangle(const Region &region) const {
    api()->draw_rectangle(bmap(), &region.region());
  }

  void draw_rectangle(const Point &p, const Area &d) const {
    draw_rectangle(Region(p, d));
  }

  /*! \details Pours an area on the bitmap.
   *
   * @param point Center of the pour
   * @param bounds Bounds for the pour
   *
   * The pour will seek boundaries going outward until it hits
   * a non-zero color or hits the bounding box.
   */
  void draw_pour(const Point &point, const Region &bounds) const {
    api()->draw_pour(bmap(), point, &bounds.region());
  }

  /*! \details This function sets the pixels in a bitmap
   * based on the pixels of the source bitmap
   *
   * @param p_dest The point in the destination bitmap of the top left corner of
   * \a bitmap
   * @param src The source bitmap
   * @return Zero on success
   */
  void draw_bitmap(const Point &p_dest, const Bitmap &src) const {
    api()->draw_bitmap(bmap(), p_dest, src.bmap());
  }

  void
  apply_antialias_filter(const AntiAliasFilter &filter, const Region &bounds) {
    api()->antialias_filter_apply(bmap(), filter.filter(), bounds);
  }

  /*!
   * \details Draws the source bitmap by down sampling by factor
   * \param source The bitmap to draw
   * \param factor number to downsample by with different values for width and
   * height (2 means have the size of source)
   *
   *
   */
  void downsample_bitmap(const Bitmap &source, const Area &factor);

  /*! \details This function draws a pattern on the bitmap.
   *
   * @param region The region to draw the pattern in
   * @param odd_pattern The odd pattern as a 1bpp bitmask (e.g. 0xAA)
   * @param even_pattern The even pattern as a 1bpp bitmask (e.g. 0x55)
   * @param pattern_height The pixel height of alternating pixels
   */
  void draw_pattern(
    const Region &region,
    sg_bmap_data_t odd_pattern,
    sg_bmap_data_t even_pattern,
    sg_size_t pattern_height) const {
    api()->draw_pattern(
      bmap(),
      &region.region(),
      odd_pattern,
      even_pattern,
      pattern_height);
  }
  void draw_pattern(
    const Point &p,
    const Area &d,
    sg_bmap_data_t odd_pattern,
    sg_bmap_data_t even_pattern,
    sg_size_t pattern_height) const {
    draw_pattern(Region(p, d), odd_pattern, even_pattern, pattern_height);
  }

  /*! \details This function draws a subset of
   * the source bitmap on the destination bitmap.
   *
   * @param p_dest The point in the destination bitmap to start setting pixels
   * @param src The source bitmap
   * @param region_src The regions of the source bitmap to draw
   * @return Zero on success
   */
  void draw_sub_bitmap(
    const Point &destination_point,
    const Bitmap &source_bitmap,
    const Region &source_region) const {
    api()->draw_sub_bitmap(
      bmap(),
      destination_point,
      source_bitmap.bmap(),
      &source_region.region());
  }

  void draw_sub_bitmap(
    const Point &destination_point,
    const Bitmap &source_bitmap,
    const Point &source_point,
    const Area &source_area) const {
    draw_sub_bitmap(
      destination_point,
      source_bitmap,
      Region(source_point, source_area));
  }

  Region calculate_active_region() const;

  // these are deprecated and shouldn't be documented?
  void invert() { invert_rectangle(sg_point(0, 0), area()); }
  void invert_rectangle(const Point &p, const Area &d) {
    sg_region_t region = sg_region(p, d);
    m_bmap.pen.o_flags = SG_PEN_FLAG_IS_INVERT;
    api()->draw_rectangle(bmap(), &region);
  }

  void clear_rectangle(const Point &p, const Area &d) {
    sg_region_t region = sg_region(p, d);
    m_bmap.pen.o_flags = SG_PEN_FLAG_IS_ERASE;
    api()->draw_rectangle(bmap(), &region);
  }

  /*! \details This method is designated as an interface
   * for classes that inherit Bitmap to copy the bitmap to a physical
   * device (such as hal::DisplayDev).  The implementation in this class is
   * simple an empty method.  Here is an example:
   *
   * \code
   * DisplayDev display; //this class inherits bitmap and re-implments refresh()
   *
   * display.clear(); //clear the bitmap in memory
   * display.refresh(); //copy the memory to the physical device (or notify the
   * underlying driver that it is time to copy) \endcode
   *
   */
  virtual void refresh() const {}

  /*! \details This method is designated as an interface for classes
   * that inherit Bitmap and use refresh() to copy the bitmap memory to a
   * physical device. The default implementation always returns false.
   *
   * The bitmap should not be modified while a refresh is in progress to prevent
   * a frame from being partially copied.
   *
   * See refresh() for an example.
   *
   * @return True if the refresh() is still in progress, false if the bitmap can
   * be modified again
   */
  virtual bool is_busy() const { return false; }

  /*! \details This method will block until the refresh operation is complete */
  virtual void wait(const chrono::MicroTime &resolution) const {}

  bool is_empty(const Region &region) const;

  sg_size_t height() const { return m_bmap.area.height; }
  sg_size_t width() const { return m_bmap.area.width; }

  u32 color_count() const;

  Area area() const { return m_bmap.area; }

  Region region() const { return Region(Point(), m_bmap.area); }

  inline sg_size_t columns() const { return m_bmap.columns; }

  inline sg_size_t margin_left() const { return m_bmap.margin_top_left.width; }
  inline sg_size_t margin_right() const {
    return m_bmap.margin_bottom_right.width;
  }
  inline sg_size_t margin_top() const { return m_bmap.margin_top_left.height; }
  inline sg_size_t margin_bottom() const {
    return m_bmap.margin_bottom_right.height;
  }

  inline void set_margin_left(sg_size_t v) { m_bmap.margin_top_left.width = v; }
  inline void set_margin_right(sg_size_t v) {
    m_bmap.margin_bottom_right.width = v;
  }
  inline void set_margin_top(sg_size_t v) { m_bmap.margin_top_left.height = v; }
  inline void set_margin_bottom(sg_size_t v) {
    m_bmap.margin_bottom_right.height = v;
  }
  inline void set_margin(sg_size_t v) {
    set_margin_bottom(v);
    set_margin_left(v);
    set_margin_right(v);
    set_margin_top(v);
  }

  const sg_bmap_data_t *bmap_data(const Point &p) const;
  sg_bmap_data_t *bmap_data(const Point &p);

  sg_bmap_t *bmap() { return &m_bmap; }
  const sg_bmap_t *bmap() const { return &m_bmap; }

protected:
private:
  sg_bmap_t m_bmap = {0};

  sg_color_t calculate_color_sum();
  int set_internal_bits_per_pixel(u8 bpp);
  void initialize_members();
  void calculate_members(const Area &dim);
};

} // namespace sgfx

namespace sys {
class Printer;
Printer &operator<<(Printer &printer, const sgfx::Bitmap &a);
Printer &operator<<(Printer &printer, const sgfx::Point &a);
Printer &operator<<(Printer &printer, const sgfx::Region &a);
Printer &operator<<(Printer &printer, const sgfx::Area &a);
Printer &operator<<(Printer &printer, const sgfx::Pen &a);
} // namespace sys

#endif /* SAPI_SGFX_BITMAP_HPP_ */
