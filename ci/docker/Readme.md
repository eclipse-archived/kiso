# Docker Image Creation For Jenkins

## Introduction

These `Dockerfile`s are the source of truth for the docker image used in our continuous integration. This means, modifying it will affect all builds.

* `development` contains the description for the `eclipse/kiso-build-env` image used by developers and CI/CD alike, to build and unit-test Kiso packages.
* `agent` contains the description for the `eclipse/kiso-agent` image used by CI/CD for integration-testing. It adds a Jenkins agent to `eclipse/kiso-build-env`.

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
