#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <malloc.h>
#include <signal.h>

#define MAX_BUFF 32

//game1 에서 사용됨
#define MAX_BUTTON 9
unsigned char quit = 0;

//game2 에서 사용됨
int equal = 0;
int start = 0;
char answer[30];


//fnd cntl
int push_dev;
int fnd_dev;
int text_lcd_dev;
int motor_dev;
int led_dev;
int dot_dev;

//fnd cntl
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
void fnd_call(char *);
void dot_call(int dot_num);
void text_call(unsigned char* str);
void led_call(void);
void motor_call(int onoff , int dir);
void start(void);
void open_devices(void);
void close_devices(void);

//game1 함수
void game1(void);
void user_signal1(int sig);
int bridge(void);
void putRan(int* n1, int* n2, int* n3);

//game2 함수
void game2(void);
void *GetAnswer(); //get answer from answer.txt 스레드 함수


int main(void)
{

	open_devices();
	start();
    sleep(1);
    game1();
	usleep(1000000);
	game2();

//	fnd_call(1111);
//	dot_call(2);
//	led_call();
    usleep(1000000);
	motor_call(0 , 0);

	close_devices();

	return 0;
}
//game 시작
void start(void)
{
	text_call("Welcome to the  3leg nakg game!");
	led_call();
}
void game1()
{
	text_call("  game 1 start  crossbridge game");
	printf("game 1 start! \n");
	dot_call(1);

    if(bridge()<0)
        printf("bridge err\n");

    text_call("Game 1 clear");
    printf("Game 1 clear\n");

}
void game2(void)
{
	text_call("  game 2 start  speednumber game");
	printf("game 2 start! \n");
	dot_call(2);

    pthread_t thread;
	int t;
    char *zero ="0000";
	system("python3 makeRandom.py");
    
	t = pthread_create(&thread,NULL,GetAnswer,NULL);

    fnd_call(answer);
    usleep(300000);
    fnd_call(zero);

    while(1){
        if(start == 1){
            if(equal == 1){
                printf("\n======================\n");
                printf("game 2 clear\n");
                printf("======================\n");
                text_call("You are Winner");
                led_call();
                motor_call(1,1);

            }else{
                printf("game 2 failed\n");
                text_call("     you die        game over   ");
                exit(0);
            }
        }

    }

}

//fnd cntl
void fnd_call(char *ans)
{
	// char fnd_string[4];
	// memset(fnd_string,0,sizeof(fnd_string));
	// fnd_string[0] = num / 1000;
	// fnd_string[1] = (num % 1000)/100 ;
	// fnd_string[2] = (num % 100)/10;
	// fnd_string[3] = num % 10;
	write(fnd_dev, ans , 4);
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

//game1
void user_signal1(int sig)
{
        quit = 1;
}
int bridge(void){
        //int dev;
        int buf_s;
        unsigned char push_sw_buff[MAX_BUTTON];
        unsigned char push_buf[3][3];

        //dev = open("/dev/fpga_push_switch", O_RDWR);
        // if (dev<0){
        //         printf("Device Open Error\n");
        //         close(dev);
        //         return -1;
        // }
        (void)signal(SIGINT, user_signal1);

        buf_s=sizeof(push_buf);


        //printf("Press <ctrl+c> to quit. \n");
        int cnt = 1;
        printf("====Game Start : Push Switch====\n");
        for(int k=2;k>=0;k--){
                int n1, n2, n3;
                putRan(&n1, &n2, &n3);
                //printf("answer : %d\n", n1);
                while(!quit){
                        usleep(400000);
                        read(push_dev, &push_buf, buf_s);

                        if(push_buf[k][n1]==1){
                                printf("stage %d : Success\n", cnt);
                                cnt++;
                                break;
                        }
                        else if((push_buf[k][n2] ==1) || (push_buf[k][n3] == 1)){
                                printf("you die\n");
                                text_call("you die");
                                printf("game 1 failed\n");
                                return 0;
                        }
                }
        }

        //close(dev);
        return 0;
}
void putRan(int* n1, int* n2, int* n3){

        srand((unsigned int)time(NULL));
        *n1 = rand() % 3;

        if(*n1==0){
                *n2 = 1;
                *n3 = 2;
        } else if(*n1==1) {
                *n2 = 0;
                *n3 = 2;
        } else if(*n1==2) {
                *n2 = 0;
                *n3 = 1;
        }

}

//game2 thread
void *GetAnswer(){

	FILE *f;
	char *tmp[3]; //start result equal
	
	int tid=pthread_self();

	while(1){
		if((f=fopen("answer.txt","r"))){
			printf("game.c -> start reading\n");
			char *str;
			int i = 0;
			printf("file reading\n");
			
			for ( i; i<3;i++){
				str = (char*)malloc(sizeof(char)*100);
				fgets(str,100,f);
				tmp[i] = str;
			}
			fclose(f);
			
			printf("file close\n");
			//printf("%s,%s,%s",tmp[0],tmp[1],tmp[2]);
			
			if(!strcmp(tmp[0],"true\n")){
				start = 1;
			}
			
			if(!strcmp(tmp[2],"true\n")){
				equal = 1;
			}
			strcpy(answer,tmp[1]);

			printf("game.c -> finish reading\n");
			printf("start = %d\n",start);
			printf("equal = %d\n",equal);
			printf("answer = %s\n",answer);
			break;
		}else{
			continue;
		}				
	}
	pthread_exit(NULL);
}
