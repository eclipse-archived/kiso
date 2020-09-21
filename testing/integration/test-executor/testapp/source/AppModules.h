
#ifndef APPMODULES_H_
#define APPMODULES_H_

/**
 * @brief Enumerates application modules which are reporting error codes according to RETCODE specification.
 * @info  usage:
 *      #undef KISO_APP_MODULE_ID
 *      #define KISO_APP_MODULE_ID APP_MODULE_ID_xxx
 */
enum App_ModuleID_E
{
    APP_MODULE_ID_MAIN = 1,
    /* Define next module ID here and assign a value to it! */
};

#endif
