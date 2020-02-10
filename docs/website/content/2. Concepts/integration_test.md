---
title: "Integration Test Framework"
description: "How the integration test framework is build and can be used"
weight: 2
draft: false
toc: true
menu:
  main:
    parent: 2. Concepts
    identifier: integration_test
    weight: 2
---

# Integration Test Framework Developer's Guide

## Introduction

The Integration Test Framework provides the possibility to write and run tests on a HW target. It can orchestrate multiple machines and services which are involved in the tests. The framework can be used for both white-box and black-box testing as well as in the integration and system testing.

## Design Overview

{{<figure src="../../images/kiso_integration_test_context.png" title="Figure 1: Integration Test Framework Context">}}

### Test Coordinator

The **test-coordinator** is the central module setting up and running the tests. Based on a configuration file (in json), it does the following:

* Generate the python test-auxiliaries instances with the right flashing and cchannel.
* Generate the list of tests to perform
* Verify if the tests can be performed
* Flash and run and synchronize the tests on the auxiliaries
* Gather the reports and publish the results

It leverages the native python unittest framework to get some advance behaviors.

### Test Auxiliary

The **test-auxiliary** provides to the **test-coordinator** an interface to interact with the physical or digital auxiliary target.
It is composed by 2 blocks:

* Physical or digital instance creation / deletion (e.g. flash the *device under test* with the testing software. E.g. Start a docker container)
* Communication with this instance (e.g. execute the following command)

In case of the specific *device under test* auxiliary, we have:

* As communication channel (**cchannel**) usually *uart*
* As flashing channel (**flashing**) usually j-tag

For other auxiliaries like the one interacting with cloud services, maybe we just have:

* A communication channel (**channel**) like *Rest*

### Communication Channel

It is also called **cchannel**. It is the medium to communicate with auxiliary target. It can be *uart*, *udp*, *usb*, *Rest*,...
The communication protocol itself can be auxiliary specific. In case of the *device under test*, we have a specific communication protocol. Please see the next paragraph.

### Message Protocol ( If in used )

The message protocol is used (but not only) between the *device under test* HW and its **test-auxiliary**. The communication pattern is as follows:

1. The test manager sends a message that contains a test command to a test participant.
2. The test participant sends an acknowledgement message back. 
3. The test participant may send a report message.
4. The test manager replies to a report message with an acknowledgement message.

The message structure is as follow:

```
0               1               2               3
0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|Ver| MT|  Res  |   Msg Token   |   Sub-Type    | Error code    |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| Test Section  |  Test Suite   |   Test Case   | Payload length|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| Payload (in TLV format)
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

It consist of:

| Code                          | size (in bytes)   | Explanation |
|:------------------------------|:------------------|:------------|
| Ver (Version)                 | 2 bits            | Indicates the version of the test coordination protocol. |
| MT (Message Type)             | 2 bits            | Indicates the type of the message. |
| Res (Reserved)                | 4 bits            |  |
| Msg Token (Message Token)     | 1                 | Arbitrary byte. It must not be repeated for 10 consecutive messages. In the acknowledgement message the same token must be used. |
| Sub-Type (Message Sub Type)   | 1                 | Gives more information about the message type |
| Error Code                    | 1                 | Error code that can be used by the auxiliaries to forward an error |
| Test Section                  | 1                 | Indicates the test section number |
| Test Suite                    | 1                 | Indicates the test suite number which permits to identify a test suite within a test section |
| Test Case                     | 1                 | Indicates the test case number which permits to identify a test case within a test suite |
| Payload Length                | 1                 | Indicate the length of the payload composed of TLV elements. If 0, it means there is no payload |
| Payload                       | X                 | Optional, list of TLVs elements. One TLV has 1 byte for the *Tag*, 1 byte for the *length*, up to 255 bytes for the *Value* |

The **message type** and **message sub-type** are linked and can take the following values:
| Type      | Type Id   | Sub-type              | Sub-type Id   | Explanation |
|:----------|:----------|:----------------------|:--------------|:------------|
| COMMAND   | 0         | PING                  | 0             | For ping-pong between the auxiliary to verify if a communication is established |
|           |           | TEST_SECTION_SETUP    | 1             |  |
|           |           | TEST_SUITE_SETUP      | 2             |  |
|           |           | TEST_CASE_SETUP       | 3             |  |
|           |           | TEST_SECTION_RUN      | 11            |  |
|           |           | TEST_SUITE_RUN        | 12            |  |
|           |           | TEST_CASE_RUN         | 13            |  |
|           |           | TEST_SECTION_TEARDOWN | 21            |  |
|           |           | TEST_SUITE_TEARDOWN   | 22            |  |
|           |           | TEST_CASE_TEARDOWN    | 23            |  |
|           |           | ABORT                 | 99            |  |
| REPORT    | 1         | TEST_PASS             | 0             |  |
|           |           | TEST_FAILED           | 1             |  |
| ACK       | 2         | ACK                   | 0             |  |
|           |           | NACK                  | 1             |  |
| LOG       | 3         |                       |               |  |

The TLV only supported *Tag* are:

* TEST_REPORT = 110
* FAILURE_REASON = 112

### Flashing

The flashing is usually needed to put into the *Device under test* the test-software containing the tests we would like to run.

## Usage

### Flow

1. Create a root-folder that will contain the tests. Let us call it *test-folder*.
2. Create, based on your test-specs, one folder per test-suite.
3. In each test-suite folder, implement the tests. (See how under)
4. Implement the configuration file. (See how also under)
5. If your test-setup is ready, run `python3 integration_test.py -b <PATH_TO_THE_TEST_BINARIES> -c <ROOT_TEST_DIR>`
6. If the tests fail, you will see it in the the output. For more details, you can take a look at the log file (as default *integ_test.log*).

### Implementation of the tests (basic)

**Structure**: *test-folder*/*test-suite-1*/**test_suite_1.py**

**test_suite_1.py**:
```python
"""
I want to run the following tests documented in the following test-specs <TEST_CASE_SPECS>.
"""
import generic_test_case

