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

-

## Tests

### Traffic Controle (`tc`)

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
  ```

### Dynamic Injection (`LD_PRELOAD`)

#### Dublicated id (pid)

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
