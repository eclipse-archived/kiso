---
title: "Integration Testing"
weight: 2
---

## Definition

Integration testing is a form of software testing which combines multiple (sub-)modules of a system into one test environment and verifies the compatibility between the selected system subset.

## Realization

In Kiso, integration testing is done **on-target**. This is realized through a special test-app which is flashed onto reference or user hardware and listens to the instructions of a test-coordinator (usually a host PC connected via UART, USB, CAN, or the like). Looking at the hierarchy top-to-bottom (see diagram below), the Kiso integration test framework consists of:

- Test-App - Flashable/Bootable binary for reference and/or user hardware.
  - Test-Executor - Application component of the test-app. Handles basic hardware initialization and connects with test-coordinator.
  - Test-Entry - Linkable static library usually declared on a package-level (such as Essentials, Cellular, Utils, etc.), combining different test-suites into a single entity.
    - Test-Suite - Collection of test-cases that share a common setup and teardown sequence. Can also be used as logical separation of test-cases.
      - Test-Case - Individual test to verify one specific aspect of the (sub-)system. Comprised of setup, run and teardown.
- Test-Coordinator - Python based coordinator application running on a host PC. It communicates with the test-executor running on the device-under-test and decides which test-suites/-cases are to be executed. The python framework is realized as `pykiso` and is available as own executable python module.

![integration test framework](/images/integration_test.svg)

All Kiso packages share a common test-executor, each package also defines one test-entry. Each test-executor/-entry pair compiles into a standalone binary which can be flashed onto hardware. Users can declare their own test-entries and link them with the provided test-executor, that way users can profit form the existing test framework to run integration tests of user written drivers or applications.

Test-Entry, test-suite and test-case contain optional hooks for setup and teardown work.

## Test-App Build System

As with any target binary in Kiso, building is handled by the CMake toolchain. Two special workspace settings need to be provided during workspace configuration to build a integration test app, targeted towards a specific hardware:

- `ENABLE_INTEGRATION_TESTING = ON` - Marks this workspace as targeted towards building a integration test app. Note that with this option set, `KISO_APPLICATION_NAME` will be ignored, meaning no user application will be built.
- `KISO_INTEGRATION_TEST_ENTRY_PATH = "path/to/test-entry"` - Path to directory containing a `CMakeLists.txt` used to build the `testentry` static-library. This library will be linked with the test-executor to create the test-app. The `CMakeLists.txt` must declare a static-library target called `testentry`, containing source-files for any test-suites that shall be part of the final test-app.

The static-library target `testentry` can be created as follows:

```cmake
add_library(testentry STATIC source/test_suite1.c source/test_suite2.c ...)
# List of additional libs needed
target_link_libraries(testentry testing essentials ${KISO_BOARD_LIBS})
```

Please have a look at the existing CMake build files to get a better idea of what to include in your `CMakeLists.txt`. Test-Entry build files are usually located at `core/<package>/test/integration/`
