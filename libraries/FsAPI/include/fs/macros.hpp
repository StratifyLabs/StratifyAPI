#ifndef FSAPI_FS_MACROS_HPP
#define FSAPI_FS_MACROS_HPP

#if !defined __link
#define MCU_INT_CAST(var) ((void *)(u32)var)

#define FSAPI_LINK_DECLARE_DRIVER_NULLPTR
#define FSAPI_LINK_DECLARE_DRIVER_NULLPTR_LAST
#define FSAPI_LINK_DECLARE_DRIVER
#define FSAPI_LINK_DECLARE_DRIVER_LAST
#define FSAPI_LINK_STAT_STRUCT stat
#define FSAPI_LINK_DEFAULT_PAGE_SIZE 512
#define FSAPI_LINK_INHERIT_DRIVER
#define FSAPI_LINK_INHERIT_DRIVER_LAST
#define FSAPI_LINK_MEMBER_DRIVER
#define FSAPI_LINK_MEMBER_DRIVER_LAST
#define FSAPI_LINK_SET_DRIVER(x, y)
#else
#define FSAPI_LINK_STAT_STRUCT link_stat
#define FSAPI_LINK_DECLARE_DRIVER_NULLPTR                                      \
  link_transport_mdriver_t *link_driver = nullptr
#define FSAPI_LINK_DECLARE_DRIVER_NULLPTR_LAST                                 \
  , link_transport_mdriver_t *link_driver = nullptr
#define FSAPI_LINK_DECLARE_DRIVER link_transport_mdriver_t *link_driver
#define FSAPI_LINK_DECLARE_DRIVER_LAST , link_transport_mdriver_t *link_driver
#define FSAPI_LINK_DEFAULT_PAGE_SIZE 4096
#define FSAPI_LINK_INHERIT_DRIVER link_driver
#define FSAPI_LINK_MEMBER_DRIVER driver()
#define FSAPI_LINK_INHERIT_DRIVER_LAST , link_driver
#define FSAPI_LINK_MEMBER_DRIVER_LAST , driver()
#define FSAPI_LINK_SET_DRIVER(x, y) x.set_driver(y)

#undef fileno
#define MCU_INT_CAST(var) ((void *)(u64)var)
#endif

#endif // FSAPI_FS_MACROS_HPP
