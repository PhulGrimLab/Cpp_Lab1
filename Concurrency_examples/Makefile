# 컴파일러 설정
CXX = g++
THREADFNC_MODE ?= 0
CXXFLAGS = -Wall -Wextra -std=c++17 -g -I./include -pthread -DTHREADFNC_MODE=$(THREADFNC_MODE)
LDFLAGS = -pthread

# 디렉토리 설정
SRC_DIR = src
BUILD_DIR = build

ifeq ($(OS),Windows_NT)
TARGET = $(BUILD_DIR)/my_concurrency_example.exe
MKDIR_BUILD = if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
REMOVE_BUILD = if exist "$(BUILD_DIR)" rmdir /s /q "$(BUILD_DIR)"
RUN_TARGET = .\$(subst /,\,$(TARGET))
else
TARGET = $(BUILD_DIR)/my_concurrency_example
MKDIR_BUILD = mkdir -p $(BUILD_DIR)
REMOVE_BUILD = rm -rf $(BUILD_DIR)
RUN_TARGET = ./$(TARGET)
endif

# 소스 파일 찾기
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# 기본 타겟
all: $(BUILD_DIR) $(TARGET)

# 빌드 디렉토리 생성
$(BUILD_DIR):
	$(MKDIR_BUILD)

# 실행 파일 링크
$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(CXXFLAGS)

# 오브젝트 파일 컴파일
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 클린업
clean:
	$(REMOVE_BUILD)

# 재빌드
rebuild: clean all

# 디버그 정보 추가
debug: CXXFLAGS += -DDEBUG -O0
debug: all

# 릴리즈 빌드
release: CXXFLAGS += -O2 -DNDEBUG
release: LDFLAGS += -s -pthread
release: all

# 실행
run: all
	$(RUN_TARGET)

# 병렬 모드(기본: THREADFNC_MODE=0)
parallel: THREADFNC_MODE=0
parallel: rebuild

# 직렬화 모드(THREADFNC_MODE=1)
serialized: THREADFNC_MODE=1
serialized: rebuild

.PHONY: all clean rebuild debug release run parallel serialized