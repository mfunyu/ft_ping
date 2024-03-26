TARGET		:= ../ft_ping

TEST_RECV	:= test_recv
TEST_PID	:= test_pid
TEST_TIME	:= test_time

all	: recv pid

.PHONY	: recv
recv	:
	$(CC) -shared -fPIC -o lib$(TEST_RECV).so $(TEST_RECV).c -ldl

.PHONY	: pid
pid		:
	$(CC) -shared -fPIC -o lib$(TEST_PID).so $(TEST_PID).c

.PHONY	: time
time	:
	$(CC) -shared -fPIC -o lib$(TEST_TIME).so $(TEST_TIME).c -ldl

.PHONY	: run
run		:
	sudo LD_PRELOAD=./lib$(TEST_RECV).so $(TARGET) localhost

.PHONY	: clean
clean	:
	$(RM) *.so