# Explicitly add kernel sources specific to the Xtensa optimized
# implementations.
MICROLITE_CC_KERNEL_SRCS += \
  tensorflow/lite/micro/kernels/xtensa/conv_hifi.cc \
  tensorflow/lite/micro/kernels/xtensa/conv_int8_reference.cc \
  tensorflow/lite/micro/kernels/xtensa/conv_int16_reference.cc \
  tensorflow/lite/micro/kernels/xtensa/depthwise_conv_hifi.cc \
  tensorflow/lite/micro/kernels/xtensa/softmax_int8_int16.cc

ifeq ($(TARGET_ARCH), hifi5)
  DOWNLOAD_RESULT := $(shell $(MAKEFILE_DIR)/ext_libs/xtensa_download.sh ${MAKEFILE_DIR}/downloads hifi5)
  ifneq ($(DOWNLOAD_RESULT), SUCCESS)
    $(error Something went wrong with the xtensa download: $(DOWNLOAD_RESULT))
  endif

  # TODO(b/161489252): -Wno-shadow is only needed for xannlib. But since we do
  # not have separate cflags (or the concept of modular build targets) with the
  # Makefile, -Wno-shadow will be used for everything.

  PLATFORM_FLAGS = \
    -DNNLIB_HIFI5 \
    -Wno-shadow

  CCFLAGS += $(PLATFORM_FLAGS)
  CXXFLAGS += $(PLATFORM_FLAGS)

  NNLIB_PATH := $(MAKEFILE_DIR)/downloads/xa_nnlib_hifi5

  THIRD_PARTY_KERNEL_CC_SRCS += \
    $(shell find $(NNLIB_PATH) -name "*.c")

  EXCLUDED_NNLIB_SRCS = \
    $(NNLIB_PATH)/algo/layers/cnn/src/xa_nn_cnn_api.c \
    $(NNLIB_PATH)/algo/layers/gru/src/xa_nn_gru_api.c \
    $(NNLIB_PATH)/algo/layers/lstm/src/xa_nn_lstm_api.c

  THIRD_PARTY_KERNEL_CC_SRCS := $(filter-out $(EXCLUDED_NNLIB_SRCS), $(THIRD_PARTY_KERNEL_CC_SRCS))

  THIRD_PARTY_CC_HDRS += \
    $(shell find $(NNLIB_PATH) -name "*.h")

  INCLUDES += \
    -I$(NNLIB_PATH)/ \
    -I$(NNLIB_PATH)/algo/kernels/ \
    -I$(NNLIB_PATH)/include/nnlib/ \
    -I$(NNLIB_PATH)/include/ \
    -I$(NNLIB_PATH)/algo/common/include/ \
    -I$(NNLIB_PATH)/algo/ndsp/hifi5/include/

else ifeq ($(TARGET_ARCH), $(filter $(TARGET_ARCH), hifi4 hifi4_internal))

  DOWNLOAD_RESULT := $(shell $(MAKEFILE_DIR)/ext_libs/xtensa_download.sh ${MAKEFILE_DIR}/downloads hifi4)
  ifneq ($(DOWNLOAD_RESULT), SUCCESS)
    $(error Something went wrong with the xtensa download: $(DOWNLOAD_RESULT))
  endif

  # TODO(b/161489252): -Wno-shadow is only needed for xannlib. But since we do
  # not have separate cflags (or the concept of modular build targets) with the
  # Makefile, -Wno-shadow will be used for everything.

  PLATFORM_FLAGS = \
    -DNNLIB_V2 \
    -Wno-shadow

  CCFLAGS += $(PLATFORM_FLAGS)
  CXXFLAGS += $(PLATFORM_FLAGS)

  NNLIB_PATH := $(MAKEFILE_DIR)/downloads/xa_nnlib_hifi4

  THIRD_PARTY_KERNEL_CC_SRCS += \
    $(shell find $(NNLIB_PATH) -name "*.c")

  EXCLUDED_NNLIB_SRCS = \
    $(NNLIB_PATH)/algo/layers/cnn/src/xa_nn_cnn_api.c \
    $(NNLIB_PATH)/algo/layers/gru/src/xa_nn_gru_api.c \
    $(NNLIB_PATH)/algo/layers/lstm/src/xa_nn_lstm_api.c

  THIRD_PARTY_KERNEL_CC_SRCS := $(filter-out $(EXCLUDED_NNLIB_SRCS), $(THIRD_PARTY_KERNEL_CC_SRCS))

  THIRD_PARTY_CC_HDRS += \
    $(shell find $(NNLIB_PATH) -name "*.h")

  INCLUDES += \
    -I$(NNLIB_PATH)/ \
    -I$(NNLIB_PATH)/algo/kernels/ \
    -I$(NNLIB_PATH)/include/nnlib/ \
    -I$(NNLIB_PATH)/include/ \
    -I$(NNLIB_PATH)/algo/common/include/ \
    -I$(NNLIB_PATH)/algo/ndsp/hifi4/include/

else ifeq ($(TARGET_ARCH), vision_p6)
  $(info We do not have XaNNlib for TARGET_ARCH=vision_p6)
else
  $(error Unsupported TARGET_ARCH=$(TARGET_ARCH))
endif
