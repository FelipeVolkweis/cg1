.PHONY: all clean run

BUILD_DIR = build
TARGET = $(BUILD_DIR)/cg

all: $(TARGET)

$(TARGET): CMakeLists.txt
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake ..
	@$(MAKE) -C $(BUILD_DIR)

run: all
	@./$(TARGET)

clean:
	@rm -rf $(BUILD_DIR)
