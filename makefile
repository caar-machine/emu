ifneq ($(words $(MAKECMDGOALS)),1)
.DEFAULT_GOAL = all
%:
	@$(MAKE) $@ --no-print-directory -rRf $(firstword $(MAKEFILE_LIST))
else
ifndef ECHO
T := $(shell $(MAKE) $(MAKECMDGOALS) --no-print-directory -nrRf $(firstword $(MAKEFILE_LIST)) ECHO="COUNTTHIS" | grep -c "COUNTTHIS")
N := x
C = $(words $N)$(eval N := x $N)
ECHO=printf "[$C/$T] $1     \r"
endif


CC ?= g++
CFILES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp)
HFILES = $(wildcard $(SRC_DIR)/*.hpp) $(wildcard $(SRC_DIR)/*/*.hpp)
OFILES = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(CFILES))

CFLAGS = -std=c++17 -Wall -Werror -Wextra -Isrc -DGIT_HASH=\"$(shell git rev-parse --short HEAD)\"
LDFLAGS = -lfmt -lSDL2

BINARY = $(TARGET)

BUILD_DIR = build/$(TARGET)
SRC_DIR = src

TARGET = caar-emu

all: $(BINARY)

$(BUILD_DIR)/%.o: %.cpp $(HFILES)
	@mkdir -p $(@D)
	@$(call ECHO,CC $@)
	@$(CC) $(CFLAGS) -c -o $@ $< 

$(BINARY): $(OFILES)
	@mkdir -p $(@D)
	@$(call ECHO,LD $@)
	@$(CC) -o $@ $^ $(LDFLAGS)

clean:
	@-rm -rf $(BUILD_DIR) $(BINARY)
endif
