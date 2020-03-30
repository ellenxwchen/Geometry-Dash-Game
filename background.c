/*----------------------------------------------------------------------------------------------------------*/
//address_map_arm.h
/*----------------------------------------------------------------------------------------------------------*/


/* This files provides address values that exist in the system */

#define BOARD                 "DE1-SoC"

/* Memory */
#define DDR_BASE              0x00000000
#define DDR_END               0x3FFFFFFF
#define A9_ONCHIP_BASE        0xFFFF0000
#define A9_ONCHIP_END         0xFFFFFFFF
#define SDRAM_BASE            0xC0000000
#define SDRAM_END             0xC3FFFFFF
#define FPGA_ONCHIP_BASE      0xC8000000
#define FPGA_ONCHIP_END       0xC803FFFF
#define FPGA_CHAR_BASE        0xC9000000
#define FPGA_CHAR_END         0xC9001FFF

/* Cyclone V FPGA devices */
#define LEDR_BASE             0xFF200000
#define HEX3_HEX0_BASE        0xFF200020
#define HEX5_HEX4_BASE        0xFF200030
#define SW_BASE               0xFF200040
#define KEY_BASE              0xFF200050
#define JP1_BASE              0xFF200060
#define JP2_BASE              0xFF200070
#define PS2_BASE              0xFF200100
#define PS2_DUAL_BASE         0xFF200108
#define JTAG_UART_BASE        0xFF201000
#define JTAG_UART_2_BASE      0xFF201008
#define IrDA_BASE             0xFF201020
#define TIMER_BASE            0xFF202000
#define AV_CONFIG_BASE        0xFF203000
#define PIXEL_BUF_CTRL_BASE   0xFF203020
#define CHAR_BUF_CTRL_BASE    0xFF203030
#define AUDIO_BASE            0xFF203040
#define VIDEO_IN_BASE         0xFF203060
#define ADC_BASE              0xFF204000

/* Cyclone V HPS devices */
#define HPS_GPIO1_BASE        0xFF709000
#define HPS_TIMER0_BASE       0xFFC08000
#define HPS_TIMER1_BASE       0xFFC09000
#define HPS_TIMER2_BASE       0xFFD00000
#define HPS_TIMER3_BASE       0xFFD01000
#define FPGA_BRIDGE           0xFFD0501C

/* ARM A9 MPCORE devices */
#define   PERIPH_BASE         0xFFFEC000    // base address of peripheral devices
#define   MPCORE_PRIV_TIMER   0xFFFEC600    // PERIPH_BASE + 0x0600

/* Interrupt controller (GIC) CPU interface(s) */
#define MPCORE_GIC_CPUIF      0xFFFEC100    // PERIPH_BASE + 0x100
#define ICCICR                0x00          // offset to CPU interface control reg
#define ICCPMR                0x04          // offset to interrupt priority mask reg
#define ICCIAR                0x0C          // offset to interrupt acknowledge reg
#define ICCEOIR               0x10          // offset to end of interrupt reg
/* Interrupt controller (GIC) distributor interface(s) */
#define MPCORE_GIC_DIST       0xFFFED000    // PERIPH_BASE + 0x1000
#define ICDDCR                0x00          // offset to distributor control reg
#define ICDISER               0x100         // offset to interrupt set-enable regs
#define ICDICER               0x180         // offset to interrupt clear-enable regs
#define ICDIPTR               0x800         // offset to interrupt processor targets regs
#define ICDICFR               0xC00         // offset to interrupt configuration regs

/*----------------------------------------------------------------------------------------------------------*/
//part3.c 
/*----------------------------------------------------------------------------------------------------------*/

#include <stdbool.h>
#include <stdlib.h>

#define M 199 
#define N 319
int background [M][N];
	
volatile int pixel_buffer_start; // global variable

void wait_for_vsync(){
	volatile int *pixel_ctrl_ptr = 0xFF203020;
	register int status;
	
	*pixel_ctrl_ptr = 1; 						//Request to synchronize with VGA timing
	
	status = *(pixel_ctrl_ptr + 3);
	while ((status & 0x01)!=0){
		status = *(pixel_ctrl_ptr + 3);			//Wait for Status to be 0 (finish drawing screen)
	}
}

//Plot a pixel at that spot
void plot_pixel(int x, int y, short int line_color)
{
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}

