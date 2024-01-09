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
