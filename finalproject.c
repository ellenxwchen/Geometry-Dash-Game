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
int main () {
	volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
	int color_box[8],  x_box_for_square[20], y_box_for_square[20]; 
	short color[6] = {0xFFFF, 0xF800, 0x07E0, 0x001F, 0x000F, 0x010F};
	int triangle_x=319, triangle_y=199, trianglesize, change_in_pos=1;
	trianglesize=10;
	change_in_pos=1; 
	bool isvisible = true;
	// declare other variables(not shown)
    // initialize location and direction of rectangles(not shown)

    /* set front
	bool isvisible;pixel buffer to start of FPGA On-chip memory */
    *(pixel_ctrl_ptr + 1) = 0xC8000000; // first store the address in the 
                                        // back buffer
	
	pixel_buffer_start = *(pixel_ctrl_ptr + 1);
	clear_screen();
	wait_for_vsync();
	clear_screen();
	wait_for_vsync();
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
	draw_square_player(x_box_for_square, y_box_for_square, color_box);
	while (1) {
		clear_screen_for_triangle(triangle_x, triangle_y, trianglesize); 	/* Erase any boxes and lines that were drawn in the last iteration */
		if(isvisible) {
			draw_triangle_barrier(triangle_x, triangle_y, trianglesize, color_box);
			update_position_for_triangle(&triangle_x, &triangle_y, trianglesize, &isvisible, change_in_pos); 
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
	
	