FROM debian:11

RUN apt-get -y update \
	&& apt-get -y install inetutils-ping \
	&& apt-get -y install tcpdump make gcc libc6 iproute2

WORKDIR /workdir

CMD [ "tail", "-f", "/dev/null" ]