void swap (int *x, int *y){
	int temp = *x;
	*x = *y;
	*y = temp;
}

//Completely clear screen 
void clear_screen(){					//Clear screen to be black
	for(int y = 0; y < 199; y++){
		for(int x = 0; x < 320;x++){
			plot_pixel(x,y,0x0275); //highlight 0x0335
		}
	}
}

//Draw ground with a white line for clarity
void draw_ground(){        				//Ground starts at (199,239)
	for (int x = 0; x <= 319; x ++){
		for (int y = 199; y <= 239; y ++){
			plot_pixel(x,y,0x0076);
			plot_pixel(x,199,0xFFFF);
		}
	}
}

//Initialize the background to the original picture
//1 pattern duplicated on both sides of the screen
void initialize_background(){
	for (int x =0; x<319; x++){
		for (int y = 0; y <199; y++){
			background[y][x] = 0;
		}
	}
	//frame of first half
	for(int x = 0; x <=159; x++){
		for (int y = 0; y<=5;y++){
			background[y][x] = 1;
			background[y][x+160] = 1;
		}
	}
	for(int x = 0; x <=5; x++){
		for (int y = 0; y<199;y++){
			background[y][x] = 1;
			background[y][x+160] = 1;
		}
	}
	for(int x = 159-5; x <=159; x++){
		for (int y = 0; y<199;y++){
			background[y][x] = 1;
			background[y][x+160] = 1;
		}
	}
	for(int x = 0; x <=159; x++){
		for (int y = 199-5; y<199;y++){
			background[y][x] = 1;
			background[y][x+160] = 1;
		}
	}
	
	//small two right squares
	for(int x = 100; x <=105; x++){
		for (int y = 0; y<101;y++){
			background[y][x] = 1;
			background[y][x+160] = 1;
		}
	}
	for(int x = 100; x <=159; x++){
		for (int y = 50; y<56;y++){
			background[y][x] = 1;
			background[y][x+160] = 1;
		}
	}
	
	//Middle Lines
	for(int x = 0; x <=159; x++){
		for (int y = 100; y<106;y++){
			background[y][x] = 1;
			background[y][x+160] = 1;
		}
	}
	
	//Right big rectangle
	for(int x = 60; x <=65; x++){
		for (int y = 100; y<199;y++){
			background[y][x] = 1;
			background[y][x+160] = 1;
		}
	}
	for(int x = 60; x <=159; x++){
		for (int y = 149; y<149+5;y++){
			background[y][x] = 1;
			background[y][x+160] = 1;
		}
	}
	
	for(int x = 110; x <=115; x++){
		for (int y = 149; y<199;y++){
			background[y][x] = 1;
			background[y][x+160] = 1;
		}
	}
}

//Rotate left by 1
void rotate_left(int array[M][N])
{
	int total_size = M*N;
    int *cast_array = (int *)array;
    int temp = cast_array[0];

    for ( size_t i = 1; i < total_size; ++i )
        cast_array[i-1] = cast_array[i];

    cast_array[total_size-1] = temp;

}

void draw_background(){
	//Drawing the whole background
	for (int x = 0; x<=319;x++){
		for (int y = 0; y<=198; y++){
			if (background[y][x] == 1){
				plot_pixel(x,y,0x443E);
			}
		}
	}
	
	//Pretty choppy, only erase what's necessary
	//Rotating the background left to move 
	int speed = 4;
	for (int i = 0; i <speed; i++){
		rotate_left(background);
	}
	
}

void draw(){
	clear_screen();
	draw_ground();
	draw_background();
}

int main(void)
{
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    /* set front pixel buffer to start of FPGA On-chip memory */
    *(pixel_ctrl_ptr + 1) = 0xC8000000; // first store the address in the 
                                        // back buffer
    /* now, swap the front/back buffers, to set the front buffer location */
    wait_for_vsync();
	initialize_background();
    /* initialize a pointer to the pixel buffer, used by drawing functions */
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen(); // pixel_buffer_start points to the pixel buffer
    /* set back pixel buffer to start of SDRAM memory */
    *(pixel_ctrl_ptr + 1) = 0xC0000000;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer

    while (1)
    {
        /* Erase any boxes and lines that were drawn in the last iteration */
        //...
		draw();
        wait_for_vsync(); // swap front and back buffers on VGA vertical sync
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
    }
}