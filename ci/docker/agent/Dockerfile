FROM eclipse/kiso-build-env:v0.0.8

LABEL Description="Eclipse Kiso Jenkins Agent container, used to execute hardware integation tests"

ARG VERSION=4.3
ARG user=jenkins
ARG group=jenkins
ARG uid=1000
ARG gid=1000
ARG AGENT_WORKDIR=/home/${user}/agent

# Change to 'accepted' if you read and agreed to the SEGGER Terms of Use
# available at https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack
# Otherwise set to 'not accepted' to skip JLink installation.
ARG ACCEPT_JLINK_TERMS_OF_USE=

RUN groupadd -g ${gid} ${group}
RUN useradd -c "Jenkins Agent user" -d /home/${user} -u ${uid} -g ${gid} -m ${user}

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
    curl \
    docker.io \
    git-lfs \
    libncurses5 \
    && apt-get clean

RUN JLINK_ARCH=`([ $(uname -m) = 'x86_64' ] && echo 'x86_64') || ([ $(uname -m) = 'armv7l' ] && echo 'arm') || ([ $(uname -m) = 'aarch64' ] && echo 'arm64')` \
    && [ -z "$ACCEPT_JLINK_TERMS_OF_USE" ] \
    && (echo "\n################################################################################\n" \
    "\nBuild argument ACCEPT_JLINK_TERMS_OF_USE not set. Please read the below stated SEGGER Terms of Use and set docker build argument ACCEPT_JLINK_TERMS_OF_USE to 'accepted' (to install JLink) or 'not accepted' (to skip installation).\n" \
    "\n################################################################################\n" \
    ; curl -fsSL "https://www.segger.com/downloads/jlink/JLink_Linux_${JLINK_ARCH}.deb" \
    | xmllint --html --xpath "//body/div[@id='page']/main/div[@id='productdetails']//textarea/text()" - 2>/dev/null \
    && echo '\n\n' \
    ; exit 123) \
    || [ "$ACCEPT_JLINK_TERMS_OF_USE" = 'accepted' -o "$ACCEPT_JLINK_TERMS_OF_USE" = 'not accepted' ] \
    && true \
    || (echo "Invalid build argument value ACCEPT_JLINK_TERMS_OF_USE='${ACCEPT_JLINK_TERMS_OF_USE}'"; exit 124);

# Workaround for new buggy update-ca-certificates in Debian Buster on ARM 32-bit
# https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=923479
RUN ([ ! $(uname -m) = 'armv7l' ] && true) || ([ $(uname -m) = 'armv7l' ] && c_rehash)

RUN curl --create-dirs -fsSLo /usr/share/jenkins/agent.jar https://repo.jenkins-ci.org/public/org/jenkins-ci/main/remoting/${VERSION}/remoting-${VERSION}.jar \
    && chmod 755 /usr/share/jenkins \
    && chmod 644 /usr/share/jenkins/agent.jar \
    && ln -sf /usr/share/jenkins/agent.jar /usr/share/jenkins/slave.jar
RUN JLINK_ARCH=`([ $(uname -m) = 'x86_64' ] && echo 'x86_64') || ([ $(uname -m) = 'armv7l' ] && echo 'arm') || ([ $(uname -m) = 'aarch64' ] && echo 'arm64')` \
    && ([ "$ACCEPT_JLINK_TERMS_OF_USE" = 'not accepted' ] \
    && true \
    || (TEMP_DEB="$(mktemp)" \
    && curl -fsSL -X POST -F 'accept_license_agreement=accepted' "https://www.segger.com/downloads/jlink/JLink_Linux_${JLINK_ARCH}.deb" -o "$TEMP_DEB" \
    && dpkg -i "$TEMP_DEB" \
    && rm -f "$TEMP_DEB"))

USER ${user}
ENV AGENT_WORKDIR=${AGENT_WORKDIR}
RUN mkdir /home/${user}/.jenkins && mkdir -p ${AGENT_WORKDIR}

VOLUME /home/${user}/.jenkins
VOLUME ${AGENT_WORKDIR}

WORKDIR /home/${user}
