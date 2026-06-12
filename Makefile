CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -O2 -Iinclude
SRC_DIR=src
BUILD_DIR=build
OBJS=$(BUILD_DIR)/main.o $(BUILD_DIR)/graph.o $(BUILD_DIR)/utils.o $(BUILD_DIR)/routing.o $(BUILD_DIR)/security.o $(BUILD_DIR)/flow.o $(BUILD_DIR)/scheduler.o $(BUILD_DIR)/report.o

all: netguard

netguard: $(OBJS)
	$(CC) $(CFLAGS) -o netguard $(OBJS)

$(BUILD_DIR):
	if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q $(BUILD_DIR)\*.o netguard.exe reports\vulnerability_report.txt reports\topology_visual.html topology_visual.html 2>NUL || exit 0
