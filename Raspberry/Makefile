DIR_Config   = ./lib/Config
DIR_EPD      = ./lib/e-Paper
DIR_FONTS    = ./lib/Fonts
DIR_GUI      = ./lib/GUI
DIR_Examples = ./examples

DIR_BIN      = ./bin

OBJ_C = $(wildcard ${DIR_Config}/*.c ${DIR_EPD}/*.c ${DIR_FONTS}/*.c ${DIR_GUI}/*.c ${DIR_Examples}/*.c)
OBJ_O = $(patsubst %.c,${DIR_BIN}/%.o,$(notdir ${OBJ_C}))

TARGET = epd

CC = gcc

MSG = -g -O0 -Wall
DEBUG = -D USE_DEBUG
STD = -std=gnu99

CFLAGS += $(MSG) $(DEBUG) $(STD)

LIB_USE = -lm -lrt -lpthread

LIB = BCM
# LIB = LGPIO
# LIB = GPIOD
ifeq ($(LIB), BCM)
    LIB_USE += -lbcm2835
	OBJ_O := $(filter-out ${DIR_BIN}/RPI_gpiod.o ${DIR_BIN}/dev_hardware_SPI.o, ${OBJ_O})
else ifeq ($(LIB), LGPIO)
    LIB_USE += -llgpio -lm
	OBJ_O := $(filter-out ${DIR_BIN}/RPI_gpiod.o ${DIR_BIN}/dev_hardware_SPI.o, ${OBJ_O})
else ifeq ($(LIB), GPIOD)
    LIB_USE += -lgpiod -lm
endif

$(shell mkdir -p $(DIR_BIN))

${TARGET}: ${OBJ_O}
	$(CC) $(CFLAGS) $^ -o $@ $(LIB_USE)

${DIR_BIN}/%.o: ${DIR_Config}/%.c
	$(CC) $(CFLAGS) -D $(LIB) -c $< -o $@

${DIR_BIN}/%.o: ${DIR_EPD}/%.c
	$(CC) $(CFLAGS) -c $< -o $@

${DIR_BIN}/%.o: ${DIR_FONTS}/%.c
	$(CC) $(CFLAGS) -c $< -o $@

${DIR_BIN}/%.o: ${DIR_GUI}/%.c
	$(CC) $(CFLAGS) -c $< -o $@

${DIR_BIN}/%.o: ${DIR_Examples}/%.c
	$(CC) $(CFLAGS) -c $< -o $@

${DIR_BIN}/%.d: ${DIR_Config}/%.c
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< | sed 's,^,$(DIR_BIN)/,' > $@

-include $(patsubst %.c,${DIR_BIN}/%.d,$(notdir ${OBJ_C}))

clean:
	rm -rf $(DIR_BIN)/* $(TARGET)
