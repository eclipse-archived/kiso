# Integration Test Framework

The ITF is composed of the following packages:

- Test-auxiliaries: List of elements that support more complex tests (example: utilities to control a bench power supply or a database server the device-under-test can access during the test).
- Test-coordinator: Coordinate the build and execution of tests.
- Test-executor: Software that will be flashed on the device under test.
- Test-scheduler: Called in continuous integration. Defines where the tests should be run.

## CI/CD Configuration

This directory contains a special `Jenkinsfile` designed for Jenkins agents that act as Kiso's Integration Test hosts. These Agents must have physical access to test-hardware on which to flash and execute the integration tests. Additionally, Jenkins master must be configured to source the job's `Jenkinsfile` from this directory. This can be done in the configuration view of any GitHub or Pipeline job.

Agents participating in the ITF setup must be marked by labels. Check out the [integration testing guide](http://docs.eclipsekiso.de:1313/user-guide/integration_testing_setup_guide.html) for more details.
