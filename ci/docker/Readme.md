# Docker Image Creation For Jenkins

## Introduction

These `Dockerfile`s are the source of truth for the docker image used in our continuous integration. This means, modifying it will affect all builds.

* `development` contains the description for the `eclipse/kiso-build-env` image used by developers and CI/CD alike, to build and unit-test Kiso packages.
* `agent` contains the description for the `eclipse/kiso-integration-test-agent` image used by CI/CD for integration-testing. It adds a Jenkins agent to `eclipse/kiso-build-env`. Building it requires a build argument to indicate whether or not to install JLink. Use `docker build ... --build-arg ACCEPT_JLINK_TERMS_OF_USE='accepted'` if you have read and accepted the SEGGER's Terms of Use for this software. Alternatively, set `ACCEPT_JLINK_TERMS_OF_USE='not accepted'` to skip JLink installation. \
**NOTE: The image must not be re-distributed, if built with JLink enabled! This option is intended for your local setup only.**

## Installation of docker

Please check the following installation manuals:

* [For Windows 10](https://runnable.com/docker/install-docker-on-windows-10)
* [For Linux](https://runnable.com/docker/install-docker-on-linux)

## Creation of the image

In `development` directory, open a terminal and type `docker build . --tag=<image_tag>` to build the image.

## Verification of the image

You can run the image in a container with `docker run -it <image_tag> bash`

## Deploy an updated image

To deploy a modified image open a terminal and enter the following:

```sh
docker login <registry_address> # username and password entered interactively
docker push <image_tag_or_id> <username>/<image_tag>:<version>
```
