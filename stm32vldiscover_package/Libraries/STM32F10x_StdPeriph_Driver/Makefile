LIBRARY = libstm32_std_periph.a

INCLUDES = \
	-I../../../Assignment2_cbartlett/Bsp/ \
	-I../CMSIS/CM3/DeviceSupport/ST/STM32F10x \
	-I../CMSIS/CM3/CoreSupport \
	-I./inc

all :
	rm -f lib/$(LIBRARY)
	echo $(INCLUDES)
	arm-none-eabi-gcc $(INCLUDES) -DUSE_FULL_ASSERT -DUSE_STDPERIPH_DRIVER -DSTM32F10X_MD_VL -mcpu=cortex-m3 -mthumb -c src/*.c
	arm-none-eabi-ar rcs $(LIBRARY) *.o
	mv $(LIBRARY) ./lib
	make clean

.PHONY: clean
clean:
	rm -f *.o
