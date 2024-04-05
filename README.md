# ft_ping

- This project reprecate `ping` of `inetutils-2.0`
- It takes a simple IPV4 as parameters
  - address and hostname
- It manages Fully Qualified Domain Name (FQDN) without DNS resolution

### default compilation

- Supports `-v` and `-?` options

  | option | details |
  | :- | :- |
  | -v, --verbose | verbose output |
  | -?, --help | give this help list |

### bonus compilation

- Supports `-w` option

  | option | details |
  | :- | :- |
  | -c, --count=NUMBER | stop after sending NUMBER packets |
  | -q, --quiet | quiet output |
  | -w, --timeout=N | stop after N seconds |

## Usage

- Compilation
  ```
  make
  ```

- Run docker
  ```
  make build
  ```
  ```
  make exec
  ```

## References

- Internet Controle Message Protocol (ICMP) RFC792
  - https://datatracker.ietf.org/doc/html/rfc792

- Requirements for Internet Hosts -- Communication Layers RFC1122
  - https://datatracker.ietf.org/doc/html/rfc1122
  - Section: [Timestamp Option](https://datatracker.ietf.org/doc/html/rfc1122#:~:text=is%20OPTIONAL.%0A%0A%0A%20%20%20%20%20%20%20%20%20%20%20%20(e)-,Timestamp%20Option,-Implementation%20of%20originating)

# Tests

## Traffic Controle (`tc`)

> ! Under ssh environment, these changes will make the connection unstable.
> Highly recommended to run directly on the machine.

- Reset
  ```
  sudo tc qdisc del dev enp0s3 root
  ```

#### Create delay

- How

  ```
  sudo tc qdisc add dev enp0s3 root netem delay 1000ms
  ```

- Expect: change in triptime
  ```shell
  $> ping google.com
  ...
  64 bytes from 142.250.179.110: icmp_seq=2 ttl=63 time=1008,595 ms
  64 bytes from 142.250.179.110: icmp_seq=3 ttl=63 time=1008,336 ms
  ...
  ```

#### Create packet losses
- How
  ```
  sudo tc qdisc add dev enp0s3 root netem loss 50%
  ```

- Expect: change in summary
  ```shell
  $> ping google.com
  ```

#### Create packet corruptions

- How
  ```
  sudo tc qdisc add dev enp0s3 root netem corrupt 50%
  ```

- Expect
  ```shell
  $> ping google.com
  ...
  64 bytes from 142.250.179.110: icmp_seq=7 ttl=63 time=8,070 ms
  64 bytes from 142.250.179.110: icmp_seq=10 ttl=63 time=7,956 ms
  64 bytes from 142.250.179.110: icmp_seq=11 ttl=63 time=7,912 ms
  --- google.com ping statistics ---
  12 packets transmitted, 10 packets received, 16% packet loss
  round-trip min/avg/max/stddev = 7,434/8,106/9,049/0,426 ms
  ```

## Dynamic Injection (`LD_PRELOAD`)

Create `fakepid.c`
```c
#include <sys/types.h>
#include <stdio.h>

pid_t	getpid(void)
{
	printf("--- PID replaceed with 4242 ---\n");
	return 4242;
}
```

- Compile and create a shared object file `lib_fakepid.so`
```
gcc -shared fakepid.c -o lib_fakepid.so
```

- Replace getpid with fake one (! Does not work without `sudo` !)
```
sudo LD_PRELOAD=./lib_fakepid.so ping google.com
```

### Duplicated id (pid)

#### Same destination
- How
  ```
  sudo LD_PRELOAD=./lib_fakepid.so ping google.com
  ```
  ```
  sudo LD_PRELOAD=./lib_fakepid.so ping google.com
  ```

- Expect: Duplication detection
  ```shell
  $> sudo LD_PRELOAD=./lib_fakepid.so ping google.com
  ...
  64 bytes from 142.250.179.110: icmp_seq=0 ttl=63 time=7,476 ms (DUP!)
  64 bytes from 142.250.179.110: icmp_seq=4 ttl=63 time=8,242 ms
  64 bytes from 142.250.179.110: icmp_seq=1 ttl=63 time=7,581 ms (DUP!)
  64 bytes from 142.250.179.110: icmp_seq=5 ttl=63 time=8,101 ms
  ...
  --- google.com ping statistics ---
  8 packets transmitted, 8 packets received, +4 duplicates, 0% packet loss
  ```
  ```shell
  $> sudo LD_PRELOAD=./lib_fakepid.so ping google.com
  ...
  64 bytes from 142.250.179.110: icmp_seq=4 ttl=63 time=8,128 ms
  64 bytes from 142.250.179.110: icmp_seq=1 ttl=63 time=7,925 ms
  64 bytes from 142.250.179.110: icmp_seq=5 ttl=63 time=7,988 ms
  64 bytes from 142.250.179.110: icmp_seq=2 ttl=63 time=8,697 ms
  4 packets transmitted, 7 packets received, -- somebody is printing forged packets!
  ```

#### Different destinations
- How
  ```
  sudo LD_PRELOAD=./lib_fakepid.so ping google.com
  ```
  ```
  sudo LD_PRELOAD=./lib_fakepid.so ping localhost
  ```

- Expect: Both packets are captured, Duplication detection
  ```
  $> sudo LD_PRELOAD=./lib_fakepid.so ping localhost
  ...
  64 bytes from 142.250.179.110: icmp_seq=0 ttl=63 time=7,209 ms (DUP!)
  64 bytes from 127.0.0.1: icmp_seq=3 ttl=64 time=0,115 ms
  64 bytes from 142.250.179.110: icmp_seq=1 ttl=63 time=7,698 ms (DUP!)
  64 bytes from 127.0.0.1: icmp_seq=4 ttl=64 time=0,114 ms
  ```