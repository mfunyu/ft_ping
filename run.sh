#!/bin/bash

# ---------------------------------------------------------------------------- #
# define
THICK="\033[1m"
CYAN="\033[36m"
RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
RESET="\033[m"
PROMPT="${THICK}${CYAN}\$${RESET}"
# ---------------------------------------------------------------------------- #

# Set up a trap to catch Ctrl+C (SIGINT)
trap SIGINT

exe () {
	printf "$PROMPT "
	echo $@
	"$@"
}

header () {
	printf "${THICK}${CYAN}"
	echo $@
	printf "${RESET}"
}

discription () {
	printf "${CYAN}"
	echo $@
	printf "${RESET}"
}

run_test () {
	echo "--------------------"
	exe sudo ./ft_ping $1
	# exe ping $1
	echo "--------------------"
}

test_delay () {
	discription [+] Add delay to the network interface
	exe sudo tc qdisc add dev enp0s3 root netem delay 1000ms

	run_test $1

	discription [-] Remove the delay from the network interface
	exe sudo tc qdisc del dev enp0s3 root
}

test_loss () {
	discription [+] Set packet loss to the network interface
	exe sudo tc qdisc add dev enp0s3 root netem loss 50%

	run_test $1

	discription [-] Remove the packet loss from the network interface
	exe sudo tc qdisc del dev enp0s3 root
}

test_corrupt () {
	discription [+] Create corrupted packets
	exe sudo tc qdisc add dev enp0s3 root netem corrupt 50%

	run_test $1

	discription [-] Remove the corruption setting from the network interface
	exe sudo tc qdisc del dev enp0s3 root
}

run () {
	header ========[[ $@ ]]=========
	"$@"
	echo
}

run run_test 10.0.0.0
run run_test 10.0.2.14
run test_delay google.com
run test_delay localhost
run test_loss google.com
run test_loss localhost
