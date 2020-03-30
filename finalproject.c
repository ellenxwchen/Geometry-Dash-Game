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

#define M 199 
#define N 319
	
int background [M][N];
int speed = 4;
	
volatile int pixel_buffer_start; // global variable
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
void clear_screen();
void clear_screen_for_triangle(int triangle_x, int triangle_y, int trianglesize); 
void draw_line(int x0, int y0, int x1, int y1, short int line_colour);
void plot_pixel(int x, int y, short int line_color);
void wait_for_vsync();
void draw_square_player(int x_box[], int y_box[], int color_box[]);
void draw_square(int x, int y, short int line_color);
void draw_triangle_barrier(int triangle_x, int triangle_y, int trianglesize, int color_box[]);
void update_position_for_triangle(int* triangle_x, int* triangle_y, int trianglesize, bool *isvisible, int change_in_pos);
void draw_ground();
void initialize_background();
void rotate_left(int array[M][N]);
void draw_background();
void draw();

int main () {
	volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
	int color_box[8],  x_box_for_square[20], y_box_for_square[20]; 
	short color[6] = {0xFFFF, 0xF800, 0x07E0, 0x001F, 0x000F, 0x010F};
	int triangle_x=319, triangle_y=199, trianglesize, change_in_pos=1;
	trianglesize=10;
	//change_in_pos=4; 
	bool isvisible = true;
	// declare other variables(not shown)
    // initialize location and direction of rectangles(not shown)

    /* set front
	bool isvisible;pixel buffer to start of FPGA On-chip memory */
    *(pixel_ctrl_ptr + 1) = 0xC8000000; // first store the address in the 
                                        // back buffer
	wait_for_vsync();
	initialize_background();
	pixel_buffer_start = *(pixel_ctrl_ptr);
	clear_screen();
	/* now, swap the front/back buffers, to set the front buffer location */
   // wait_for_vsync();
    /* initialize a pointer to the pixel buffer, used by drawing functions */
   // pixel_buffer_start = *pixel_ctrl_ptr;
    //clear_screen(); // pixel_buffer_start points to the pixel buffer
    /* set back pixel buffer to start of SDRAM memory */
    *(pixel_ctrl_ptr + 1) = 0xC0000000;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer
	// initilize variables 
	for (int i=0; i<8;i++) {
			color_box[i] = color[rand()%4];
	}
	/*
	for (int i=0; i<8; i++) {
			dx_box[i] = rand()%2 * 2 - 1;
			dy_box[i] = rand()%2 * 2 - 1;
	}
	*/
	// initilize triangle 
	// placing my box at (0,179) (19, 179), (0, 199), (19,198)
	for (int i=0; i<20; i++ ) {
		x_box_for_square[i]=i;
	}
	// my x_box will be from 1 to 20
	for (int i=0; i<20; i++) {
		y_box_for_square[i]=i+179;
	}
	draw_square_player(x_box_for_square, y_box_for_square, color_box); // draw the square player 
	wait_for_vsync();
	while (1) {
		clear_screen_for_triangle(triangle_x, triangle_y, trianglesize); 	/* Erase any boxes and lines that were drawn in the last iteration */
		draw();
			draw_square_player(x_box_for_square, y_box_for_square, color_box);
		if(isvisible) {
			draw_triangle_barrier(triangle_x, triangle_y, trianglesize, color_box);
			update_position_for_triangle(&triangle_x, &triangle_y, trianglesize, &isvisible, speed); 
		}
		wait_for_vsync(); // swap front and back buffers on VGA vertical sync
		pixel_buffer_start = *(pixel_ctrl_ptr+1);
	}
}

// fill in
void draw_square_player(int x_box[], int y_box[], int color_box[]) {
	for(int i=0; i<20; i++) {
		for (int j=0; j<20; j++) {
			plot_pixel(x_box[i], y_box[j], color_box[0]);
		}
	}
	draw_square(x_box[0]+1, y_box[0]+1,color_box[1]);
	draw_square(x_box[17]+1, y_box[0]+1,color_box[1]);
	draw_square(x_box[0]+1, y_box[17]+1, color_box[1]);
	draw_square(x_box[17]+1, y_box[17]+1, color_box[1]);
	// draw bounds 3 *3
	draw_line(x_box[0], y_box[0], x_box[17], y_box[0], color_box[1]);
	draw_line(x_box[0], y_box[0], x_box[0], y_box[17], color_box[1]);
	draw_line(x_box[19], y_box[0], x_box[19], y_box[19], color_box[1]);
	draw_line(x_box[0], y_box[19], x_box[19], y_box[19], color_box[1]);
	// draw eyes 4*4
	// left eye
	draw_line(x_box[3], y_box[3], x_box[6], y_box[3], color_box[2]);
	draw_line(x_box[3], y_box[3], x_box[3], y_box[6], color_box[2]);
	draw_line(x_box[6], y_box[3], x_box[6], y_box[6], color_box[2]);
	draw_line(x_box[3], y_box[6], x_box[6], y_box[6], color_box[2]);
	// right eye
	draw_line(x_box[13], y_box[3], x_box[16], y_box[3], color_box[2]);
	draw_line(x_box[13], y_box[6], x_box[16], y_box[6], color_box[2]);
	draw_line(x_box[13], y_box[3], x_box[13], y_box[6], color_box[2]);
	draw_line(x_box[16], y_box[3], x_box[16], y_box[6], color_box[2]);
	// draw mouth
	draw_line(x_box[3], y_box[11], x_box[16], y_box[11], color_box[2]);
}