@generic_test_case.define_test_case_parameters([], 0, 1, 1) # ([], test_section_id, test_suite_id, test_case_id)
class MyTest(generic_test_case.GenericTest.BasicTest):
    pass

@generic_test_case.define_test_case_parameters([], 0, 1, 2)
class MyTest2(generic_test_case.GenericTest.BasicTest):
    pass
```

### Implementation of the tests (advance)

If you need to have more complex tests, you can do the following:

* `BasicTest` is a specific implementation of `unittest.TestCase` therefor it contains 3 steps/methods **setUp()**, **tearDown()** and **test_run()** that can be overwritten.
* `BasicTest` will contain the list of **auxiliaries** you can use. It will be hold in the attribute `test_auxiliary_list`.
* `BasicTest` also contains the following information `test_section_id`, `test_suite_id`, `test_case_id`.
* Import *logging* or/and *message* (if needed) to communicate with the **auxiliary**

**test_suite_2.py**:
```python
"""
I want to run the following tests documented in the following test-specs <TEST_CASE_SPECS>.
"""
import generic_test_case
import message

@generic_test_case.define_test_case_parameters([], 0, 2, 1) # ([], test_section_id, test_suite_id, test_case_id)
class MyTest(generic_test_case.GenericTest.BasicTest):
    def setUp(self):
        # I loop through all the auxiliaries
        for aux in self.test_auxiliary_list:
            if aux.name == "aux1": # If I find the auxiliary to which I need to send a special message, I compose the message and send it.
                # Compose the message to send with some additional informations
                tlv = { TEST_REPORT:"Give me something" }
                testcase_setup_special_message = message.Message(msg_type=message.MessageType.COMMAND, sub_type=message.MessageCommandType.TEST_CASE_SETUP,
                                                        test_section=self.test_section_id, test_suite=self.test_suite_id, test_case=self.test_case_id, tlv_dict=tlv)
                # Send the message
                aux.run_command(testcase_setup_special_message, blocking=True, timeout_in_s=10)
            else: # Do not forget to send a setup message to the other auxiliaries!
                # Compose the normal message
                testcase_setup_basic_message = message.Message(msg_type=message.MessageType.COMMAND, sub_type=message.MessageCommandType.TEST_CASE_SETUP,
                                                        test_section=self.test_section_id, test_suite=self.test_suite_id, test_case=self.test_case_id)
                # Send the message
                aux.run_command(testcase_setup_basic_message, blocking=True, timeout_in_s=10)
