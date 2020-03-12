# Integration Test Framework

It is composed of the following packages:

* Test-scheduler: Called in continuous integration. Defines where the tests should be run.
* Test-coordinator: Coordinate the build and execution of tests.
* Test-auxiliaries: List of elements that support more complex tests (example: communication tests between a cloud service and a device)
* Test-executor: Software that will be flashed on the device under test.
