/* ========================================
PID controller for a quadcopter

The goal is to get two motors to maintain a centered position while constrained like a "See-Saw." 
Both motors balance each other when the difference between Compare values is 13500.
    This difference is maintained throughout control.

Receives 8 bit (signed) input from an accelerometer

Output is to ESC's 
    The output to the ESC's is in the form of a PWM's duty cycle.
    The ESC labeled 33 starts at a compare value of about 18500 and maxes out at around 19220
    The ESC labeled 59 starts at a compare value of about 19850 and maxes out at around 20500
    ESC output pins are pin 0[0] and 0[1]
 * ========================================
*/
#include <project.h>
#include "stdio.h"
#include <stdlib.h>

#define TransmitBufferSize 16
#define CR (0x0Du)

#define TRANSMIT_BUFFER_SIZE  256

/*Define Varibles for I2C*/
uint8 Address[1];
uint8 i2cWritebuf[2];
uint8 i2cReadbuf[3];

uint8 count = 0 ;
uint8 States;

/* Variable to store UART received character */
    uint8 Ch;
/* Transmit Buffer */
    char TransmitBuffer[TRANSMIT_BUFFER_SIZE];
    
    
int I2C_SetAcc()
{
    CyGlobalIntEnable;
   
    /* Place your initialization/startup code here */
    UART_1_Start();
    I2C_1_Start();    
    I2C_1_EnableInt();
       
    CyDelay(50);
    
    UART_1_PutString("Port is Open \r\n");    

    /* Initialization Varibles*/
    Address[0]= 0x1D;           // Address of the Chip 
    i2cWritebuf[0]= 0x16;       // Address of the Mode Countrol Regester
    // Configures the Device for 2g measurent Range and Put the Device in Measurement Mode
    i2cWritebuf[1]= 0x25;        
   
    /* Write to a Speacefic Regester*/
    States =  I2C_1_MasterWriteBuf(Address[0], (uint8 *) i2cWritebuf,2, I2C_1_MODE_COMPLETE_XFER);
    
    /* Format State of the WriteBuf Function result for transmition */
    sprintf(TransmitBuffer, "State: %d ", States);
    /* Send out the data */
    UART_1_PutString(TransmitBuffer);
    
    /* Format the Data Sent in the WriteBuf[0] for transmition */
    sprintf(TransmitBuffer, ",  Reading from Regester: %X ", i2cWritebuf[0]);
    /* Send out the data */
   UART_1_PutString(TransmitBuffer);
    
    /* Format the Data Sent in the WriteBuf[0] for transmition */
    sprintf(TransmitBuffer, " , Value = %X \r\n", i2cWritebuf[1]);
    /* Send out the data */
    UART_1_PutString(TransmitBuffer);
       
    
    I2C_1_MasterClearStatus(); /* Clear any previous status */    
    
    /* Reading from a Spefice Regester  */
    States = I2C_1_MasterWriteBuf(Address[0], (uint8 *) i2cWritebuf, 1, I2C_1_MODE_NO_STOP);
    
    /*Format State of the WriteBuf Function result for transmition */
    sprintf(TransmitBuffer, "State: %d \r\n", States);
    /* Send out the data */
    UART_1_PutString(TransmitBuffer);
            
    I2C_1_MasterClearStatus(); /* Clear any previous status */    
    
    States = I2C_1_MasterReadBuf(Address[0], (uint8 *) i2cReadbuf, 1, I2C_1_MODE_REPEAT_START);
    
    /* Format State of the ReadBuf Function result for transmition */
    sprintf(TransmitBuffer, "State: %d ", States);
    /* Send out the data */
    UART_1_PutString(TransmitBuffer);
    
    /* Format the Data Sent in the WriteBuf[0] for transmition */
    sprintf(TransmitBuffer, ",  Reading from Regester: %X ", i2cWritebuf[0]);
    /* Send out the data */
   UART_1_PutString(TransmitBuffer);
    
    /* Format the Data Read in the ReadBuf[0] for transmition */
    sprintf(TransmitBuffer, " , Value = %X \r\n", i2cReadbuf[0]);
    /* Send out the data */
    UART_1_PutString(TransmitBuffer);
    
    /*Check to See if the Transmition is Done*/
    while((I2C_1_MasterStatus() & I2C_1_MSTAT_WR_CMPLT)) 
    {
        States = I2C_1_MasterStatus();
        count++;
    /* Transfer complete. Check Master status to make sure that transfer
    completed without errors. */
        /* Format Readbuf Status for transmition */
    sprintf(TransmitBuffer, "Count: %d %d \r\n", States, count);
    /* Send out the data */
    UART_1_PutString(TransmitBuffer);
    
    I2C_1_MasterClearStatus(); /* Clear any previous status */    
    }
    return 0;
}

