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
void update_position_for_triangle(int* triangle_x, int* triangle_y, int trianglesize, bool *isvisible[], int change_in_pos);
void draw_ground();
void initialize_background();
void rotate_left(int array[M][N]);
void draw_background();
void draw();
void draw_obstacle_single_block(int obstacle_box_x, int colour);
void update_position_single_block(int *obstacle_box_x, int speed, bool *isvisible[]);
void draw_obstacle_spikes(int spike_x, int colour);
void update_position_spikes(int *spike_x, int speed, bool *isvisible[]);
void draw_obstacle_hanging(int hanging_x, int colour);
void update_position_hanging(int *hanging_x, int speed, bool *isvisible[]);
void draw_obstacle_hanging(int hanging_x, int colour);
void update_position_hanging(int *hanging_x, int speed, bool *isvisible[]);

int main () {
	volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
	int color_box[8],  x_box_for_square[20], y_box_for_square[20]; 
	int obstacle_box_x = 319 - 20;
	int spike_x = 319 - 20;
	int hanging_x = 319 - 20;
	short color[6] = {0xFFFF, 0xF800, 0x07E0, 0x001F, 0x000F, 0x010F};
	int triangle_x=319, triangle_y=199, trianglesize, change_in_pos=1;
	trianglesize=10;
	//change_in_pos=4; 
	//bool isvisible = true;
	bool isvisible [10];
	for (int i = 1; i <10; i++){
		isvisible[i] = false;
	}
	isvisible[0] = true;
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

    /* initialize a pointer to the pixel buffer, used by drawing functions */
    // pixel_buffer_start points to the pixel buffer
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
		if(isvisible[0]) {
			draw_triangle_barrier(triangle_x, triangle_y, trianglesize, color_box);
			update_position_for_triangle(&triangle_x, &triangle_y, trianglesize, &isvisible, speed); 
			if (triangle_x <=319-30) isvisible[1] = true;
		}
		if(isvisible[1]) {
			draw_obstacle_single_block(obstacle_box_x, 0xFFFF);
			update_position_single_block(&obstacle_box_x, speed, &isvisible);
			if (obstacle_box_x <=319 - 60) isvisible[2] = true;
		}
		if (isvisible[2]){
			draw_obstacle_spikes(spike_x, 0xFFFF);
			update_position_spikes(&spike_x, speed, &isvisible);
			if (spike_x <=319 - 60) isvisible[3] = true;
		}
		if (isvisible[3]){
			draw_obstacle_hanging(hanging_x, 0xFFFF);
			update_position_hanging(&hanging_x, speed, &isvisible);
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

void draw_obstacle_hanging(int hanging_x, int colour){
	//hanging_x will be the x coordinate of the hanging block, not the wire or the triangle
	for (int x = hanging_x + 8; x <= hanging_x + 12; x++){
		draw_line (x,0,x,199-60,colour);
	}
	for (int x = hanging_x;x < hanging_x + 20; x++){
		draw_line (x,199-60,x,199-40,colour);
	}
	for (int x = hanging_x; x <= hanging_x + 10; x++){
		draw_line (x,199-40,hanging_x+10,199-25,colour);
	}
	for (int x = hanging_x+10; x < hanging_x + 20; x++){
		draw_line (hanging_x + 10,199-25,x,199-40,colour);
	}
}

void update_position_hanging(int *hanging_x, int speed, bool *isvisible[]){
	*hanging_x= *hanging_x - speed;
	if (hanging_x < 0) *isvisible[3] = false;
}

void update_position_for_triangle(int* triangle_x, int* triangle_y, int trianglesize, bool* isvisible[],  int change_in_pos) { 
	int leftcenterx = (*triangle_x) - trianglesize + 1;
	if (leftcenterx < 2) {
		(*isvisible[0]) = false; 	
	}
	if(isvisible[0]) {
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

void update_position_single_block(int *obstacle_box_x, int speed, bool *isvisible[]){
	*obstacle_box_x= *obstacle_box_x - speed;
	if (obstacle_box_x < 0) *isvisible[1] = false;
}

void draw_obstacle_single_block(int obstacle_box_x, int colour){
	//if (obstacle_box_x <=319 - 20){
		for (int x = obstacle_box_x; x < 20 + obstacle_box_x; x++){
			for (int y = 199-20; y<199;y++){
				plot_pixel (x,y,colour);
			}
		}
	//}
}

void draw_obstacle_spikes(int spike_x, int colour){
	
	for (int x = spike_x; x <spike_x + 5; x ++){
		draw_line (spike_x,199-12,x,199,colour);
	}
	for (int x = spike_x+5; x <spike_x + 10; x ++){
		draw_line (spike_x + 5,199-12,x,199,colour);
	}
	for (int x = spike_x+10; x <spike_x + 15; x ++){
		draw_line (spike_x+10,199-12,x,199,colour);
	}
	for (int x = spike_x+15; x <spike_x + 20; x ++){
		draw_line (spike_x+15,199-12,x,199,colour);
	}
}

void update_position_spikes(int *spike_x, int speed, bool *isvisible[]){
	*spike_x= *spike_x - speed;
	if (spike_x < 0) *isvisible[2] = false;
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
			else{
				plot_pixel(x,y,0x0275);
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
