#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <string.h>

#define MAX_BUFF 32

int push_dev;
int fnd_dev;
int text_lcd_dev;
int motor_dev;
int led_dev;
int dot_dev;

unsigned char fpga_number[10][10] = {
		{0x3e, 0x7f, 0x63, 0x73, 0x73, 0x6f, 0x67, 0x63, 0x7f, 0x3e}, // 0
		{0x0c, 0x1c, 0x1c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x1e}, // 1
		{0x7e, 0x7f, 0x03, 0x03, 0x3f, 0x7e, 0x60, 0x60, 0x7f, 0x7f}, // 2
		{0xfe, 0x7f, 0x03, 0x03, 0x7f, 0x7f, 0x03, 0x03, 0x7f, 0x7e}, // 3
		{0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7f, 0x7f, 0x06, 0x06}, // 4
		{0x7f, 0x7f, 0x60, 0x60, 0x7e, 0x7f, 0x03, 0x03, 0x7f, 0x7e}, // 5
		{0x60, 0x60, 0x60, 0x60, 0x7e, 0x7f, 0x63, 0x63, 0x7f, 0x3e}, // 6
		{0x7f, 0x7f, 0x63, 0x63, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03}, // 7
		{0x3e, 0x7f, 0x63, 0x63, 0x7f, 0x7f, 0x63, 0x63, 0x7f, 0x3e}, // 8
		{0x3e, 0x7f, 0x63, 0x63, 0x7f, 0x3f, 0x03, 0x03, 0x03, 0x03}, // 9
};
void fnd_call(int fnd_num);
void dot_call(int dot_num);
void text_call(unsigned char* str);
void led_call(void);
void motor_call(int onoff , int dir);
void start(void);
void game1(void);
void game2(void);
void open_devices(void);
void close_devices(void);
int main(void)
{

	open_devices();
	start();
	game1();
	usleep(1000000);
	game2();
//	fnd_call(1111);
//	dot_call(2);
//	led_call();
//	motor_call(0 , 0);
	usleep(1000000);
	close_devices();

	return 0;
}
void start(void)
{
	text_call("Welcome to the  3leg nakg game!");
	led_call();
}
void game1(void)
{
	text_call("  game 1 start  crossbridge game");
	printf("game 1 start! \n");
	dot_call(1);
}
void game2(void)
{
	text_call("  game 2 start  speednumber game");
	printf("game 2 start! \n");
	dot_call(2);
}
void fnd_call(int num)
{
	char fnd_string[4];
	memset(fnd_string,0,sizeof(fnd_string));
	fnd_string[0] = num / 1000;
	fnd_string[1] = (num % 1000)/100 ;
	fnd_string[2] = (num % 100)/10;
	fnd_string[3] = num % 10;
	write(fnd_dev, fnd_string , 4);
}

void dot_call(int dot_num)
{
	int dot_size;	
	dot_size = sizeof(fpga_number[dot_num]);
	write(dot_dev, fpga_number[dot_num], dot_size);
}
void text_call(unsigned char* str)
{
	unsigned char string[32] = "                                ";
	int str_size;	
	str_size = strlen(str);
	write(text_lcd_dev,string,MAX_BUFF);
	write(text_lcd_dev,str,str_size);
}
void led_call(void)
{

	char led_data = 0x00;
	write(led_dev, &led_data ,1);
	for(int i = 0; i < 10; i++)
	{
		if(i % 2 == 0)
		{
		led_data = 0x00;
		write(led_dev, &led_data ,1);
		led_data = 0x5a;
		write(led_dev,&led_data, 1);
		usleep(200000);
		}
		else
		{
		led_data = 0x00;
		write(led_dev, &led_data ,1);
		led_data = 0xa5;
		write(led_dev,&led_data, 1);
		usleep(200000);
		}

	}
	led_data = 0x00;
	write(led_dev, &led_data ,1);
}
void motor_call(int onoff , int dir)
{	
	unsigned char motor_state[3];
	memset(motor_state, 0 , sizeof(motor_state));
	if(onoff == 1)
	{
		if(dir == 1)
		{
			motor_state[0] = (unsigned char)1;
			motor_state[1] = (unsigned char)1;
			motor_state[2] = (unsigned char)255;
			write(motor_dev, motor_state, 3);
		}
		else if(dir == 0)
		{
			motor_state[0] = (unsigned char)1;
			motor_state[1] = (unsigned char)2;
			motor_state[2] = (unsigned char)255;
			write(motor_dev, motor_state, 3);
		}
		
	}
	else if(onoff == 0)
	{	
		motor_state[0] = (unsigned char)0;
		motor_state[1] = (unsigned char)0;
		motor_state[2] = (unsigned char)255;
		write(motor_dev, motor_state, 3);
	}
}
void open_devices(void)
{	
	dot_dev = open("/dev/fpga_dot", O_WRONLY);
	push_dev = open("/dev/fpga_push_switch",O_RDWR);
	fnd_dev = open("/dev/fpga_fnd",O_RDWR);
	text_lcd_dev = open("/dev/fpga_text_lcd",O_RDWR);
	motor_dev = open("/dev/fpga_step_motor",O_RDWR);
	led_dev = open("/dev/fpga_led",O_RDWR);
	if(push_dev<0)
	{
		printf("PS error\n");
		close(push_dev);
	}	
	if(fnd_dev<0)
	{
		printf("FND error\n");
		close(fnd_dev);
	}
	if(text_lcd_dev<0)
	{
		printf("Tlcd error\n");
		close(text_lcd_dev);
	}
	if(motor_dev<0)
	{
		printf("motor error\n");
		close(motor_dev);
	}
	if(dot_dev<0)
	{
		printf("Dot error \n");
		close(dot_dev);
	}
}
void close_devices(void)
{

	close(push_dev);
	close(dot_dev);
	close(fnd_dev);
	close(text_lcd_dev);
	close(motor_dev);
	close(led_dev);
}
