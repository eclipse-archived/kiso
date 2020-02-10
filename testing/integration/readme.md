# Integration Test Framework #

## Introduction ##
Allows us to run in our CI hil tests on our device farm / or allows a user to run the test on his Pc.

## List of limitations / todos for the python side ##

* **When the auxiliary does not answer (ping or else), GenericTest.BasicTest.cleanup_and_skip() call will result in a lock and break the framework.**
* No test-setion will be executed, needs to be removed later.
* **the flashing feature + cfg parameter pass is missing.**
* Names & configurations in the *cfg file json* are character precise class names & associated parameters.
* **User guide is missing.**
* Spelling mistakes need to be fixed!
* Extension of the parameters for the generic test-case (via list & dict) needed to allow multiple different generic test-case derivatives later on.
* Add verbosity parameters to pass to the unittest framework to get more details about the test.
* **Add result parsing for Jenkins (see: https://stackoverflow.com/questions/11241781/python-unittests-in-jenkins).**
* Create a python package and host it on pip.