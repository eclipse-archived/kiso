## Test Section 1: CommonGateway Manual Tests

## Test Suite 1.1: Feature-set for Release v0.1.0  (BCX release)

### Description
To verify the functionalities, our test-strategy contains the following points:
* All BCX features shall be tested
* The tests will be written in Mita

Set of features to test:
* Cellular: On / Off / Activate data-context / HTTP/POST
* Led: On / Off / Change colors
* BMA280 (acc): On / Off / Correctness of the collected samples
* BME280 (pressure, humidity, temperature): On / Off / Correctness of the collected samples

In the description of every test-cases, **bold** elements are tested, the rest no

## Test Cases

### TC 1.1.1: Test data-collection & send data to the cloud

#### Description
The following functionalities will be verified in this test-case:
* **Cellular**: **On / Off / Activate data-context / HTTP/POST**
* **Led**: **On / Off** / Change colors
* **BMA280** (acc): **On / Off** / Correctness of the collected samples
* **BME280** (pressure, humidity, temperature): **On / Off / Correctness of the collected samples**

#### Setup
1. Initialize the sensors
2. Initialize the Cellular module

#### Run
1. Start sampling data every 500ms
2. Enable Cellular and send sampled data to the Backend

Do it x times.

#### TearDown
1. Data are received on the Backend side and are completed
2. We verify via an external source the following values: pressure, humidity, temperature

----------

### TC 1.1.2: Test correctness accelerometer

#### Description
The following functionalities will be verified in this test-case:
* **Cellular**: **On / Off / Activate data-context / HTTP/POST**
* **Led**: On / Off / Change colors
* **BMA280** (acc): **On / Off / Correctness of the collected samples**
* BME280 (pressure, humidity, temperature): On / Off / Correctness of the collected samples

#### Setup
1. Initialize the bma280 sensor
2. Initialize the Cellular module

#### Run
1. Start sampling data every 500ms
2. Enable Cellular and send sampled data to the Backend

Do x times:
3. Push device in an orthogonal plan and verify on the Backend that the axes are correct

#### TearDown
None

----------