int I2C_AccRead()
{
    
     CyGlobalIntEnable;
    
     /* Place your initialization/startup code here */
    UART_1_Start();
    I2C_1_Start();    
    I2C_1_EnableInt();    
   
     /* Initialization Varibles*/
    Address[0]= 0x1D;   // Address of the Chip 
    /*Regester to Read from */
    i2cWritebuf[0]= 0x06;
    
    /* Reading from a Spefice Regester Set in i2cWritebus[0]  */
    States = I2C_1_MasterWriteBuf(Address[0], (uint8 *) i2cWritebuf, 1, I2C_1_MODE_NO_STOP);
    
    /*Format State of the WriteBuf Function result for transmition */
    sprintf(TransmitBuffer, "State: %d \r\n", States);
    /* Send out the data */
    UART_1_PutString(TransmitBuffer);
            
    States = I2C_1_MasterReadBuf(Address[0], (uint8 *) i2cReadbuf, 3, I2C_1_MODE_REPEAT_START);
    
    /* Format State of the WriteBuf Function result for transmition */
    sprintf(TransmitBuffer, "State: %d ", States);
    /* Send out the data */
    UART_1_PutString(TransmitBuffer);
    
    /* Format the Data Sent in the WriteBuf[0] for transmition */
    sprintf(TransmitBuffer, ",  Reading from Regester: %X ", i2cWritebuf[0]);
    /* Send out the data */
   UART_1_PutString(TransmitBuffer);
    
    /* Format the Data Read in Readbuf result for transmition */
    sprintf(TransmitBuffer, " , Value = X: %d Y: %d Z: %d \r\n", i2cReadbuf[0],i2cReadbuf[1],i2cReadbuf[2]);
    /* Send out the data */
    UART_1_PutString(TransmitBuffer);
    
    /*Check to See if the Transmition is Done*/
    while((I2C_1_MasterStatus() & I2C_1_MSTAT_WR_CMPLT)) 
    {
        States = I2C_1_MasterStatus();
        count++;
    /* Transfer complete. Check Master status to make sure that transfer
    completed without errors. */
        /* Format Readbuf result for transmition */
        sprintf(TransmitBuffer, "Count: %d %d \r\n", States, count);
        /* Send out the data */
        UART_1_PutString(TransmitBuffer);
    
        I2C_1_MasterClearStatus(); /* Clear any previous status */    
    }
        
    return i2cReadbuf[1] ;
   
}
int main()
{
     CyGlobalIntEnable;
    I2C_SetAcc(); // Initiating and setting Acc 
    
    
    //Flags
    char Begin = 0;
    char Stop = 0;
    char i = 0;
    char in = 0;
    char8 store[4];
    
    //Variable to store UART received character
    char Ch;
    
    //Buffers
    char TransmitBuffer[TransmitBufferSize];
    uint8 ReceiveBuffer[TransmitBufferSize];
    
    //Variables to store values from accelerometer
    //int8 x_pos = 0;
    int16 y_pos;
    //int8 z_pos;
    
    //Variables to store error values
    //int8 e_x;//must be int not uint to account for negatives
    int8 e_y;
    //int8 e_z;
    
    //Variables to store previous error values
    //int8 e_xLast = 0;//must be int not uint to account for negatives
    int8 e_yLast = 0;
    //int8 e_zLast = 0;
    
    //Variables to store the difference between currrent and previous error values
    //int8 e_xDiff = 0;//must be int not uint to account for negatives
    int8 e_yDiff = 0;
    //int8 e_zDiff = 0;
    
    //Variables to store running sum values for the integral part of the PID
   // int16 e_xInt = 0;//must be int not uint to account for negatives
    int e_yInt = 0;
    //int8 e_zInt = 0;
    
    //Variables to store SET POIINT values
   // int8 x_set = 0;//Might have to change these later//must be int not uint to account for negatives
    int8 y_set = 225;
    //int8 z_set = 0;
    
    //Variables to store the result of PID calculation 
    //int8 Result_x = 0;//must be int not uint to account for negatives
    int32 Result_y = 0;
    //int8 Result_z = 0;
    
    //Set initial P, I, and D  (TUNABLE PARAMETERS)
    float Kp = 25;//8 is totally unstable when by itself.
    float Kd = 15;//6 makes it totally unstable when kp is 1
    float Ki = 0.02;
    
    //Variables for high time of pwm.
    uint16 Compare1; 
    uint16 Compare2;
    
    // Variables to Stop and Start Balanceing.
    uint8 balance =0;
    uint8 Acc = 0;
    
    ///////////////////////////////////////////////////START MODULES/////////////////////////////////////////////////////////////////////
    //Start PWM module
    PWM_1_Start();
    //Start I2C module
    I2C_1_Start();
    //Start UART for taking commands
    UART_1_Start();

    /* CyGlobalIntEnable; */ /* Uncomment this line to enable global interrupts. */
    
    //Get user input
    UART_1_PutString("---Quadcopter controller---\r\n");
    UART_1_PutString("Enter a to arm");
    while(Ch){
        Ch = UART_1_GetChar();//Doesn't block AKA doesn't sit and wait for a character to be received.   
    }
    
    ////////////////////////////////////////////////////MAIN LOOP//////////////////////////////////////////////////////////////////////////
    for(;;)
    {
        
        Ch = UART_1_GetChar();
        
        
        //Set flags
        switch(Ch)//Set flags
        {
            //Set here then decrement by about 100 to arm then increment back up to turn on.
            
            case 'a'://Sets initial values of compare (with a difference of 1350)
                Compare1 = 15650 - 200;//Initial compare values for PWM modules
                Compare2 = 13000 - 200;
                sprintf(TransmitBuffer, "Compare 1 ---> %d", Compare1);//Pin closest to Psoc Starts at 12100
                UART_1_PutString("\r\n");
                UART_1_PutString(TransmitBuffer);
                sprintf(TransmitBuffer, "Compare 2 ---> %d", Compare2);//Starts at 13700
                UART_1_PutString("\r\n");
                UART_1_PutString(TransmitBuffer);
                break;
            case '1'://Increment by 100ee
                //Begin = 1;
                Compare1 = Compare1 + 100;
                Compare2 = Compare2 + 100;
                //enter arming duty cycle
                sprintf(TransmitBuffer, "Compare 1 ---> %d", Compare1);
                UART_1_PutString("\r\n");
                UART_1_PutString(TransmitBuffer);
                sprintf(TransmitBuffer, "Compare 2 ---> %d", Compare2);
                UART_1_PutString("\r\n");
                UART_1_PutString(TransmitBuffer);
                break;
            
            case 'i'://Increment by 1
                //Stop = 1;
                Compare1++;
               // Compare2--;
                sprintf(TransmitBuffer, "Compare 1 ---> %d", Compare1);
                UART_1_PutString("\r\n");
                UART_1_PutString(TransmitBuffer);
                sprintf(TransmitBuffer, "Compare 2 ---> %d", Compare2);
                UART_1_PutString("\r\n");
                UART_1_PutString(TransmitBuffer);               
                break;
            
            case 'o'://Decrement by 1
               // Compare1--;
                Compare2++;
                sprintf(TransmitBuffer, "Compare 1 ---> %d", Compare1);
                UART_1_PutString("\r\n");
                UART_1_PutString(TransmitBuffer);
                sprintf(TransmitBuffer, "Compare 2 ---> %d", Compare2);
                UART_1_PutString("\r\n");
                UART_1_PutString(TransmitBuffer);               
                break;
            
            case 'e'://Decrement by 100
                Compare1 = Compare1 - 100;
                Compare2 = Compare2 - 100;
                sprintf(TransmitBuffer, "Compare 1 ---> %d", Compare1);
                UART_1_PutString("\r\n");
                UART_1_PutString(TransmitBuffer);
                sprintf(TransmitBuffer, "Compare 2 ---> %d", Compare2);
                UART_1_PutString("\r\n");
                UART_1_PutString(TransmitBuffer);               
                break;    
                           
            case 'b': //  Bigen Balanceing 
                balance = 1;
                break;
           
            case 's': // Stop Balanceing                
                balance = 0;
                break;       
                    
            case 'm': //  Bigen Balanceing 
                Acc = 1;
                break;
           
            case 'n': // Stop Balanceing                
                Acc = 0;
                break;       
            
        }
        
        if(Acc ==1)
        {
            I2C_AccRead();
        }
        if (balance == 1)
        {
            //////////////////////////////READ ACCELEROMETER////////////////////////////////////////////////////////////////////////////////
            //(For 2g input, our range should be 200 to 255). Gravity = 1g (max possible felt for our setup). 
            //x_pos = Ch; //Accepts 0 to 255
            y_pos =  I2C_AccRead(); // Geting the Y-axis Value
            //z_pos = 
            //////////////////////////////Y saturates at 255///////////////////////////////////
            if ((y_pos >= 0) && (y_pos <= 30))//if the y reading goes over 255, set ypos = 255 + the amount that it went over
            {
                y_pos = y_pos + 255 ;
            }
            /////////////////////////////////////////////////////////////////PID////////////////////////////////////////////////////////////
            //Errors
            //e_x = x_set - x_pos; //Range: x_set - x_pos(max and min) = -23 to 32 if x_set = 0
            e_y = y_set - y_pos;
            //e_z = z_set - z_pos;
            
            //Derivative
            //e_xDiff = e_xLast - e_x;//Range: Could be as much as +-255 (max last error or current error can be is +-127)
           // e_xLast = e_x;
            e_yDiff = e_yLast - e_y;
            e_yLast = e_y;
            //e_zDiff = e_zLast - e_z;
            //e_zLast = e_z;
            
            //Integral
           // if(e_yInt<200 && e_yInt>-200)//Limits the affect e_xInt has (Saturates)
            //{
                //e_xInt = e_xInt + e_x;//Range: +-200 (depends on parameters of if statement)
                e_yInt = e_yInt + e_y;
                //e_zInt = e_zInt + e_z;
           // }
           
            //PID 
            //Result_x = Kp*e_x + Ki*e_xInt + Kd*e_xDiff; //Range: +-518 (+-255 + +-63 + +-200= +-518)
          Result_y = Kp*e_y + Ki*e_yInt + Kd*e_yDiff;
            //Result_z = Kp*e_z + Ki*e_zInt + Kd*e_zDiff;
           
            //PD 
            //Result_x = Kp*e_x  + Kd*e_xDiff; //Range: +-318 (+-255 + +-63 = +-318)
           //Result_y = Kp*e_y + Kd*e_yDiff;
            //Result_z = Kp*e_z  + Kd*e_zDiff;
            
            ///////////////////////APPLY RESULT TO COMPARE VALUES////////////////////////////////////////////////////////////////
                if(y_pos < y_set)//If leaning away from accelerometer
                {
                    Compare1 = 15650 +2000 + Result_y;//Pin closest to psoc
                    Compare2 = 13000 +2000 - Result_y;
                }
                if(y_pos > y_set)//If leaning toward accelerometer
                {
                    Compare1 = 15650 + 2000 - Result_y;//Pin closest to psoc
                    Compare2 = 13000 + 2000 + Result_y;
                    
                }
            if (Compare1 <= 15200)//Lower saturation of compare1//Pin closest to psoc
            {Compare1 = 15200;}
            if (Compare1 >= 20500)//Upper saturation of compare1
            {Compare1 = 20500;}
            
            if (Compare2 <= 13000)//Lower saturation of compare2
            {Compare2 = 13000;}
            if (Compare2 >= 20500)//Upper saturation of compare2
            {Compare2 = 20500;} 
        }

 //Accepts 16 bit value (Should be offset by whatever the start value is for the motor)
        PWM_1_WriteCompare1(Compare1);//Accepts 16 bit value (Should be offset by whatever the start value is for the motor)
        PWM_1_WriteCompare2(Compare2);
    }

}
