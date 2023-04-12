################################################################################
# MRS Version: {"version":"1.8.4","date":"2023/02/015"}
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/libraries/zf_driver/zf_driver_adc.c \
C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/libraries/zf_driver/zf_driver_delay.c \
C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/libraries/zf_driver/zf_driver_dvp.c \
C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/libraries/zf_driver/zf_driver_encoder.c \
C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/libraries/zf_driver/zf_driver_exti.c \
C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/libraries/zf_driver/zf_driver_flash.c \
C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/libraries/zf_driver/zf_driver_gpio.c \
C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/libraries/zf_driver/zf_driver_iic.c \
C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/libraries/zf_driver/zf_driver_pit.c \
C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/libraries/zf_driver/zf_driver_pwm.c \
C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/libraries/zf_driver/zf_driver_soft_iic.c \
C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/libraries/zf_driver/zf_driver_soft_spi.c \
C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/libraries/zf_driver/zf_driver_spi.c \
C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/libraries/zf_driver/zf_driver_timer.c \
C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/libraries/zf_driver/zf_driver_uart.c \
C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/libraries/zf_driver/zf_driver_usb_cdc.c 

OBJS += \
./zf_driver/zf_driver_adc.o \
./zf_driver/zf_driver_delay.o \
./zf_driver/zf_driver_dvp.o \
./zf_driver/zf_driver_encoder.o \
./zf_driver/zf_driver_exti.o \
./zf_driver/zf_driver_flash.o \
./zf_driver/zf_driver_gpio.o \
./zf_driver/zf_driver_iic.o \
./zf_driver/zf_driver_pit.o \
./zf_driver/zf_driver_pwm.o \
./zf_driver/zf_driver_soft_iic.o \
./zf_driver/zf_driver_soft_spi.o \
./zf_driver/zf_driver_spi.o \
./zf_driver/zf_driver_timer.o \
./zf_driver/zf_driver_uart.o \
./zf_driver/zf_driver_usb_cdc.o 

C_DEPS += \
./zf_driver/zf_driver_adc.d \
./zf_driver/zf_driver_delay.d \
./zf_driver/zf_driver_dvp.d \
./zf_driver/zf_driver_encoder.d \
./zf_driver/zf_driver_exti.d \
./zf_driver/zf_driver_flash.d \
./zf_driver/zf_driver_gpio.d \
./zf_driver/zf_driver_iic.d \
./zf_driver/zf_driver_pit.d \
./zf_driver/zf_driver_pwm.d \
./zf_driver/zf_driver_soft_iic.d \
./zf_driver/zf_driver_soft_spi.d \
./zf_driver/zf_driver_spi.d \
./zf_driver/zf_driver_timer.d \
./zf_driver/zf_driver_uart.d \
./zf_driver/zf_driver_usb_cdc.d 


# Each subdirectory must supply rules for building sources it contributes
zf_driver/zf_driver_adc.o: C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/libraries/zf_driver/zf_driver_adc.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\Libraries\doc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Core" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Ld" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Peripheral" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Startup" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\user\inc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_common" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_device" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\code" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
zf_driver/zf_driver_delay.o: C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/libraries/zf_driver/zf_driver_delay.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\Libraries\doc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Core" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Ld" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Peripheral" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Startup" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\user\inc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_common" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_device" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\code" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
zf_driver/zf_driver_dvp.o: C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/libraries/zf_driver/zf_driver_dvp.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\Libraries\doc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Core" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Ld" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Peripheral" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Startup" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\user\inc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_common" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_device" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\code" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
zf_driver/zf_driver_encoder.o: C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/libraries/zf_driver/zf_driver_encoder.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\Libraries\doc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Core" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Ld" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Peripheral" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Startup" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\user\inc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_common" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_device" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\code" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
zf_driver/zf_driver_exti.o: C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/libraries/zf_driver/zf_driver_exti.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\Libraries\doc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Core" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Ld" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Peripheral" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Startup" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\user\inc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_common" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_device" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\code" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
zf_driver/zf_driver_flash.o: C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/libraries/zf_driver/zf_driver_flash.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\Libraries\doc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Core" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Ld" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Peripheral" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Startup" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\user\inc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_common" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_device" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\code" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
zf_driver/zf_driver_gpio.o: C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/libraries/zf_driver/zf_driver_gpio.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\Libraries\doc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Core" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Ld" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Peripheral" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Startup" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\user\inc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_common" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_device" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\code" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
zf_driver/zf_driver_iic.o: C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/libraries/zf_driver/zf_driver_iic.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\Libraries\doc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Core" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Ld" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Peripheral" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Startup" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\user\inc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_common" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_device" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\code" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
zf_driver/zf_driver_pit.o: C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/libraries/zf_driver/zf_driver_pit.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\Libraries\doc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Core" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Ld" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Peripheral" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Startup" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\user\inc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_common" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_device" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\code" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
zf_driver/zf_driver_pwm.o: C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/libraries/zf_driver/zf_driver_pwm.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\Libraries\doc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Core" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Ld" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Peripheral" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Startup" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\user\inc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_common" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_device" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\code" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
zf_driver/zf_driver_soft_iic.o: C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/libraries/zf_driver/zf_driver_soft_iic.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\Libraries\doc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Core" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Ld" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Peripheral" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Startup" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\user\inc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_common" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_device" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\code" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
zf_driver/zf_driver_soft_spi.o: C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/libraries/zf_driver/zf_driver_soft_spi.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\Libraries\doc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Core" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Ld" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Peripheral" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Startup" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\user\inc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_common" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_device" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\code" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
zf_driver/zf_driver_spi.o: C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/libraries/zf_driver/zf_driver_spi.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\Libraries\doc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Core" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Ld" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Peripheral" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Startup" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\user\inc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_common" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_device" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\code" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
zf_driver/zf_driver_timer.o: C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/libraries/zf_driver/zf_driver_timer.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\Libraries\doc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Core" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Ld" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Peripheral" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Startup" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\user\inc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_common" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_device" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\code" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
zf_driver/zf_driver_uart.o: C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/libraries/zf_driver/zf_driver_uart.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\Libraries\doc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Core" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Ld" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Peripheral" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Startup" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\user\inc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_common" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_device" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\code" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
zf_driver/zf_driver_usb_cdc.o: C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/libraries/zf_driver/zf_driver_usb_cdc.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\Libraries\doc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Core" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Ld" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Peripheral" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Startup" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\user\inc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_common" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_device" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\code" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

