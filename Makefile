.PHONY: all appimage clean clean-appimage run run-appimage

BUILD_DIR = build
TARGET = $(BUILD_DIR)/cg
BIN_DIR = bin
APPIMAGE = $(BIN_DIR)/cg.AppImage
APPIMAGE_TOOL = $(BIN_DIR)/appimagetool-x86_64.AppImage
APPIMAGE_TOOL_URL = https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage

SOURCES := $(shell find src 3rdparty/glad -type f)
ASSETS := $(shell find data shaders -type f)

all: $(TARGET)

$(TARGET): CMakeLists.txt $(SOURCES)
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake ..
	@$(MAKE) -C $(BUILD_DIR)

run: all
	@./$(TARGET)

$(APPIMAGE_TOOL):
	@mkdir -p $(BIN_DIR)
	@curl --fail --location --retry 3 --output $@ $(APPIMAGE_TOOL_URL)
	@chmod +x $@

$(APPIMAGE): Makefile $(TARGET) $(ASSETS) packaging/cg.desktop packaging/cg.png \
		packaging/package-appimage.sh $(APPIMAGE_TOOL)
	@./packaging/package-appimage.sh $(TARGET) $(APPIMAGE) $(APPIMAGE_TOOL)

appimage: $(APPIMAGE)

run-appimage: $(APPIMAGE)
	@./$(APPIMAGE) --appimage-extract-and-run

clean:
	@rm -rf $(BUILD_DIR)

clean-appimage:
	@rm -rf $(APPIMAGE) tmp/cg.AppDir
