/*****************************************************************************/
/** @file magic.h
 *  @brief gtest extension module's interface header.
 *
 *****************************************************************************/

/*****************************************************************************/
/** @page page_magic_id Header information of gtest extension module
 *******************************************************************************
 @par Interface Header
 @verbatim
 The gtest extension module's interface header exports the following features:
 - Magic class
 - Magic::GetAddress()
 @endverbatim
 **************************************************************************** */

/*****************************************************************************/
/** @defgroup page_magic_id_ih gtest extension module's interface header
 * Interface section of the gtest extension module.
 *
 * @ingroup module_magic_id
 * @{
 *************************************************************************** */

/* ****************************************************************************
 * Copyright
 *
 * Robert Bosch Kft.
 * Gyomroi street 120.
 * H-1103 Budapest
 *
 * Copyright (c) 2013           All rights reserved.
 * This software is copyright protected and proprietary to Robert Bosch Kft.
 *
 *****************************************************************************/

/* ****************************************************************************
 * Creation date: 2013-07-07
 *
 * Authors:
 * User        Name                  Department
 *  klt2bp      Tamas Kleiber         AE/EMS-Bp
 *
 * Description: Interface header for the gtest extension module.
 *
 *****************************************************************************/

/* header definition ******************************************************* */
#ifndef MAGIC_H_
#define MAGIC_H_

/* public interface declaration ******************************************** */

/** gtest extension - class declaration to look up run-time symbol addresses. */
class Magic
{
public:
    Magic();
    virtual ~Magic();

    /** The method returns NULL or the address as void* pointer to the requested symbol name.
     *
     * The symbol name is a concatenated string built from the function name and
     * function static variable name separated by the "::" tag. Like <function>::<variable>.
     *
     * For example in case of the following function,
     * @code
     * void TST_function(void)
     * {
     *     static uint32_t testLocalStaticVariable = TST_INIT_VALUE;
     *
     *     ...
     * }
     * @endcode
     * the symbol input parameter to the GetAddress method should
     * be "TST_function::testLocalStaticVariable".
     *
     * @param symbol Special string representation of the requested symbol name.
     *
     * @retval void none
     */
    void* GetAddress(const char* symbol);

private:
    unsigned long GetFileSizeEx(void* hFile);
    void InitDataFile(void);
    void ReadData(void);
    bool m_isInited;
    unsigned char* m_dataBuffer;
};

/* public function prototype declarations */

/** Helper macro - direct use is forbidden! */
#define MAGIC_CONCAT_(a, b, c) a##b##c

/** Helper macro - direct use is forbidden! */
#define MAGIC_CONCAT(a, b, c) MAGIC_CONCAT_(a, b, c)

/** The macro provides an interface to fetch a statically allocated symbol's
 * run-time relocated physical address and assign it to a pointer variable.
 *
 * For example the following code snippet looks up the address of the function
 * static variable called URCstate which is defined in function scope within the
 * LG100_frameProc() API and creates a pointer to the variable, with pointer type
 * LG100_rxFrameState_t*, called MGC_URCstate_md.
 *
 * @code MAGIC_INIT_ACCESS_PROVIDER(LG100_rxFrameState_t, URCstate, "LG100_frameProc::URCstate"); @endcode
 *
 * To be able to use the macro, var shall be defined via @a MAGIC_DEFINE_ACCESS_PROVIDER()
 * first.
 *
 * The macro can only be called from function body context as it executes code.
 *
 * @param type Original type of the symbol which is looked up by address.
 * @param var A file scope unique variable name which will hold the pointer to the looked up variable.
 *        The variable name var will be auto pre- and post fixed by MGC_##var##_md tags.
 *        See also @a MAGIC_GET_IDENTIFIER.
 * @param symbol C string representing a symbol name to be looked up prefixed by its context. See also @a Magic::GetAddress().
 *
 * @retval void none
 */
