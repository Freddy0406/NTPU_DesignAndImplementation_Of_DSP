#include "easyDSP-Expansion_Board.h"
#include "math.h"

#define SAMPLING_FREQ 12000
#define PI 3.14159265358979
short loop_time = 0;
#define GAIN 0.6
#define BUF_SIZE 1000
short buffer[BUF_SIZE],delayed,echo_output;
unsigned short freq = 0,sample = 0, step = 1;
unsigned short digital=15,old_digital=15;
short DTMF_count=0,DTMF_flag=0;
int i = 0,j = 0;
int button = 0;
int pointer = 0;

long count=0;

int main(void)
{
  Board_Init(); //Initial easyDSP-Expansion_Board
  Setup_Audio_Init(FS_12000_HZ,ADC_GAIN_0DB,DAC_ATTEN_24DB,LINE_INPUT);
  for(i=0 ; i<BUF_SIZE ; i++)   // clear buffer
        buffer[i] = 0;

  while(1)
  {
      if(step==1){
          Control_LED_ON(1);
          Control_LED_OFF(2);
          Control_LED_OFF(3);
      }
      else if(step==2){
          Control_LED_ON(1);
          Control_LED_ON(2);
          Control_LED_OFF(3);
      }
      else if(step==4){
          Control_LED_ON(1);
          Control_LED_ON(2);
          Control_LED_ON(3);
      }
      else{
          Control_LED_OFF(1);
          Control_LED_OFF(2);
          Control_LED_OFF(3);
      }
      if(Read_SW(1)==1){
          Control_LED_ON(4);
          LCD_PUT_CMD(LCD_SECOND_LINE);
          LCD_PUT_String("Echo ON ");
      }
      else if(Read_SW(1)==0){
          Control_LED_OFF(4);
          LCD_PUT_CMD(LCD_SECOND_LINE);
          LCD_PUT_String("Echo OFF");
      }
      digital = Read_keypad();
      switch(digital)
           {
           case 1:
            freq = 261.63*step;
            button=1;
            break;
           case 2:
            freq = 293.66*step;
            button=2;
            break;
           case 3:
            freq = 329.63*step;
            button=3;
            break;
           case 4:
            freq = 349.23*step;
            button=4;
            break;
           case 5:
            freq = 392.00*step;
            button=5;
            break;
           case 6:
            freq = 440.00*step;
            button=6;
            break;
           case 7:
            freq = 493.88*step;
            button=7;
            break;
           case 8:
            button=8;
            break;
           case 9:

            break;
           case 0:

            break;
           case 10: //-->A

            break;
           case 11: //-->B

            break;
          }
      if(DTMF_flag==1)
      {
          if(old_digital != digital)
          {
          DTMF_count = 0;
          DTMF_flag = 0;
          }
      }
      if(button==1){
          LCD_PUT_CMD(LCD_FIRST_LINE);
          if(step==1){
              LCD_PUT_String("C4");
          }
          else if(step==2){
              LCD_PUT_String("C5");
          }
          else if(step==4){
              LCD_PUT_String("C6");
          }

      }
      else if(button==2){
          LCD_PUT_CMD(LCD_FIRST_LINE);
          if(step==1){
              LCD_PUT_String("D4");
          }
          else if(step==2){
              LCD_PUT_String("D5");
          }
          else if(step==4){
              LCD_PUT_String("D6");
          }
      }
      else if(button==3){
          LCD_PUT_CMD(LCD_FIRST_LINE);
          if(step==1){
              LCD_PUT_String("E4");
          }
          else if(step==2){
              LCD_PUT_String("E5");
          }
          else if(step==4){
              LCD_PUT_String("E6");
          }
      }
      else if(button==4){
          LCD_PUT_CMD(LCD_FIRST_LINE);
          if(step==1){
              LCD_PUT_String("F4");
          }
          else if(step==2){
              LCD_PUT_String("F5");
          }
          else if(step==4){
              LCD_PUT_String("F6");
          }
      }
      else if(button==5){
          LCD_PUT_CMD(LCD_FIRST_LINE);
          if(step==1){
              LCD_PUT_String("G4");
          }
          else if(step==2){
              LCD_PUT_String("G5");
          }
          else if(step==4){
              LCD_PUT_String("G6");
          }
      }
      else if(button==6){
          LCD_PUT_CMD(LCD_FIRST_LINE);
          if(step==1){
              LCD_PUT_String("A4");
          }
          else if(step==2){
              LCD_PUT_String("A5");
          }
          else if(step==4){
              LCD_PUT_String("A6");
          }
      }
      else if(button==7){
          LCD_PUT_CMD(LCD_FIRST_LINE);
          if(step==1){
              LCD_PUT_String("B4");
          }
          else if(step==2){
              LCD_PUT_String("B5");
          }
          else if(step==4){
              LCD_PUT_String("B6");
          }
      }
  }
}
//-------------------------------
// interrupt service routine #4
//-------------------------------
interrupt void INT4_ISR(void)
{
    if(Read_SW(4)==1){
        loop_time=20000/2;
    }
    else if(Read_SW(4)==0){
        loop_time=5000/2;
    }

    if(button==8){
        unsigned short freq_arr [30] = {261.63,0,261.63,0,392.00,0,392.00,0,440.00,0,440.00,0,392.00,0,0,349.23,0,349.23,0,329.63,0,329.63,0,293.66,0,293.66,0,261.63,0};
        if(DTMF_count < loop_time)
        {
            sample=11000*(sin(2.0*PI*DTMF_count*(freq_arr[pointer]*step)/SAMPLING_FREQ));
            DTMF_count++;
        }
        else
        {
            DTMF_count=0;
            pointer++;
            if(pointer>29)pointer=0;
        }
    }
    else{
        pointer=0;
        if(DTMF_count < loop_time)
        {
            if(Read_SW(2)==1){
                sample=7000*(sin(2.0*PI*DTMF_count*freq/SAMPLING_FREQ)+sin(2.0*PI*DTMF_count*(freq/2)/SAMPLING_FREQ));
            }
            else{
                sample=10000*(sin(2.0*PI*DTMF_count*freq/SAMPLING_FREQ));
            }

            if(Read_SW(3)==1)DTMF_count+=3;
            else if (Read_SW(3)==0)DTMF_count++;
        }

        else
        {
            sample = 0;
            DTMF_flag=1;
            old_digital = digital;
            DTMF_count = loop_time;
        }
    }

    if(Read_SW(1)==1){
        delayed = buffer[i];
        echo_output = sample + delayed;
        output_right_sample(echo_output);
        buffer[i] = sample + delayed * GAIN;
        if(++i >= BUF_SIZE) i=0;
    }
    else{
        output_right_sample(sample);
    }
    return;
}

interrupt void INT5_ISR(void)
{
    step = 1;
}
interrupt void INT6_ISR(void)
{
    step = 2;
}
interrupt void INT7_ISR(void)
{
    step = 4;
}
interrupt void INT8_ISR(void)
{
    step = 0;
}