```

### Implementation of the tests (advance ++)

Because we are python based, you can until some extend, design and implement parts of the framework to fulfil your needs. For example:

**test_suite_3.py**:
```python
"""
I want to run the following tests documented in the following test-specs <TEST_CASE_SPECS>.
"""
import generic_test_case
import message

class MyTestTemplate(generic_test_case.GenericTest.BasicTest):
    def test_run(self):
        # Prepare message to send
        testcase_run_message = message.Message(msg_type=message.MessageType.COMMAND, sub_type=message.MessageCommandType.TEST_CASE_RUN,
                                                    test_section=self.test_section_id, test_suite=self.test_suite_id, test_case=self.test_case_id)
        # Send test start through all auxiliaries
        for aux in self.test_auxiliary_list:
            if aux.run_command(testcase_run_message, blocking=True, timeout_in_s=10) is not True:
                self.cleanup_and_skip("{} could not be run!".format(aux))
        # Device will reboot, wait for the reboot report
        for aux in self.test_auxiliary_list:
            if aux.name == "DeviceUnderTest":
                report = aux.wait_and_get_report(blocking=True, timeout_in_s=10) # Wait for a report from the DeviceUnderTest
                break
        # Check if the report for the reboot was received.
        report is not None and report.get_message_type() == message.MessageType.REPORT and report.get_message_sub_type() == message.MessageReportType.TEST_PASS:
            pass # We can continue
        else:
            self.cleanup_and_skip("Device failed rebooting")
        # Loop until all reports are received
        list_of_aux_with_received_reports = [False]*len(self.test_auxiliary_list)
        while False in list_of_aux_with_received_reports:
            # Loop through all auxiliaries
            for i, aux in enumerate(self.test_auxiliary_list):
                if list_of_aux_with_received_reports[i] == False:
                    # Wait for a report
                    reported_message = aux.wait_and_get_report()
                    # Check the received message
                    list_of_aux_with_received_reports[i] = self.evaluate_message(aux, reported_message)

@generic_test_case.define_test_case_parameters([], 0, 3, 1)
class MyTest(MyTestTemplate):
    pass

@generic_test_case.define_test_case_parameters([], 0, 3, 2)
class MyTest2(MyTestTemplate):
    pass

```

### Implement the configuration file

The configuration is written in json:

```json
{
    "auxiliary_list":[
        {
            // Auxiliary 1 setting
        },
        // ...
    ],
    "test_suite_list":[
        {
            // Test suite 1 setting
        },
        // ...
    ]
}
```

The test-suite always follows the same pattern:

```json
{
    "test_suite_list":[
        {
            "test_suite_id":1, // What is the id of the test suite?
            "suite_dir":"<PATH_TO_TEST_SUITE", // What is the path to the tests
            "test_filter_pattern":"*.py" // What pattern do you want to apply on the tests. Ex, if you want to test only a sub-set of tests, you can do it
            
        },
        // ...
    ]
}
```

The Auxiliaries configuration is a bit more complex. It depends on which auxiliary you are using.
But do not worry, if you forget a configuration, you will get the information.

```json
{
    "auxiliary_list":[
        {
            "auxiliary_name":"aux2", // Name of the auxiliary. it will be found in the test under `aux.name`.
            
            "auxiliary_type":"example_test_auxiliary", // What kind of auxiliary you use. Check `test-coordinator/test-auxiliaries` for a list. The module name is what you enter here.
            "auxiliary_cfg":{}, // Correspond to the init parameters of the auxiliary module. Please check for the sphinx doxumentation (todo).

            "channel_type":"cc_example", // What is the communication channel that should be used. Check `test-coordinator/cchannel` for a list. The module name is what you enter here.
            "channel_cfg":{}, // Correspond to the init parameters of the cchannel module. Please check for the sphinx doxumentation (todo).

            // In some cases, like the DeviceUnderTest, you will also have the following parameters following the same pattern as above.
            "flashing_type":"flashing_example",
            "flashing_cfg":{}
        },
        // ...
    ]
}
```

### Run the tests

`python3 integration_test.py -b <PATH_TO_THE_TEST_BINARIES> -c <ROOT_TEST_DIR>`
