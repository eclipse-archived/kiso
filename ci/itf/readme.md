# CI/CD Configuration - Integration Test Framework (ITF)

This directory contains a special `Jenkinsfile` designed for Jenkins Agents that act as Kiso's Integration Test hosts. These Agents must have physical access to test-hardware on which to flash and execute the integration tests. Additionally, Jenkins Master must be configured to source the job's `Jenkinsfile` from this directory. This can be done in the configuration view of any GitHub or Pipeline job.

Agents participating in the ITF setup must be marked by labels.

* General labels:
  * `kiso-integration-test` - general ITF agent, applied to all agents participating in ITF tasks
* Hardware labels:
  * `nucleo-f767zi` - STM32 NUCLEO-F767ZI board
