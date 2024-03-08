TARGET		:= ../ft_ping

TEST_RECV	:= test_recv
TEST_PID	:= test_pid

all	: recv pid

.PHONY	: recv
recv	:
	$(CC) -shared -fPIC -o lib$(TEST_RECV).so $(TEST_RECV).c -ldl

.PHONY	: pid
pid		:
	$(CC) -shared -fPIC -o lib$(TEST_PID).so $(TEST_PID).c

.PHONY	: run
run		:
	sudo LD_PRELOAD=./lib$(TEST_RECV).so $(TARGET) localhost

.PHONY	: clean
clean	:
	$(RM) *.so