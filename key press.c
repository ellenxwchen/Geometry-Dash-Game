#define KEY_EDGECAPTURE ((volatile long *) 0xFF20005C)
#define LEDs ((volatile long *) 0xFF200000)

int main()
{ 
	int pressed = 0; //for turning on/off the LED
	long release;
	
	*KEY_EDGECAPTURE = 0xF; //clear any changes
	while (1)
	{
		// Display the state of the change register on red LEDs
		release = *KEY_EDGECAPTURE;
		
		if (release){
			*LEDs = 0x1;
			if (pressed == 1)
				pressed = 0;
			else if (pressed == 0)
				pressed = 1;
			if (pressed == 0){
				*LEDs = 0x0000;
			}
			*KEY_EDGECAPTURE = 0xF; //reset the changes for next round
		}
	}
}