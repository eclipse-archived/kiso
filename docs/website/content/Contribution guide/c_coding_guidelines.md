---
title: "C Coding Guidelines"
weight: 3
---

# What are the guidelines I need to follow?

### Table of Contents

* [Introduction](#introduction)
* [Coding Standards](#coding-standards)
* [File Organization](#file-organization)
* [File Structure](#file-structure)
* [Declaration and naming conventions](#conventions)
* [Comments and Documentation](#comments-documentation)
* [References](#references)
* [Revision History](#revison-history)

## Introduction

**Every team member is responsible for the quality of her/his code. The goal is to deliver quality, readable, maintainable and robust code. This document should help you to achieve that goal.** It defines the coding guidelines for software team. The guideline is written considering the Test Driven Development (TDD) principles to make our C code clean, consistent, and easy to read. This document shall also be used as a guidance to writing portable, robust, reliable and extensible programs, so that our well-written program is more than a debugged and tested program, not only a computer can understand, but also humans can understand easily.

Always keep in mind that the primary rule of this coding guideline is:

> **_NOTE:_**  **The code must be easy to read and to maintain.**

## Coding Standards

The software shall follow the [ISO C991](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1124.pdf) standard.

## File Organization

### Source Code Structure

Source code is organized in units . In object-oriented languages a unit is usually a class. C code is mainly placed in source files (*.c) and header files (*.h). In the C language, a unit is formed by a source file along with its header file(s). The source file contains the implementation, while the header files exposes the interface of this unit.

The term module is used here as a synonym of unit.

A software component is a bundle of one or more units or modules, which are logically grouped together.

A software package is a group of one or more components which are versioned, released and distributed together. We also use the term library as a synonym for package .

### Folder Structure

The code of each software package should be organized in a folder structure of this form:

```c
• <root> (the name of the software package)
    • include
    • source
        • protected
        • <Component 1>     // if the package contains multiple components, otherwise source files are located directly under source.
        • <Component 2>
        • and so on...
    • test
        • unit      // Put the artefacts related to the unit testing here.
            • include       // Put the faked public header files of the modules here
            • source        // Put the unit test implementation here
                • protected        // Put the faked protected header files here.
                • <Component 1>     // Put the unit tests of the modules of Component 1 here.
                • <Component 2>     // Put the unit tests of the modules of Component 2 here.
            • Makefiles and build related auxiliary files of the unit tests
        • integration       // put the artefacts related to the integration testing here.
            • spec      // Put the specification in markdown format (*.md) here
            • ...
    • Makefiles and build related auxiliary files
    • Further folders and files such as documentation, examples, etc.
    • Further folders (e.g. debug and release) may be generated during the build
```

### Public Header Files

Public header files are those header files which describe interfaces which should be accessible from outside the package.

Public header files should be located in the folder "**\<root>/include**" .

### Protected Header Files

Protected header files are those header files which may be used across the components within one package, but not exposed to the public (outside the package).

If protected header files exist, then they shall be located in the folder "**\<root>/source/protected**"

### Private Header Files

Private header files are those header files which should only be used internally within a component.

If private header files exist, then they shall be located inside the component's own folder "**\<root>/source/ \<Component>**"

### Source Files

C source files are located in the source folder. If the package contains multiple components, then the component source files are located in the component's sub-folder under the  **"\<root>/source/\<Component>"**.

### Configuration Files

A software package may require some configurations, such as hardware mapping header files and other parameters.

The configuration of the software packages should be provided by the project application in a dedicated configuration folder. Remember that every project may have different configurations than other projects.

### Test Files

The folder "test/unit" contains all unit test artefacts.

All the faked public headers of the package are placed in the folder **"\<root>/test/unit/include>"**.

All the faked protected headers of the package are placed in the folder **"\<root>/test/unit/source/protected>"**.

 The unit test files for each component are placed under **"\<root>/test/unit/source/ \<Component>"** directory.

 For every package, files related to the build of the unit tests are placed in the folder **"\<root>/test/unit"** .

 All faked headers must have the suffix "_th" in their names.

 All unit test source files must have the suffix "_unittest" in their names.

## File Structure

### Header Files

Each header file must have a **guard macro** \<file name>_H to avoid multiple inclusions.

Each source or header file must include exactly those header files which it needs to be compiled.

Each source file must include its own header files first, before any external or system headers. Header files are included in the following order:

1. The public header(s) of the module or component in "..."

2. The protected headers(s) of the module or component in "..."

3. The private header(s) of the module itself in "..."

4. System headers in <...> (You should avoid directly including standard headers. But include Kiso_Basics.h instead)

5. Headers of other components and packages in "..."

#### **Example**

```c
MyFile.c
--------
#include "MyPublic.h"
#include "MyProtected.h"
#include "MyPrivate.h"
#include <stdint.h>
#include <...>
#include "OtherComponent.h"
#include "..."

```

Do not directly include standard c headers. Include instead "Kiso_Basics.h" which is charge of including standard C lib functionality.

## Declaration and naming conventions

### Integral Data Types

Use the standard integer data types with defined size. In C99 these standard data types are defined in the stdint.h system header file.

You should not directly include stdint.h, but include Kiso_Basics.h which will ensure that these types and macros are defined.

**Type**|**Description**|**Type conversion macros**
--------|---------------|--------------------------
int8_t |signed 8-bit integer| INT8_C()
uint8_t|unsigned 8 bit integer| UINT8_C()
int16_t |signed 16-bit integer| INT16_C()
uint16_t |unsigned 16-bit integer| UINT16_C()
int32_t |signed 32-bit integer| INT32_C()
uint32_t |unsigned 32-bit integer| UINT32_C()
int64_t |signed 64-bit integer| INT64_C()
uint64_t |unsigned 64-bit integer| UINT64_C()

If a dedicated data type size is not particularly required, then use the 32-bit types int32_t or uint32_t.

Literals must be stated with the right type conversion macro.

If a signed literal should be to unsigned variable, then a type-cast to the destination data type must be done.

### Floating Point Data Types

Avoid using floating point data types (i.e. float and double) in an embedded environment if possible! It is OK to use float for signal processing.

Even in signal processing, use double only if it is really necessary.

### Meaningful Names

The naming must be done in such a way that they are understandable.

The function name must clearly express what can be achieved using that function.

Avoid unnecessary abbreviations. Standard and commonly understandable abbreviations should be used.

### Length of Names

Keep the name meaningful and as short as possible.

Mind that the compiler may truncate variable and function names at **31 characters**.

### Guard Macros

Each header file must have a guard macro.

The name of the guard macro must be unique all over the project.

The guard macros must have the following format **Kiso_\<file base name>_H_**

### Components

A component is a set of modules which are logically grouped together. If a software package contains more than one component, then each component is located in an own folder under source named with the component name. This applies independent of the number of source and header files of the component.

The component name is a meaningful name for the component using the camel notation with first character written in upper case.

\<Root Folder>/source/GsmDriver

### Files

Each public header file should have the prefix "Kiso_" in its name. This means that the name must have the form **Kiso_\<file base name>.h**

Protected and private header files must not have the prefix "Kiso_" in their names.

The file base name of each public or protected header file should clearly reflect the component or module name, for example:

* Public: **Kiso_GsmDriver.h**

* Protected: **GsmDriver.h**

Both header and source file should be named with a meaningful name written in the **camel notation with first character written in upper case.**

Public header file:

\<Root Folder>/include/Kiso_MyAlgo.h

Protected header file:

\<Root Folder>/source/include/GsmDriverControl.h

Private header file:

\<Root Folder>/source/GsmDriver/Monitor.h

Source file:

\<Root Folder>/source/GsmDriver/Monitor.c

Unit-test source file:

\<Root Folder>/source/GsmDriver/test/unit/source/GsmDriver/Monitor_unittest.cc

Unit-test header file:

\<Root Folder>/test/unit/include/GsmDriverMonitor_th.hh

### Functions

Function name should follow the **camel notation with first character written in upper case**.

Function name should start with a verb or end with a term indicating a function such as "func", "routine" or "callback".

The functions with global scope (i.e. functions which are exposed in header files) should be prefixed by the component name or module name followed by an underscore and the function name.

**\<Component/module name>_\<function name>**

Module private functions (i.e. declared as static) need not to be prefixed with the component/module name. This applies to static inline functions as well.

Static inline functions implemented in header files should follow the naming convention rules of normal, non static functions.

#### **Example for functions**

```c
/* Global function */
void GsmDriver_SendUdpData(void);

/* Static function */
static void ParseResponseData(void);
```

This function naming convention applies also to inline functions and function-like macros.

Use C99 inline functions instead of function like macros whenever possible.

Note that the compiler decides autonomously whether an inline function is really inlined or not.

### Variables

There are three types of variables based on their scope of visibility. Different naming rules apply to each type of variable.

**Global variables:**

 These are variables with a global visibility. Global variables must be avoided whenever possible. If a global variable is necessary, then it should be named as:

**\<Component / module name>_\<Variable Name>_\<Optional Postfix>**

**Module local variables:**

These are variables which are defined in a source file with the keyword static, e.g. static uint32_t Counter;

**Function local variables:**
These are variables which are defined inside a function and have initial lower case letter. They are only visible within this function.

File and function local variables: **\<variable Name>_\<optional Postfix>**

**\<Component / module name>** is the name of the component, the variable is defined in.

**\<Variable Name>** a meaningful name for the variable using **camel notation with first character written in upper case (in case of file scope static) or first character written in lower case (in case of parameter/local).**

**\<Optional Postfix>** may be used for emphasize a certain information about this variable, e.g. the variable is a pointer, e.g. **_ptr**.

#### **Example for variables**

```c
uint16_t fooTest;  /* Local variable */
static void SomeFunction(uint16_t fooBar) {...}  /* Function parameter */
static uint8_t TransmitBuffer[]; /* Static variable (file scope)*/
void * GsmDriver_Lb2cHandle; /* Global variable */
static Socket_T * Socket_ptr; /* Variable name with an optional postfix*/
```

Variables should always be defined in the source file and must never be defined in the header files.

So, it is not recommended to use global variables unless it is absolutely necessary. You need a good reason for this.

If a global variable is absolutely necessary, then it should be exposed in the header file using the extern declaration (keyword "extern").

### Constants and Macros

The constants should follow the **camel notation with first character written in upper case.**

The macros should be written in upper-case characters separated by underscores. The macro name may also contain digits if appropriate, but not at the first character position.

Constants and macros with global visibility must be prefixed by the component or module name, e.g.

**\<Component / module name>_\<Constant Name>**

**\<Component / module name>_\<Macro Name>**

**\<Component / module name>** is the name of the component or module, the macro or constant is defined in.

**\<Constant Name>** a meaningful name for the constant or constant.

**\<Macro Name>** a meaningful name for the macro or constant.

In the const declaration, place the keyword "const" behind the data type.

#### **Example for constant**

```c
/** Debounce time for push button */
#define PUSHBUTTON_DEBOUNCE_DELAY_TIME_MS          (UINT8_C(10))  

/* const value */
static uint32_t const Identifier = ...;

/* const pointer */
static uint32 const * const Identifier = ...;
```

It is highly recommended to use the type conversion macros from <stdint.h> (which are included in Kiso_Basics.h), see </span style="color:blue"> Integral Data Types (see page 18)

Macro values shall be enclosed within parenthesis.

### Structures, Unions and Enumerations

Structures, enumerations and unions should be named as **\<Component name>_\<Name>_\<Suffix>**

**\<Component name>** is the name of the component, the structure / union / enumeration is defined in.

**\<Name>** a meaningful name describing the structure / union / enum, written **using camel notation with first character in upper case.**

**\<Suffix>** is the suffix as follows:

enum names should be suffixed by **_E**

struct names should be suffixed by **_S**

union names should be suffixed by **_U**

The **members in structures or unions** must be named using **camel notation that begin with upper case.**

The **members of an enum** must be named in **upper case separated by underscore.**

#### **Example for structure and enum**

```c
struct GsmDriver_SettingsConfig_S
{
    uint8_t *ApnAddress;
    uint8_t LengthOfApnAddress;
};

enum GsmDriver_State_E
{
    GSM_STATE_INIT_DONE,
    GSM_STATE_READY,
    GSM_STATE_BUSY,
    GSM_STATE_EXCEPTION,
    GSM_STATE_SHUTDOWN,
    GSM_STATE_MAX
};
```

### Typedefs

Do not combine the type-definition while defining the data type (struct / enum / union / function pointer), but separate the typedef from the definition of the structure / enum / union / function pointer.

typedef type names should be suffixed with the **_T** tag

#### **Example for typedef**

```c
typedef struct GsmDriver_SettingsConfig_S GsmDriver_SettingsConfig_T;
typedef struct GsmDriver_SettingsConfig_S const *GsmDriver_SettingsConfigPtr_T;
typedef enum GsmDriver_State_E GsmDriver_State_T;
typedef union ProductInfo_Version_U ProductInfo_Version_T;
typedef Retcode_T (*GsmDriver_SomeFunction_T)(...);
```

## Comments and Documentation

There are four acceptable comment styles:

* The single line comment style //...(use only for debugging and other technical needs, not for code documentation)

* The classical multiple line comment style is also available /* ... */

* The third comment type is the doxygen comment which shall follow the JavaDoc style /** Doxygen comment */ .

* For in-line doxygen comments with the code, there is a doxy comment as follows, /**< In line comment */

Comments which should be part of the doxygen documentation should use the JavaDoc style.

Common ordinary comments in the code which should not be exported to the Doxygen documentation should use the classical C-style comments /* ... */.

The C++ single line style should be used only in special circumstances for debugging purposes. If a developer is required to disable a code snippet for some workarounds or testing purposes than the C++ style could be used. This way if a developer forgets to re-enable the previously disabled code by mistake it will be visible for every reviewers and such common mistakes can be identified easily.

Please refer to the manual of Doxygen [here](http://www.doxygen.nl/manual/).

Please note that doxygen comments should be primarily placed in the header files, not in the source files.

### Package header

```c
/********************************************************************************
* Copyright (c) 2010-2019 Robert Bosch GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* http://www.eclipse.org/legal/epl-2.0.
*
* SPDX-License-Identifier: EPL-2.0
*
* Contributors:
*    Robert Bosch GmbH - initial contribution
*
********************************************************************************/

/**
 * @defgroup    PACKAGE_NAME package_name
 * @{
 *
 * @brief
 *      What does it do?
 *
 * @details
 *      If needed?
 *
 * @par Overview
 *      Overview of the component + architecture
 * @par Usage
 *      Explain here how to use it
 * @par Extension
 *      Explain here how to extend your component. \note, it is different from how to extend your module!
 *
 * @file
 */
...
/**@} */
```

### Module header

```c
/********************************************************************************
* Copyright (c) 2010-2019 Robert Bosch GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* http://www.eclipse.org/legal/epl-2.0.
*
* SPDX-License-Identifier: EPL-2.0
*
* Contributors:
*    Robert Bosch GmbH - initial contribution
*
********************************************************************************/

/**
 * @ingroup PACKAGE_NAME
 *
 * @defgroup COMPONENT_NAME component_name
 * @{
 *
 * @brief
 *      What does it do?
 *
 * @details
 *      If needed?
 *
 * @par Overview
 *      Overview of the module + architecture
 * @par Usage
 *      Explain here how to use it
 * @par Extension
 *      Explain here how to extend your module.
 *
 * @code
 *      Code example
 * @endcode
 *
 * @file
 **/
...
/**@} */
```

### Structure documentation

```c
/**
 * @brief
 *      What it is
 * @warning
 *      Warnings?, @note can also be used
 */
typedef struct structure_S
{
    element1 element; /**< what is this element */
} structure_T;
```

### Function documentation

```c
/**
 * @brief
 *      What does it do?
 * @details
 *      Add some more details
 * @pre
 *      Add prerequisites to use this function.
 * @param [in] param_name
 *      What is it used for.
 * @param [out] param_name2
 *      What is it used for.
 * @return
 *      RETCODE_OK - Success \n
 *      RETCODE_OUT_OF_RESOURCES - FreeRTOS heap does not have enough space to create the resource \n
 *      ...
 *
 * @note
 *      Need one?
 *
 * @code
 *      Code example
 * @endcode
 *
 * @see function2
 *
 */
Retcode_T function(param_type param_name, param_type2 param_name2);
```

### File comments

Every source file and header file must have the following header format that contains the copyright, license disclaimer, file name and a brief description of the purpose of the module. It is sufficient to include only the tag @file. Doxygen will extract the name of the file.

```c
 /********************************************************************************
* Copyright (c) 2010-2019 Robert Bosch GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* http://www.eclipse.org/legal/epl-2.0.
*
* SPDX-License-Identifier: EPL-2.0
*
* Contributors:
*    Robert Bosch GmbH - initial contribution
*
********************************************************************************/

/**
 *
 * @brief
 *      Add a brief description here.
 *
 * @details
 *      Put here the documentation of this header file. Explain the interface exposed
 *      by this header, e.g. what is the purpose of use, how to use it, etc.
 *
 * @file
 **/

```

### Group comments

Group comments is for doxygen to document relative definitions as a group which may consist of certain definitions including macros, enumerations, structures, functions and so on.

A group is defined by **"@defgroup"**. Additionally, tag **"@ingroup"** defines the group name under which group exists. The comments of group starts with the tag **"@brief"**, followed by tag **"@details"** which mostly contains the lists of functions that are used in the header. The group is enclosed within " /**@{ */ and /**@}*/ " .

```c
/**
 * @ingroup ${group name in which file_base comes under}
 *
 * @defgroup ${file_base}
 * @{
 *
 * @brief
 *      Add a brief description here.
 *
 * @details
 *      Put here the documentation of this header file. Explain the interface exposed
 *      by this header, e.g. what is the purpose of use, how to use it, etc.
 */

/**@}*/
```

### Other comments

Doxygen provides special commands to document the notes and warnings also. They are as follows:

```c
/**
 * @note
 *      Important notes about the function's usage(like Mutex are created such that the
 *      first call to OS_semaphoreTake() on the mutex will pass.)
 * @warning
 *      Warning to the user that function should not be used in certain scenario(like Mutex
 *      type semaphores cannot be used in Interrupt Service Routines(ISR))
 * @pre
 *      Used to specify calling context rules or preconditions for an API or interface. For
 *      example many APIs do not support reentrancy, so these APIs cannot be called from
 *      interrupt context.
 */
```

#### Macro

For macros, the comment can be provided in-line to the definition or also as a normal doxygen comment.

#### Structure, Enumeration and Union**

For structures / enumeration / union, a brief description about the structure must be provided in doxygen comment. Doxygen command _**@brief**_ must be used. If needed, documentation for the members can be provided using inline comment style.

## References

1. [Doxygen user manual](http://www.doxygen.nl/manual/)
2. [PlantUML](http://plantuml.sourceforge.net)

## Revision History

**Issue**|**Date**|**Editor**|**Description of amendment**
--------|---------------|---------- | ----------
0.1 | 01.08.2019 |Kiso maintainer team| Initial version