// always pass the bottom right coordinate for triangle
void draw_triangle_barrier(int triangle_x, int triangle_y, int trianglesize, int color_box[]) {
	draw_square(triangle_x-1, triangle_y-1, color_box[3]);
	draw_square(triangle_x-(trianglesize)/2, triangle_y-trianglesize, color_box[3]);
	draw_square(triangle_x-trianglesize+1, triangle_y-1, color_box[3]);
	draw_line(triangle_x-(trianglesize)/2, triangle_y-trianglesize, triangle_x-1,  triangle_y-1, color_box[3]);
	draw_line(triangle_x-trianglesize+1, triangle_y-1, triangle_x-1, triangle_y-1, color_box[3]);
	draw_line(triangle_x-trianglesize+1, triangle_y-1, triangle_x-(trianglesize)/2, triangle_y-trianglesize, color_box[3]);
}

void update_position_for_triangle(int* triangle_x, int* triangle_y, int trianglesize, bool* isvisible,  int change_in_pos) { 
	int leftcenterx = (*triangle_x) - trianglesize + 1;
	if (leftcenterx < 2) {
		(*isvisible) = false; 	
	}
	if(isvisible) {
		(*triangle_x)=(*triangle_x)-change_in_pos;
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
    int temp0 = cast_array[0];
	int temp1 = cast_array[1];
	int temp2 = cast_array[2];
	int temp3 = cast_array[3];
	int temp4 = cast_array[4];
	int temp5 = cast_array[5];
	int temp6 = cast_array[6];
	int temp7 = cast_array[7];
    for ( size_t i = speed; i < total_size -(speed-1); i=i+speed ){
        cast_array[i-speed] = cast_array[i];
		cast_array[i-speed+1] = cast_array[i+1];
		cast_array[i-speed+2] = cast_array[i+2];
		cast_array[i-speed+3] = cast_array[i+3];
		cast_array[i-speed+4] = cast_array[i+4];
		cast_array[i-speed+5] = cast_array[i+5];
		cast_array[i-speed+6] = cast_array[i+6];
		cast_array[i-speed+7] = cast_array[i+7];
	}
    cast_array[total_size-1] = temp7;
	cast_array[total_size-2] = temp6;
	cast_array[total_size-3] = temp5;
	cast_array[total_size-4] = temp4;
	cast_array[total_size-5] = temp3;
	cast_array[total_size-6] = temp2;
	cast_array[total_size-7] = temp1;
	cast_array[total_size-8] = temp0;
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

	rotate_left(background);
	
}

void draw(){
	clear_screen();
	draw_ground();
	draw_background();
}

void draw_square(int x, int y, short int line_color){
	plot_pixel(x-1,y-1,line_color);
	plot_pixel(x  ,y-1,line_color);
	plot_pixel(x+1,y-1,line_color);
	plot_pixel(x-1,y  ,line_color);
	plot_pixel(x  ,y  ,line_color);
	plot_pixel(x+1,y  ,line_color);
	plot_pixel(x-1,y+1,line_color);
	plot_pixel(x  ,y+1,line_color);
	plot_pixel(x+1,y+1,line_color);
}
void clear_screen() {
    for (int i=0; i<320; i++) {
        for (int j=0; j<240; j++ ) {
	    	plot_pixel(i,j,0);
		}
    }
}
void clear_screen_for_triangle(int triangle_x, int triangle_y, int trianglesize) {
	for (int i=triangle_x-trianglesize-5; i<triangle_x+5; i++) {
        for (int j=triangle_y-trianglesize-5; j<triangle_y+5; j++ ) {
			plot_pixel(i,j,0);
		}
    }
}
void draw_line(int x0, int y0, int x1, int y1, short int line_colour) {
	bool is_steep;
	int x;
	int b;
	is_steep = (abs(y1 -y0)) > (abs(x1-x0));
	if (is_steep){
	  int a = x0;
      x0 = y0;
      y0 = a;

      a = x1;
      x1 = y1;
      y1 = a;
	}
	if (x0 > x1){
	  b = x0;
      x0 = x1;
      x1 = b;
      b = y0;
      y0 = y1;
      y1 = b;
	}
	int deltax = x1 - x0;
	int deltay = abs(y1 - y0);
	int error = -(deltax / 2 );
	int y = y0;
	int y_step;
	if (y0 < y1){
		y_step = 1;
	}
	else{
		 y_step = -1;
	}
	for (x=x0; x<=x1; x++) {
		if(is_steep) {
			plot_pixel(y,x, line_colour); // now x and y is switched
		}
		else {
			plot_pixel(x,y, line_colour);
		}
		error = error + deltay; 
		if (error >= 0) {
			y = y + y_step;
			error = error - deltax; 
		}
	}
}

void plot_pixel(int x, int y, short int line_color)
{
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}

void wait_for_vsync() {
	volatile int * pixel_ctrl_ptr = 0xFF203020; //pixel controller
	register int status;
	*pixel_ctrl_ptr =1;
	status = *(pixel_ctrl_ptr + 3);
	while ((status & 0x01) != 0) {
		status = *(pixel_ctrl_ptr +3 );
	}
}
