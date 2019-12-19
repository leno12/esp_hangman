CC      := clang
CCFLAGS := -Wall -std=c11
ASSIGNMENT := ass-b2
all: clean bin lib

bin:
	$(CC) $(CCFLAGS) -o $(ASSIGNMENT) $(ASSIGNMENT).c
	chmod +x $(ASSIGNMENT)

lib:
	$(CC) $(CCFLAGS) -shared -fPIC -o $(ASSIGNMENT).so $(ASSIGNMENT).c

test: all
	cd ./testscripts && \
	python3 tc_runner.py && \
	python3 tc_report.py && \
	xdg-open ./results/report.html

testjson: all
	mkdir -p ./testscripts/tmp
	mkdir -p ./testscripts/results
	rm -rf ./testscripts/tmp/*
	rm -rf ./testscripts/results/*
	cd ./testscripts && \
	python3 tc_runner.py
	runner_uploader
							

clean:
	rm -f $(ASSIGNMENT)
	rm -f $(ASSIGNMENT).so
	rm -rf testscripts/tmp/*
	rm -rf testscripts/results/*
