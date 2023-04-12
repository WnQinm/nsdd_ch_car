################################################################################
# MRS Version: {"version":"1.8.4","date":"2023/02/015"}
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/project/code/gyro.c \
C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/project/code/imgproc.c \
C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/project/code/motor.c \
C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/project/code/pit.c \
C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/project/code/servo.c 

OBJS += \
./code/gyro.o \
./code/imgproc.o \
./code/motor.o \
./code/pit.o \
./code/servo.o 

C_DEPS += \
./code/gyro.d \
./code/imgproc.d \
./code/motor.d \
./code/pit.d \
./code/servo.d 


# Each subdirectory must supply rules for building sources it contributes
code/gyro.o: C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/project/code/gyro.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\Libraries\doc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Core" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Ld" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Peripheral" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Startup" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\user\inc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_common" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_device" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\code" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
code/imgproc.o: C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/project/code/imgproc.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\Libraries\doc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Core" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Ld" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Peripheral" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Startup" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\user\inc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_common" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_device" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\code" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
code/motor.o: C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/project/code/motor.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\Libraries\doc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Core" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Ld" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Peripheral" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Startup" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\user\inc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_common" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_device" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\code" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
code/pit.o: C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/project/code/pit.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\Libraries\doc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Core" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Ld" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Peripheral" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Startup" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\user\inc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_common" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_device" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\code" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
code/servo.o: C:/Users/lid/Desktop/CHV307_Library/ch_car_ver3/project/code/servo.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\Libraries\doc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Core" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Ld" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Peripheral" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\sdk\Startup" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\user\inc" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_common" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_device" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\project\code" -I"C:\Users\lid\Desktop\CHV307_Library\ch_car_ver3\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

