################################################################################
# MRS Version: {"version":"1.8.4","date":"2023/02/015"}
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/lid/Desktop/ch_car_ver3_4����������+��͸�ӣ�/project/user/src/isr.c \
C:/Users/lid/Desktop/ch_car_ver3_4����������+��͸�ӣ�/project/user/src/main.c 

OBJS += \
./user_c/isr.o \
./user_c/main.o 

C_DEPS += \
./user_c/isr.d \
./user_c/main.d 


# Each subdirectory must supply rules for building sources it contributes
user_c/isr.o: C:/Users/lid/Desktop/ch_car_ver3_4����������+��͸�ӣ�/project/user/src/isr.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\lid\Desktop\ch_car_ver3_4����������+��͸�ӣ�\Libraries\doc" -I"C:\Users\lid\Desktop\ch_car_ver3_4����������+��͸�ӣ�\libraries\sdk\Core" -I"C:\Users\lid\Desktop\ch_car_ver3_4����������+��͸�ӣ�\libraries\sdk\Ld" -I"C:\Users\lid\Desktop\ch_car_ver3_4����������+��͸�ӣ�\libraries\sdk\Peripheral" -I"C:\Users\lid\Desktop\ch_car_ver3_4����������+��͸�ӣ�\libraries\sdk\Startup" -I"C:\Users\lid\Desktop\ch_car_ver3_4����������+��͸�ӣ�\project\user\inc" -I"C:\Users\lid\Desktop\ch_car_ver3_4����������+��͸�ӣ�\libraries\zf_common" -I"C:\Users\lid\Desktop\ch_car_ver3_4����������+��͸�ӣ�\libraries\zf_device" -I"C:\Users\lid\Desktop\ch_car_ver3_4����������+��͸�ӣ�\project\code" -I"C:\Users\lid\Desktop\ch_car_ver3_4����������+��͸�ӣ�\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
user_c/main.o: C:/Users/lid/Desktop/ch_car_ver3_4����������+��͸�ӣ�/project/user/src/main.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\lid\Desktop\ch_car_ver3_4����������+��͸�ӣ�\Libraries\doc" -I"C:\Users\lid\Desktop\ch_car_ver3_4����������+��͸�ӣ�\libraries\sdk\Core" -I"C:\Users\lid\Desktop\ch_car_ver3_4����������+��͸�ӣ�\libraries\sdk\Ld" -I"C:\Users\lid\Desktop\ch_car_ver3_4����������+��͸�ӣ�\libraries\sdk\Peripheral" -I"C:\Users\lid\Desktop\ch_car_ver3_4����������+��͸�ӣ�\libraries\sdk\Startup" -I"C:\Users\lid\Desktop\ch_car_ver3_4����������+��͸�ӣ�\project\user\inc" -I"C:\Users\lid\Desktop\ch_car_ver3_4����������+��͸�ӣ�\libraries\zf_common" -I"C:\Users\lid\Desktop\ch_car_ver3_4����������+��͸�ӣ�\libraries\zf_device" -I"C:\Users\lid\Desktop\ch_car_ver3_4����������+��͸�ӣ�\project\code" -I"C:\Users\lid\Desktop\ch_car_ver3_4����������+��͸�ӣ�\libraries\zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

