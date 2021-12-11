#include<stdio.h>
#include<stdlib.h>

int main(){
    system("sudo insmod fpga_interface_driver.ko");

    system("sudo insmod fpga_fnd_driver.ko");
    system("sudo mknod /dev/fpga_fnd c 261 0");

    system("sudo insmod fpga_dot_driver.ko");
    system("sudo mknod /dev/fpga_dot c 262 0");

    system("sudo insmod fpga_text_lcd_driver.ko");
    system("sudo mknod /dev/fpga_text_lcd c 263 0");

    system("sudo insmod fpga_push_switch_driver.ko");
    system("sudo mknod /dev/fpga_push_switch c 265 0");
    
    system("sudo insmod fpga_step_motor_driver.ko");
    system("sudo mknod /dev/fpga_step_motor c 267 0");

    system("sudo insmod fpga_led_driver.ko");
    system("sudo mknod /dev/fpga_led c 260 0");

    return 0;
}
