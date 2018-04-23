#ifndef CHECKSUM_HPP
#define CHECKSUM_HPP


#include "../api/CalcObject.hpp"


namespace  calc {


/*! \brief Checksum Class
 * \details The Checksum class is purely static and provides methods
 * for calculating and verifying checksums on data structures.
 */
class Checksum : public api::CalcInfoObject {
public:


    /*! \details Calculates a zero sum value for the data.
     *
     * @param data A pointer to the u8 data to sum
     * @param size The number of bytes in the data including the checksum
     * @return A number that when summed with all \a data values results in zero
     *
     *
     * The size value includes the checksum position, but the checksum position
     * is not included in the calculation.
     *
     * If the data is 32-bit aligned, use calc_zero_sum(const u32 *, int);
     *
     *
     * \code
     *
     * #include <mcu/types.h>
     * #include <sapi/calc.hpp>
     *
     * typedef struct MCU_PACK {
     *  u8 member_a;
     *  u32 member_b;
     *  u16 member_c;
     *  u8 checksum;
     * } data_struct_t;
     *
     * data_struct_t data;
     * //set member variables
     * data.checksum = Checksum::calc_zero_sum((u8*)&data, sizeof(data));
     *
     * if( Checksum::verify_zero_sum((u8*)&data, sizeof(data)) == true ){
     *  //checksum is good
     * }
     * \endcode
     *
     *
     *
     */
    static u8 calc_zero_sum(const u8 * data, int size);


    /*! \details Verifies the u8 checksum at the end of a data structure.
     *
     * @param data A pointer to the data
     * @param size The number of bytes in the data
     * @return true if the sum of the data is zero
     *
     *
     */
    static bool verify_zero_sum(const u8 * data, int size);


    /*! \details Calculates a zero sum value for the data.
     *
     * @param data A pointer to the u8 data to sum
     * @param size The number of bytes in the data including the checksum
     * @return A number that when summed with all \a data values results in zero
     *
     *
     * The size value includes the checksum position, but the checksum position
     * is not included in the calculation.
     *
     * If the data is 32-bit aligned, use calc_zero_sum(const u32 *, int);
     *
     *
     * \code
     *
     * #include <mcu/types.h>
     * #include <sapi/calc.hpp>
     *
     * typedef struct MCU_PACK {
     *  u32 member_a;
     *  u32 member_b;
     *  u32 member_c;
     *  u32 checksum;
     * } data_struct_t;
     *
     * data_struct_t data;
     * //set member variables
     * data.checksum = Checksum::calc_zero_sum((u32*)&data, sizeof(data));
     *
     * if( Checksum::verify_zero_sum((u32*)&data, sizeof(data)) == true ){
     *  //checksum is good
     * }
     * \endcode
     *
     *
     */
    static u32 calc_zero_sum(const u32 * data, int size);

    /*! \details Verifies the u32 checksum at the end of a data structure.
     *
     * @param data A pointer to the data
     * @param size The number of bytes in the data
     * @return true if the sum of the data is zero
     *
     * The data must be aligned to 32 bits to work.
     *
     *
     *
     */
    static bool verify_zero_sum(const u32 * data, int size);


};

}

#endif // CHECKSUM_HPP
