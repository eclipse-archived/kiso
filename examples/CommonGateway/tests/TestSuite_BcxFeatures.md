## Test Section 1: CommonGateway Manual Tests

## Test Suite 1.1: Feature-set for Release v0.1.0  (BCX release)

### Description
To verify the functionalities, our test-strategy contains the following points:
* All BCX features shall be tested
* The tests will be written in Mita
* One feature will be minimum covered by one test-case

Set of features to test:
* **Cellular: On** / Off / **CAT-M1 / NB-IoT / Activate data-context / HTTP/POST**
* **Led: On** / Off / **Change colors**
* **BMA280 (acc): On** / Off / **Correctness of the collected samples**
* **BME280 (pressure, humidity, temperature): On** / Off / **Correctness of the collected samples**
* **RTT: printf**
* **UART: printf**

In the description of every test-cases, **bold** elements are tested, the rest no.
The "off" functionality cannot be tested via Mita, therefore, we cannot test it.

## Test Cases

### TC 1.1.1: Test data-collection

#### Description
The following functionalities will be verified in this test-case:
* Cellular: On / Off / CAT-M1 / NB-IoT / Activate data-context / HTTP/POST
* **Led**: **On** / Off / **Change colors**
* **BMA280** (acc): **On** / Off / Correctness of the collected samples
* **BME280** (pressure, humidity, temperature): **On** / Off / **Correctness of the collected samples**
* **RTT: printf**
* UART: printf

#### Setup
1. Initialize the sensors
2. Initialize RTT

#### Run
1. Start sampling data every 10 ms
2. Print output out via RTT
3. Every 500 ms, we change led colors

Do it x times.

#### TearDown
1. We verify data are sent out via RTT
2. We verify via an external source the following values: pressure, humidity, temperature
3. We verify the leds change colors

----------

### TC 1.1.2: Test correctness accelerometer

#### Description
The following functionalities will be verified in this test-case:
* Cellular: On / Off / CAT-M1 / NB-IoT / Activate data-context / HTTP/POST
* Led: On / Off / Change colors
* **BMA280** (acc): **On** / Off / **Correctness of the collected samples**
* BME280 (pressure, humidity, temperature): On / Off / Correctness of the collected samples
* **RTT: printf**
* UART: printf

#### Setup
1. Initialize the bma280 sensor
2. Initialize RTT

#### Run
1. Start sampling data every 10ms
2. Print output out via RTT

Do x times:
3. Push device in an orthogonal plan and verify on the RTT viewer that the axes are correct

#### TearDown
None

----------

### TC 1.1.3: Connectivity with NB-IoT

#### Description
The following functionalities will be verified in this test-case:
* **Cellular: On** / Off / CAT-M1 / **NB-IoT / Activate data-context / HTTP/POST**
* Led: On / Off / Change colors
* BMA280 (acc): On / Off / Correctness of the collected samples
* **BME280 (pressure, humidity, temperature): On** / Off / Correctness of the collected samples
* RTT: printf
* UART: printf

#### Setup
1. Initialize the bme280 sensor
2. Initialize Cellular with NB-IoT

#### Run
1. Start sampling data every 30 s
2. Send out via http the temperature and pressure data.

Wait 5 minutes

#### TearDown
1. Verify that the sent data are visible on the Backend

----------

### TC 1.1.4: Connectivity with CAT-M1

#### Description
The following functionalities will be verified in this test-case:
* **Cellular: On** / Off / **CAT-M1** / NB-IoT / **Activate data-context / HTTP/POST**
* Led: On / Off / Change colors
* **BMA280 (acc): On** / Off / Correctness of the collected samples
* BME280 (pressure, humidity, temperature): On / Off / Correctness of the collected samples
* RTT: printf
* UART: printf

#### Setup
1. Initialize the bma280 sensor
2. Initialize Cellular with NB-IoT

#### Run
1. Start sampling data every 1 s
2. Send out via http the accelerometer data.

Move the device around.

#### TearDown
1. Verify that the sent data are visible on the Backend and make sense based on the device movement

----------

### TC 1.1.5: Test uart

#### Description
The following functionalities will be verified in this test-case:
* Cellular: On / Off / CAT-M1 / NB-IoT / Activate data-context / HTTP/POST
* Led: On / Off / Change colors
* **BMA280** (acc): **On** / Off / **Correctness of the collected samples**
* BME280 (pressure, humidity, temperature): On / Off / Correctness of the collected samples
* RTT: printf
* **UART: printf**

#### Setup
1. Initialize the bma280 sensor
2. Initialize UART

#### Run
1. Start sampling data every 500 ms
2. Print output out via UART

Do x times:
3. Push device in an orthogonal plan and verify on the xterm that the axes are correct

#### TearDown
None

----------