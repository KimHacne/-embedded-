#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <time.h>

#define MAX_BUTTON 9

unsigned char quit = 0;

void putRan(int* n1, int* n2, int* n3);
int bridge(void);
void user_signal1(int sig);

int main(void)
{
        if(bridge()<0)
                printf("bridge err\n");

        return 0;
}

void user_signal1(int sig)
{
        quit = 1;
}

int bridge(void){
        int dev;
        int buf_s;
        unsigned char push_sw_buff[MAX_BUTTON];
        unsigned char push_buf[3][3];

        dev = open("/dev/fpga_push_switch", O_RDWR);
        if (dev<0){
                printf("Device Open Error\n");
                close(dev);
                return -1;
        }
        (void)signal(SIGINT, user_signal1);

        buf_s=sizeof(push_buf);


        printf("Press <ctrl+c> to quit. \n");
        int cnt = 1;
        printf("====Game Start : Push Switch====\n");
        for(int k=2;k>=0;k--){
                int n1, n2, n3;
                putRan(&n1, &n2, &n3);
                printf("answer : %d\n", n1);
                while(!quit){
                        usleep(400000);
                        read(dev, &push_buf, buf_s);

                        if(push_buf[k][n1]==1){
                                printf("stage %d : Success\n", cnt);
                                cnt++;
                                break;
                        }
                        else if((push_buf[k][n2] ==1) || (push_buf[k][n3] == 1)){
                                printf("you die\n");
                                return 0;
                        }
                }
        }

        close(dev);
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

