################################################################################
# MRS Version: {"version":"1.8.4","date":"2023/02/015"}
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/lid/Desktop/ch_car_ver3_4����������+��͸�ӣ�/libraries/sdk/Core/core_riscv.c 

OBJS += \
./sdk/Core/core_riscv.o 

C_DEPS += \
./sdk/Core/core_riscv.d 


# Each subdirectory must supply rules for building sources it contributes
sdk/Core/core_riscv.o: C:/Users/lid/Desktop/ch_car_ver3_4����������+��͸�ӣ�/libraries/sdk/Core/core_riscv.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\lid\Desktop\ch_car_ver3_4����������+��͸�ӣ�\Libraries\doc" -I"C:\Users\lid\Desktop\ch_car_ver3_4����������+��͸�ӣ�\libraries\sdk\Core" -I"C:\Users\lid\Desktop\ch_car_ver3_4����������+��͸�ӣ�\libraries\sdk\Ld" -I"C:\Users\lid\Desktop\ch_car_ver3_4����������+��͸�ӣ�\libraries\sdk\Peripheral" -I"C:\Users\lid\Desktop\ch_car_ver3_4����������+��͸�ӣ�\libraries\sdk\Startup" -I"C:\Users\lid\Desktop\ch_car_ver3_4����������+��͸�ӣ�\project\user\inc" -I"C:\Users\lid\Desktop\ch_car_ver3_4����������+��͸�ӣ�\libraries\zf_common" -I"C:\Users\lid\Desktop\ch_car_ver3_4����������+��͸�ӣ�\libraries\zf_device" -I"C:\Users\lid\Desktop\ch_car_ver3_4����������+��͸�ӣ�\project\code" -I"C:\Users\lid\Desktop\ch_car_ver3_4����������+��͸�ӣ�\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

