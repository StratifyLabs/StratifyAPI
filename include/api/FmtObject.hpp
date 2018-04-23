#ifndef FMT_FMT_OBJECT_HPP
#define FMT_FMT_OBJECT_HPP

#include "WorkObject.hpp"
#include "InfoObject.hpp"
#include "../sys/File.hpp"

namespace api {

/*! \brief Format Infomation Object.
 *
 */
class FmtInfoObject : public virtual InfoObject {

};

/*! \brief Format Work Object Class
 *
 */
class FmtWorkObject : public virtual WorkObject {

};

/*! \brief Format File Class
 *
 * \details This class is for format object that inherit sys::File.
 *
 */
class FmtFileObject : public sys::File {

};

}

#endif // FMT_FMT_OBJECT_HPP
