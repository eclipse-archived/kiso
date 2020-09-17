FROM eclipse/kiso-build-env:v0.0.8

LABEL Description="Eclipse Kiso Jenkins Agent container, used to execute hardware integation tests"

ARG VERSION=4.3
ARG user=jenkins
ARG group=jenkins
ARG uid=1000
ARG gid=1000
ARG AGENT_WORKDIR=/home/${user}/agent

RUN groupadd -g ${gid} ${group}
RUN useradd -c "Jenkins Agent user" -d /home/${user} -u ${uid} -g ${gid} -m ${user}

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
    curl \
    docker.io \
    git-lfs \
    libncurses5 \
    && apt-get clean
RUN curl --create-dirs -fsSLo /usr/share/jenkins/agent.jar https://repo.jenkins-ci.org/public/org/jenkins-ci/main/remoting/${VERSION}/remoting-${VERSION}.jar \
    && chmod 755 /usr/share/jenkins \
    && chmod 644 /usr/share/jenkins/agent.jar \
    && ln -sf /usr/share/jenkins/agent.jar /usr/share/jenkins/slave.jar
RUN JLINK_ARCH=`([ $(uname -m) = 'x86_64' ] && echo 'x86_64') || ([ $(uname -m) = 'armv7l' ] && echo 'arm') || ([ $(uname -m) = 'aarch64' ] && echo 'arm64')` \
    && TEMP_DEB="$(mktemp)" \
    && curl -X POST -F 'accept_license_agreement=accepted' "https://www.segger.com/downloads/jlink/JLink_Linux_${JLINK_ARCH}.deb" -o "$TEMP_DEB" \
    && dpkg -i "$TEMP_DEB" \
    && rm -f "$TEMP_DEB"

USER ${user}
ENV AGENT_WORKDIR=${AGENT_WORKDIR}
RUN mkdir /home/${user}/.jenkins && mkdir -p ${AGENT_WORKDIR}

VOLUME /home/${user}/.jenkins
VOLUME ${AGENT_WORKDIR}

WORKDIR /home/${user}
