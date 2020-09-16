---
title: "Integration Testing - Setup Guide"
weight: 3
---

This advanced guide will teach you how to set up your own integration testing CI/CD infrastructure. The concept behind Kiso's integration tests is explained in [Integration Testing]({{< ref "../Concepts/integration_testing.md" >}}).

While integration tests can of course be executed manually by each developer, an automated CI/CD setup, in which integration tests are run with every commit, has proved to be of great value. However, the reliance on hardware of Kiso's integration tests present some challenges. For starters, the default [Eclipse/Kiso](https://github.com/eclipse/kiso) CI/CD setup runs in a fully virtualized Kubernetes environment. Thus it can only run non-hardware-reliant checks, such as compilation, static code analysis, format checks, unit-tests, etc. To enable physical access, remote agents need to be created, equipped with hardware to flash and communicate with the Device-under-Test (DuT) and acting as a proxy between Jenkins the DuT. Agents may also be able to physically interact with the DuT during a test (such as actuating a button electronically via GPIO toggle). Each agent identifies its capabilities via labels, following a directory-like naming-scheme `kiso/itf/...` (`itf` stands for "Integration Test Framework"). A separate `Jenkinsfile` located in `<project-root>/ci/itf` controls the build, flash and execution of integration tests.

Kiso's Integration Test Infrastructure follows a decentralized approach. You as a developer may set up **your own** test infrastructure, and either run automated tests for your private fork during development, or offer access to it to the community.

## Setting up Your own Agent

### Requirements

- **Device-under-Test**
- **Jenkins master**
- Linux based **Jenkins agent** computer (e.g. Raspberry Pi)
- **Flashing equipment** (if not built into DuT)
- Auxiliaries
  - **Communication channel to DuT to control integration test sequence**
  - _Additional hardware needed as per test specification (e.g. GSM simulator, shielding box, web-cam, etc.)_

(Jenkins master and agent may be combined on the same physical computer)

### Architecture

![integration test infrastructure](/images/integration_test_infrastructure_architecture.svg)

In this guide we will set up a Kiso Integration Test Agent, as depicted in the figure above. The setup consists of a Raspberry Pi 3 and a DuT -- in this case a STM32 Nucleo-F767ZI. We will not cover, how to set up a reverse proxy for your Jenkins setup and make it accessible to the public. In its simplest form, Jenkins just polls the GitHub repository every couple of minutes and starts a new build if new commits are found. Then, once all tests have ran, the build status is posted to GitHub. Internally the Jenkins master spawns a docker-based Jenkins agent with access to the host's devices (via `docker run --device /dev/... ...`). This way we can control the toolchain via `Dockerfiles` and ensure our builds are consistent with Eclipse's Jenkins Master, while at the same time being able to interact with the hardware. Access to JLink flash hardware is provided via a JLink Remote Server, running parallel to the Jenkins Master. The agent then uses an IP-based connection to the JLink Remote Server to flash and reset the hardware.

### Step by Step