#define MAGIC_INIT_ACCESS_PROVIDER(type, var, symbol) \
    { \
        extern Magic* MGC_magicObject_gdu; \
        if(((type*)NULL == MAGIC_CONCAT(MGC_,var,_mp)) && ((Magic*)NULL != MGC_magicObject_gdu)) \
        { \
            extern Magic* MGC_magicObject_gdu; \
            type* MAGIC_CONCAT(MGC_,var,_lp) = (type*)((MGC_magicObject_gdu)->GetAddress(symbol)); \
            assert((type*)NULL != MAGIC_CONCAT(MGC_,var,_lp)); \
            MAGIC_CONCAT(MGC_,var,_mp) = MAGIC_CONCAT(MGC_,var,_lp); \
        } \
    }

/** The macro provides an interface to assign a new value to a variable previously
 * defined via @a MAGIC_DEFINE_ACCESS_PROVIDER(). The macro does not check access
 * right restrictions by const or such type qualifiers so the user must be very
 * careful to avoid write access requests to read only memory areas.
 *
 * To be able to use the macro, var shall be defined via @a MAGIC_DEFINE_ACCESS_PROVIDER()
 * first.
 *
 * Example use case:
 * @code
 * //definitions section of the unit test source file
 * MAGIC_DEFINE_ACCESS_PROVIDER(char*, staticLocalBufferVariable);
 *
 * ...
 *
 * // unit test fixture's implementation body
 * char fakeLocalBuffer[100];
 * MAGIC_SET_IDENTIFIER(staticLocalBufferVariable, fakeLocalBuffer);
 * // the above macro redirects staticLocalBufferVariable to fakeLocalBuffer
 * @endcode
 *
 * The macro can only be called from function body context as it executes code.
 *
 * @param type Original type of the symbol which is looked up by address.
 * @param var A file scope unique variable name previously defined via @a MAGIC_DEFINE_ACCESS_PROVIDER().
 * @param value New value which needs to be assigned to the function scope static variable.
 *
 * @retval void none
 */
#define MAGIC_SET_IDENTIFIER(var, value) \
    *MAGIC_CONCAT(MGC_,var,_mp) = (value)

/** The macro provides an interface to fetch the value of a variable previously
 * defined via @a MAGIC_DEFINE_ACCESS_PROVIDER(). The macro does not check
 * whether the variable had been initialized via @a MAGIC_INIT_ACCESS_PROVIDER()
 * in which case a NULL pointer will be dereferenced resulting in an immediate crash.
 * The user shall take special caution to avoid such issues.
 *
 * The macro can only be called from function body context as it executes code.
 *
 * @param var A file scope unique variable name or a file static function pointer
 *        previously defined via @a MAGIC_DEFINE_ACCESS_PROVIDER().
 *
 * @retval void none
 */
#define MAGIC_GET_IDENTIFIER(var) \
    *MAGIC_CONCAT(MGC_,var,_mp)

/** The macro provides an interface to create file scope unique variables within
 * an FFF_DEFINITION_BLOCK which could be used later on together with the
 * @a MAGIC_INIT_ACCESS_PROVIDER(), @a MAGIC_GET_IDENTIFIER(), @a MAGIC_SET_IDENTIFIER()
 * macros.
 *
 *
 * definition fetch the value of a variable previously
 * defined via @a MAGIC_DEFINE_ACCESS_PROVIDER(). The macro does not check
 * whether the variable had been initialized via @a MAGIC_INIT_ACCESS_PROVIDER()
 * in which case a NULL pointer will be dereferenced resulting in an immediate crash.
 * The user shall take special caution to avoid such issues.
 *
 * For an example use case see @a MAGIC_SET_IDENTIFIER().
 *
 * @param type Original type of the symbol which is looked up by address.
 * @param var A file scope unique variable name or a file static function pointer.
 *
 * @retval void none
 */
#define MAGIC_DEFINE_ACCESS_PROVIDER(type, var) \
    type* MAGIC_CONCAT(MGC_,var,_mp) = (type*)NULL;

/* public global variable declarations */

#endif /* MAGIC_H_ */

/** @} ********************************************************************* */
