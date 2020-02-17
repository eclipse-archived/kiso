---
title: "Unit Testing Guide"
weight: 5
---

# How should I write my unit tests?

## Where to put mockup headers and unit tests
Along each package we have a `test` directory for it. Its structure resembles a package by itself with include and source directories.
Let's look at the `utils` package for example:

- utils
    - include
        - \<module name>.h
    - source
        - \<module name>.c
    - **test**
        - **unit**
            - **include**
                - **\<module name>_th.hh** // <- mock-up headers go here
            - **source**
                - **\<module name>_unittest.cc** // <- unit test code goes here
        - **CMakeLists.txt**
    - CMakeLists.txt

For each module in a package we have one header containing the mock-ups that other modules use during testing and one source file with all the tests for that module.
[The naming pattern]({{< relref "c_coding_guidelines#test-files" >}}) of the files is mandatory as given in the above example - *<module name>* followed by either *_th.hh* for test headers or *_unittest.cc* for test code.

## Frameworks
The unit tests are written in C++ using the [Googletest Framework](https://github.com/google/googletest) and [Fake Function Framework (FFF)](https://github.com/meekrosoft/fff) for the mock-ups.
We have copies of the frameworks in the `thirdparty` directory as there are some minor modifications to them. We write our own fakes for the thirdparty libraries where necessary and put them in the library's test directory.
Sometimes (e.g. in FreeRTOS) we have to modify the library's headers in order to compile them for unit testing as sometimes code is written directly in the library header.

## Mock-ups
Using FFF allows us to quickly and easily provide mock-ups for modules. The main usage involves just two macros - FAKE_VOID_FUNC and FAKE_VALUE_FUNC.
We need to include the FFF header for the macro definitions, all required external faked headers and lastly the public module header.

Kiso_CmdProcessor_th.hh:
```c
#include "fff.h"

#include "Kiso_Retcode_th.hh" // External module dependencies
#include "Kiso_CmdProcessor.h" // Include the real module header

FAKE_VOID_FUNC(CmdProcessor_Suspend, CmdProcessor_T *)
FAKE_VOID_FUNC(CmdProcessor_Resume, CmdProcessor_T *)
FAKE_VALUE_FUNC(Retcode_T, CmdProcessor_Initialize, CmdProcessor_T *, const char *, uint32_t, uint32_t, uint32_t)
FAKE_VALUE_FUNC(Retcode_T, CmdProcessor_Enqueue, CmdProcessor_T *, CmdProcessor_Func_T, void *, uint32_t)
```

These four macros will create mockups of the functions as follows:
```c
void CmdProcessor_Suspend(CmdProcessor_T *);
void CmdProcessor_Resume(CmdProcessor_T *);
Retcode_T CmdProcessor_Initialize(CmdProcessor_T *, const char *, uint32_t, uint32_t, uint32_t);
Retcode_T CmdProcessor_Enqueue(CmdProcessor_T *, CmdProcessor_Func_T, void *, uint32_t);
```
The main difference being whether the function is void or not. More information on specifics with FFF declarations can be found in the FFF project page.
When writing the faked header, we need to make sure all public functions have their own fake.

## Test code with examples

Taking a look at the setup test code for CmdProcessor:

```c
#include <gtest.h>
extern "C"
{
#include "Kiso_Utils.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_UTILS_MODULE_ID_CMDPROCESSOR

#if KISO_FEATURE_CMDPROCESSOR

/* Include faked interfaces */
#include "Kiso_Retcode_th.hh"
#include "Kiso_Assert_th.hh"

#include "task_th.hh"
#include "queue_th.hh"

/* Include module under test */
#include "CmdProcessor.c"

/* Configuration parameters for the module if not provided by the module header itself or external configuration. */
#define TASK_PRIORITY UINT32_C(1)   /**< Task Priority should be less then timer task priority */
#define STACK_SIZE    UINT32_C(256) /**< stack size of the task */
#define QUEUE_SIZE    UINT32_C(3)   /**< size of the queue. Holds command processor command structure variables */
#define QUEUE_CREATED UINT32_C(1)   /**< Used as a return value for successful queue creation */

BaseType_t xTaskCreate_fake_success(TaskFunction_t, const char *const,
                                    const configSTACK_DEPTH_TYPE,
                                    void *const, UBaseType_t,
                                    TaskHandle_t *const pxCreatedTask)
{
    *pxCreatedTask = (TaskHandle_t)0x123;
    return pdTRUE;
}

class CmdProcessor : public testing::Test
{
protected:
    virtual void SetUp()
    {
        RESET_FAKE(xTaskCreate);
        RESET_FAKE(xQueueCreate);
        RESET_FAKE(Retcode_RaiseError);

        xTaskCreate_fake.custom_fake = xTaskCreate_fake_success;
        xQueueCreate_fake.return_val = (QueueHandle_t)QUEUE_CREATED;

        Retcode_T retVal = CmdProcessor_Initialize(&cmdProcessor, "abc", TASK_PRIORITY, STACK_SIZE, QUEUE_SIZE);
        EXPECT_EQ(RETCODE_OK, retVal);

        RESET_FAKE(xTaskCreate);
        RESET_FAKE(xQueueCreate);

        FFF_RESET_HISTORY();
    }

    CmdProcessor_T cmdProcessor = {0};
};
}

#endif
```
In this example we first include the gtest library header, the faked headers we will need in the implementation of the actual CmdProcessor code and the implementation code itself.
We can't compile the source independently because we mock the real implementation of depencencies.

All modules under test are guarded by the feature enablement macros - in this case `KISO_FEATURE_CMDPROCESSOR`. Not all boards can support all features or some are not yet implemented for a specific
processor, so we need to make sure we aren't trying to test something that's not supported.

After that we use the Gtest framework to define a test class `CmdProcessor` which is derived from `testing::Test`. Later we will use this class as a stub for the specific tests.
The main points here are that we have a set of members that each test will use and `SetUp()` and `TearDown()` methods which are executed respectively **before** and **after** each test.
Since we don't need cleanup in this case, the TearDown implementation is omitted.

The CmdProcessor implementation requires an initialized CmdProcessor instance for each function call, so we can put the fake initialization code in the `SetUp()` method.

The `CmdProcessor_Initialize()` implementation:
```c
Retcode_T CmdProcessor_Initialize(
    CmdProcessor_T *cmdProcessor, const char *name,
    uint32_t taskPriority, uint32_t taskStackDepth, uint32_t queueSize)
{
    if ((NULL != cmdProcessor) && (NULL != name))
    {
        cmdProcessor->queue = xQueueCreate(...);
        if (NULL == cmdProcessor->queue)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }
        else
        {
            if (pdPASS != xTaskCreate(..., &cmdProcessor->task))
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            }
            else
            {
                retcode = RETCODE_OK;
            }
        }
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    return retcode;
}
```

We want to simulate in the setup code a successful initialization, so we instruct the faked external dependencies (`xQueueCreate()` and `xTaskCreate()`) how to behave and what to return.
`xQueueCreate_fake` is a structure, provided by FFF via the declaration of the mock with the `FAKE_VALUE_FUNC()` macro. In the simple case we just set the `return_value` member of that structure
to indicate what we want it to return when a call to `xQueueCreate()` is made. We can see from the implementation that it has to be `!= NULL` for the straight case. While testing `CmdProcessor_Initialize()`
we need to make sure to cover as much of the branches as possible. In general, we will have a separate test case for each branch we test.
The tests would mostly just have a different mock setup and expected result to match. In this case we need to write test cases for the following possible branch paths:

branch | cmdProcessor |  name  | xQueueCreate | xTaskCreate  | retcode
-------|--------------|--------|--------------|--------------|----------------------
   1   |     NULL     |    -   | *not called* | *not called* | RETCODE_INVALID_PARAM
   2   |   NOT-NULL   |  NULL  | *not called* | *not called* | RETCODE_INVALID_PARAM
   3   |   NOT-NULL   |NOT-NULL|     NULL     | *not called* | RETCODE_FAILURE
   4   |   NOT-NULL   |NOT-NULL|   NOT-NULL   |  NOT pdPASS  | RETCODE_FAILURE
   5   |   NOT-NULL   |NOT-NULL|   NOT-NULL   |    pdPASS    | RETCODE_OK

The `xTaskCreate()` is a little different in that it has a side effect - it has an output parameter as its last argument, that needs to be set. In this case we use the FFF-provided member
`custom_fake` to set a custom function that will be executed in place of `xTaskCreate()`.

The fakes that FFF provides have internal call counters and a history of the calls made to them with their arguments so that we can check exactly how they were called. `RESET_FAKE(xQueueCreate)` resets the call count
of `xQueueCreate_fake` and `FFF_RESET_HISTORY()` resets the global call history so that we have a consistent state in the beginning of each test.
Later we can test the expected behavior in a very simple manner:

```c
void fake_fn(void *, uint32_t) { ; }
TEST_F(CmdProcessor, CmdProcessorEnqueueQueueFull)
{
    Retcode_T retVal = RETCODE_OK;
    xQueueSend_fake.return_val = errQUEUE_FULL;
    retVal = CmdProcessor_Enqueue(&cmdProcessor, fake_fn, NULL, 1);

    EXPECT_EQ(UINT32_C(1), xQueueSend_fake.call_count);
    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retVal));
}

TEST_F(CmdProcessor, CmdProcessorEnqueueSuccess)
{
    Retcode_T retVal = RETCODE_OK;
    xQueueSend_fake.return_val = pdPASS;

    retVal = CmdProcessor_Enqueue(&cmdProcessor, fake_fn, NULL, 1);

    EXPECT_EQ(UINT32_C(1), xQueueSend_fake.call_count);
    EXPECT_EQ(RETCODE_OK, Retcode_GetCode(retVal));
}
```
We define a couple of test cases for this test class (CmdProcessor) and name them according to what exactly they test. Each test-case is realized as a sub-class of the test-setup class.
We are therfore able to access the protected members of the test-setup - the initialized `cmdProcessor` instance. We are, of course, able to preconfigure the fakes we will need - in the first case a full queue.
Then we expect that the call to `CmdProcessor_Enqueue()` will make one internal call to `xQueueSend()`. We instructed the fake prior to the call to report a full queue, in which case
the `CmdProcessor_Enqueue()` shall fail with `RETCODE_FAILURE`. In the second case we also expect one call to `xQueueSend()`, but this time it's set to succeed and we expect a `RETCODE_OK`.

The test code is expected to be as simple as possible and only verify the behaviour of the module under a known and pre-set state.
We try to verify as much of the branches and lines as possible, with a minimum аccording to the [DOD]({{< relref "dod" >}}), points 6 and 8e.

## Compiling and running unit tests
The unit tests are built and run on the host machine, without the need for a running target. You would need GCC (and G++) installed for your host architecture and optionally gcov/lcov for coverage reports.
You can see how to configure a build tree for unit testing with CMake in the [Using CMake guide]({{< relref "using_cmake#building-unit-tests" >}}).

CMake creates a target for each unit test file found with a prefix of it's package. In this example we created the `CmdProcessor_unittest.cc` file which translates to target `utils_CmdProcessor_unittest` when CMake is done configuring. The target could be built by itself, as a part of the `utils_tests` target, or the coverage targets (if coverage is enabled) - `utils_cov` and `coverage`. If a coverage target is built, the tests in the unit test file are executed automatically and coverage reports are available in HTML format. You can open `<build_root>/<package>_cov/index.html` in your browser to see the coverage report for the utils, essentials and cellular packages.

You can also manually execute the compiled test binary (without making a new coverage report), which in this example is located at `<build_root>/utils/test/utils_CmdProcessor_unittest`, or execute all compiled test modules by calling `ctest` in the build root or `cmake --build <build_root> --target test`.

