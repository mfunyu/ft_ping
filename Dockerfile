FROM debian:11

RUN apt-get -y update \
	&& apt-get -y install inetutils-ping

CMD [ "tail", "-f", "/dev/null" ]
