# How to run Tests

## Basic test: `./run.sh`

- basic testings with traffic controle using `tc` command
- `Ctrl+C` for proceeding to a next test

#### How
```
./run.sh
```

## Advanced test: `LD_PRELOAD`

- replace certain functions for check error handlings

#### How
```
cd test
make
```

#### Commands
```
sudo LD_PRELOAD=./libtest_pid.so ../ft_ping google.com
```
```
sudo LD_PRELOAD=./libtest_pid.so ../ft_ping localhost
```

#### Test cases

- same pid (`libtest_pid.so`)
	- same destination
	- differenct destination

- bad checksum (`libtest_recv.so`)
	- good destination `google.com` (checksum mismatch)
	- bad destination `10.0.0.2` (ignored)

-