Start with a fresh Debian-based Raspberry Pi OS (the "Lite" version should suffice). The SD card should be sized adequately to store several GB of tooling. Anything below 32 GB may quickly become a hassle, as different toolchain versions may persist in the form of docker images. Follow the official [Getting Started Guide](https://projects.raspberrypi.org/en/projects/raspberry-pi-getting-started) and set up a SSH connection from your dev-PC to access it remotely.

#### Installing the Tools

Once on your Pi you need to install a few tools:

- Jenkins - Follow [this guide](https://pkg.jenkins.io/debian-stable/) for Debian releases. As Jenkins may not list it as a dependency you may have to manually install a JRE:
  ```sh
  sudo apt install openjdk-8-jre # use 'apt list openjdk-*-jre' to see what versions are available
  ```
- Docker - Can be installed directly via `apt`:
  ```sh
  sudo apt install docker.io
  ```
- JLink - Download the `.deb` file for 32-bit ARM systems from [SEGGER](https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack) and install it via `dpkg`:
  ```sh
  sudo apt install libncurses5 # requirement for JLink
  sudo dpkg -i JLink_Linux_arm.deb
  ```
- Some more recommended tools you maybe want to install are: `vim`, `unattended-upgrades`, and `winbind`.

#### JLink Remote Service

We assume your Nucleo's on-chip [ST-Link is flashed with JLink firmware](https://www.segger.com/products/debug-probes/j-link/models/other-j-links/st-link-on-board/). This allows us to use JLink's excellent JLink Remote Server to access the hardware via TCP/IP-connection. To ensure the server is always running we will create a [systemd service](https://www.freedesktop.org/software/systemd/man/systemd.service.html).

Create a new service unit `/etc/systemd/system/jlinkremote.service`. Paste the following unit description into the newly created file:

```systemd
[Unit]
Description=JLink Remote Server allowing remote JLink Clients to access local hardware
PartOf=jenkins.service

[Service]
type=simple
ExecStart=/usr/bin/JLinkRemoteServer

[Install]
WantedBy=jenkins.service
```

Note that we specified this service to be `WantedBy=jenkins.service` and `PartOf=jenkins.service`. Thus if Jenkins is started or stopped JLink Remote will follow in turn.

Enable and start the service initially via:

```sh
sudo systemctl enable jlinkremote.service
sudo systemctl start jlinkremote.service
```

#### Jenkins Master & Agent

Make sure Jenkins is running:

```sh
systemctl status jenkins.service # will report if Jenkins is running or not
# ...
systemctl status jenkins.service # to start Jenkins
```

Open Jenkins' web-service in a browser (IP depends on your network, port 8080), and follow the initial setup instructions. We don't need any special plugins so the default selection should be ok.

Create a GitHub Organization job via "New Item" > give it a name > select "GitHub Organization" > "OK". In the following configuration screen provide your GitHub credentials (use access tokens!) and create a repository filter for Kiso. For token-based authentication make sure the token has the `repo:status` flag set in [GitHub Developer Settings](https://github.com/settings/apps) settings. Under "Project Recognizers" change the "Script Path" to `ci/itf/Jenkinsfile`. Change the scan interval to something reasonable, like every 5 or 10 minutes. All other settings can be left as default or tweaked at a later time.

The final piece in our setup is the Jenkins agent. As described earlier we use a docker-based agent, spawned by the Jenkins master. We can create such an agent by navigating to the Jenkins dashboard and hitting "Manage Jenkins" > "Manage Nodes and Clouds" > "New Node" > give it a name (e.g. "goofy") > select "Permanent Agent" > "OK".

![agent configuration](/images/integration_test_agent_config.png)

In the configuration screen set "Remote root directory" to `/home/jenkins`, set the appropriate labels (more on that below) and select launch method "Launch agent via execution of command on the master". Set "Launch command" to:

```sh
docker run -i --rm --name jenkins-agent-kiso --device /dev/ttyACM0 -e JLINK_REMOTE="$JLINK_REMOTE" --init eclipse/kiso-integration-test-agent java -jar /usr/share/jenkins/agent.jar`.
```

Check "Environment variables" and add `JLINK_REMOTE` with value `172.17.0.1`. This should be the IP address of the docker host as seen from the docker network the agent is part of.

Note the use of `--device /dev/ttyACM0` in the launch command. This should be the device-file representing the USB serial COM-port of the Nucleo-F767. If this device does not exist, the agent will not be able to start. Connect the Nucleo-F767 and check the presence of the COM-port via `ls -l /dev/ttyACM0`.

Save the node and hit "Launch agent". If everything is set up properly, the agent should boot and now (provided the correct labels are set) be ready for integration tests.

### Agent Labeling

Agents publish their capabilities via labels. The labels follow a directory-like naming-scheme. The following predefined categories exist as of now:

- `kiso/` - Identifies that label is part of Kiso project
  - `itf/` - Identifies that label is part of Kiso's Integration Test Framework
    - `connector/` - Available connectors
      - `jlink-via-local` - JLink flasher locally available.
      - `jlink-via-remote` - JLink flasher accessible via IP, `JLINK_REMOTE` environment variable must contain IP-address pointing to JLink Remote Server instance.
      - `uart` - COM-port available.
    - `dut/` - Available Device(s)-under-Test
      - `nucleof767` - STM32 Nucleo-F767

You may add your own labels as needed. A typical agent may have the following labels:

- `kiso/itf/connector/jlink-via-remote`
- `kiso/itf/connector/uart`
- `kiso/itf/dut/nucleof767`
