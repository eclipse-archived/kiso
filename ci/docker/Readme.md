# Docker Image Creation For Jenkins

## Introduction
The Dockerfile file is the source of truth for the docker image used in our continuous integration.
This means, modifying it will affect all builds.

## Installation of docker
Please check the following install manuals:
* [For Windows 10](https://runnable.com/docker/install-docker-on-windows-10)
* [For Linux](https://runnable.com/docker/install-docker-on-linux)

## Creation of the image
In this folder, open a Terminal and
* to build the image: `docker build --tag=<image_tag> .`

## Verification of the image
You can run the image in a container with `docker run -it <image_tag> bash`

## Commit of the image
open a Terminal and
* to commit the image into the registry: `docker tag <image_tag> <registry_address>:<version>`
* to log into the registry: `docker login <registry_address>`
* to push the image into the registry: `docker push <registry_address>:<version>`
