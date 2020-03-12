# Integration Test Framework

It is composed by the following packages:
* Test-scheduler: Called in the continious integration. Define where the tests should be run.
* Test-coordinator: Coordiante the build and execution of the tests.
* Test-auxiliaries: List of elements that supports more complex tests (example: communication tests between a cloud service and a device)
* Test-executor: Software that will be flashed on the device under test.