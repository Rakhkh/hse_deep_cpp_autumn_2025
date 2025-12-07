PYTHON = python
PYTHON_VERSION = $(shell $(PYTHON) -c "import sys; print(f'{sys.version_info.major}{sys.version_info.minor}')")

PYTHON_PREFIX = $(shell $(PYTHON) -c "import sys; print(sys.prefix)")
PYTHON_INCLUDES = $(PYTHON_PREFIX)/include
PYTHON_LIBS = $(PYTHON_PREFIX)/libs

MODULE = cjson
SOURCE = cjson.c

ifeq ($(OS),Windows_NT)
    TARGET = $(MODULE).pyd  
    CC = gcc

    CFLAGS = -O2 -Wall -shared -municode -I$(PYTHON_INCLUDES)

    LDFLAGS = -L$(PYTHON_LIBS) -lpython$(PYTHON_VERSION)
else
    TARGET = $(MODULE).so
    CC = gcc
    CFLAGS = -O2 -Wall -fPIC -I$(PYTHON_INCLUDES)
    LDFLAGS = -shared
endif

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) $(LDFLAGS) -o $(TARGET)

test: $(TARGET)
	@$(PYTHON) test_cjson.py 2>&1 | tail -20
	@$(PYTHON) benchmark.py 2>&1 | tail -40

clean:
	rm -f *.pyd *.so *.o *.dll

check:
	@echo "Python executable: $(PYTHON)"
	@echo "Python version: $(PYTHON_VERSION)"
	@echo "Python prefix: $(PYTHON_PREFIX)"
	@echo "Includes exists: $(wildcard $(PYTHON_INCLUDES)/Python.h)"
	@echo "Libs directory exists: $(wildcard $(PYTHON_LIBS)/*)"
	@ls -la "$(PYTHON_LIBS)" 2>/dev/null || echo "Не удалось прочитать libs"

.PHONY: all test clean check