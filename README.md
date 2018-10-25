# stm32f103cb_co-m_Ref

# make command
make CUBEMX_DIR=/home/kdj6724/STM32Cube

# flash write command
st-flash --reset write build/stm32f103cb.bin 0x08000000
