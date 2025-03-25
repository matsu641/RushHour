volatile int pixel_buffer_start; 
short int Buffer1[240][512];
short int Buffer2[240][512];

// VGA control address
#define PIXEL_BUFFER_CTRL  0xFF203020

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240
// define car size
#define CAR_WIDTH 42
#define CAR_HEIGHT 20

#define MAX_CARS 15  // num cars
#define BOARD_SIZE 6 //board size 
#define EMPTY_SPACE '.' // for parsing the board where no car is 
#define TILE_SIZE 22   // each tile is ~22 pixels
#define BOARD_START_X 90 // top left x pos of board (accts for border)
#define BOARD_START_Y 70 // top left x pos of board (accts for border)
#define NUM_BOARDS 50  // number of board configs pre loaded 

volatile int *pixel_ctrl_ptr = (int *)PIXEL_BUFFER_CTRL;


#define PS2_DATA       ((volatile int *)0xFF200108)   //ps2 data reg
#define PS2_CONTROL    ((volatile int *)0xFF20010C)   // ps2 control reg
#define INTERRUPT_MASK ((volatile int *)0xFF202000)   // mask reg
#define INTERRUPT_STATUS ((volatile int *)0xFF202004) // satus reg
#define LEDS           ((volatile int *)0xFF200000)   // reg for LEDs(for debug)

//reg for in riscv 
#define MIE            ((volatile int *)0x304)  
#define MSTATUS        ((volatile int *)0x300)   
#define MIP            ((volatile int *)0x344)
#define MTVEC          ((volatile int *)0x305)  

// global variables 
volatile char byte1 = 0, byte2 = 0, byte3 = 0; //for reading in from ps2
volatile int redraw_needed = 0; // for indicating when we need to redraw the screen 
char selected_car = 'A';//variable for indicating what car the user currently has selected 
volatile int mouse_x = 0, mouse_y = 0; // variables for holding the mouses x and y 


/*GRAPHICS ARRAYS*/
const int redcar[20][42] = {長いので省略};

const int board[150][150] = {長いので省略};

const int title [39][250] =  {長いので省略};

const int bluecar [42][20] =  {長いので省略};

const int greencar[20][42] =  {長いので省略};

const int tealtruck[63][20] =  {長いので省略};

const int orangetruck[20][63] =  {長いので省略};

const int purplecar[42][20] =  {長いので省略};

const int pinkcar[20][42] =  {長いので省略};


const int blackcar[42][20] =  {長いので省略};

const int browncar[20][42] =  {長いので省略};
const int navycar[42][20] =  {長いので省略};

const int magentatruck[20][63] =  {長いので省略};

const int graytruck[63][20] =  {長いので省略};

const int limecar[20][42] =  {長いので省略};

const int goldcar[42][20] =  {長いので省略};

const int forestcar[42][20] =  {長いので省略};
const int hcarhigh[22][45] =  {長いので省略};

const int vcarhigh[45][22] =  {長いので省略};
const int vtruckhigh[66][22] =  {長いので省略};

const int htruckhigh[22][66] =  {長いので省略};


//  struct for vehicles that holds sprite pos size and orientation
typedef struct {
    char id;  //identifier 
    void *sprite;   // sprite/graphic array
    int x, y;         // x and y position on board  ** -1 if not part of config
    int size;         // 2 = car 3 = truck
    char orientation; //H for horiz V for vert
} Car;

//predefined cars that can be used to make configs, initially set all to offboard
Car allCars[MAX_CARS] = {
    {'A', (void *)redcar, -1, -1, 2, 'H'}, 
    {'B', (void *)bluecar, -1, -1, 2, 'V'}, 
    {'C', (void *)greencar, -1, -1, 2, 'H'}, 
    {'D', (void *)tealtruck, -1, -1, 3, 'V'}, 
    {'E', (void *)orangetruck, -1, -1, 3, 'H'}, 
    {'F', (void *)purplecar, -1, -1, 2, 'V'},
    {'G', (void *)pinkcar, -1, -1, 2, 'H'},
    {'H', (void *)blackcar, -1, -1, 2, 'V'},
    {'I', (void *)browncar, -1, -1, 2, 'H'},
    {'J', (void *)navycar, -1, -1, 2, 'V'},
    {'K', (void *)magentatruck, -1, -1, 3, 'H'},
    {'L', (void *)graytruck, -1, -1, 3, 'V'},
    {'M', (void *)limecar, -1, -1, 2, 'H'},
    {'N', (void *)goldcar, -1, -1, 2, 'V'},
    {'O', (void *)forestcar, -1, -1, 2, 'V'}
};
 


//predefined valid board configurations, adapted from https://www.michaelfogleman.com/rush/#DatabaseFormat to match my car definitons above 
const char *board_list[NUM_BOARDS] = {長いので省略};




//function for randomly selecting and setting up the board 
void setup_board() {
    // select a random board 
   // int random_index = rand() % NUM_BOARDS;
    const char *selected_board = board_list[4];

    // first reset all vehicles to off-board -1
    for (int i = 0; i < MAX_CARS; i++) {
        allCars[i].x = -1;
        allCars[i].y = -1;
    }

    // loop for updating car positions based on the board strings we predefined
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
        char car_id = selected_board[i];

        if (car_id == 'o' || car_id == 'x') continue; // if its empty or wall(bc he used walls in his database but we arent) IGNORE

        // if its car find it in car array from id 
        for (int j = 0; j < MAX_CARS; j++) {
            if (allCars[j].id == car_id) {
                // only update position for its first occur bc we only need start position
                if (allCars[j].x == -1 && allCars[j].y == -1) { //set its x and y 
                    allCars[j].x = BOARD_START_X + (i % BOARD_SIZE) * TILE_SIZE;
                    allCars[j].y = BOARD_START_Y + (i / BOARD_SIZE) * TILE_SIZE;

                }
            }
        }
    }
}



/*functions carried over from  lab7 */
void plot_pixel(int x, int y, short int line_color)
{
    volatile short int *one_pixel_address;

        one_pixel_address = pixel_buffer_start + (y << 10) + (x << 1);

        *one_pixel_address = line_color;
}

void clear_screen(){
	//nested for loop for setting each pixel of screen to black for clearing
	for(int x = 0; x<320;x++){
		for(int y = 0; y<240;y++){
			plot_pixel(x, y, 0x0000); //set to black
		}
	}
}

//this function is to wait for the 1/60th second time period 
//referenced from prof rose lecture 22
void wait_for_vsync() {
    volatile int *pixel_ctrl_ptr = (int *)0xFF203020; // VGA controller base address
    int status;

    *pixel_ctrl_ptr = 1;  // start synchronization by writing value 1 into buffer reg

    // Wait for S bit in status register to equal 0
    status = *(pixel_ctrl_ptr + 3);
    while ((status & 0x01) != 0) {
        status = *(pixel_ctrl_ptr + 3);
    }
}


// function to draw board
void draw_board(int x, int y, const int sprite[][150], int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            plot_pixel(x + j, y + i, sprite[i][j]);
        }
    }
}
// function to draw horiz car
void draw_hcar(int x, int y, const int sprite[][42], int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            plot_pixel(x + j, y + i, sprite[i][j]);
        }
    }
}
// function to draw vert car/truck
void draw_vcar(int x, int y, const int sprite[][20], int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            plot_pixel(x + j, y + i, sprite[i][j]);
        }
    }
}

// function to draw horiz truck
void draw_htruck(int x, int y, const int sprite[][63], int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            plot_pixel(x + j, y + i, sprite[i][j]);
        }
    }
}


// function to draw  title
void draw_title(int x, int y, const int sprite[][250], int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            plot_pixel(x + j, y + i, sprite[i][j]);
        }
    }
}

// Function to check if a car can move in a given direction


//function for moving a car uses case statment for determining direction 
void move_car(char car_id, char direction) {
		int index = 0; 
	//find car in array
	for(int i= 0; i<MAX_CARS;i++){
		if(car_id == allCars[i].id){
		index =  i;
		} 
	}
	
	 int steps = 22;  // move 22 pixels per square
// determine direction to increment 
    int dx = 0, dy = 0;
	  switch (direction) {
        case 'U': dy = -1; break;
        case 'D': dy =  1; break;
        case 'L': dx = -1; break;
        case 'R': dx =  1; break;
    }
	// need to loop 22 times for smooth movement bc one square is 22 pixels
	 for (int x = 0; x < steps; x++) {
        //update by one position
        allCars[index].x += dx;
        allCars[index].y += dy;
			//redraw board and title 
            draw_board(80, 60, board, 150, 150);
            draw_title(30, 10, title, 250, 39);
			//same as in main loop
            for (int i = 0; i < MAX_CARS; i++) {
                if (allCars[i].x != -1 && allCars[i].y != -1) {
                    if (allCars[i].orientation == 'V') {
                        if (allCars[i].size == 2) {
                            draw_vcar(allCars[i].x, allCars[i].y, (const int(*)[20])allCars[i].sprite, 20, 42);
                        } else {
                            draw_vcar(allCars[i].x, allCars[i].y, (const int(*)[20])allCars[i].sprite, 20, 63);
                        }
                    } else {
                        if (allCars[i].size == 2) {
                            draw_hcar(allCars[i].x, allCars[i].y, (const int(*)[42])allCars[i].sprite, 42, 20);
                        } else {
                            draw_htruck(allCars[i].x, allCars[i].y, (const int(*)[63])allCars[i].sprite, 63, 20);
                        }
                    }
                }
            }
			
	
	
	if(allCars[index].size == 2){
		if(allCars[index].orientation == 'H'){
		for (int i = 0; i < 22; i++) {
			for (int j = 0; j < 45; j++) {
				plot_pixel(allCars[index].x + j -1, allCars[index].y + i -1, hcarhigh[i][j]);
			}
			draw_hcar(allCars[index].x, allCars[index].y, allCars[index].sprite, 42, 20);
		}
			
		}
		else {for (int i = 0; i < 45; i++) {
			for (int j = 0; j < 22; j++) {
				plot_pixel(allCars[index].x + j -1, allCars[index].y + i -1, vcarhigh[i][j]);
			}
			draw_vcar(allCars[index].x, allCars[index].y, allCars[index].sprite, 20, 42);
		}
			
		}
	}
	
	else {
		if(allCars[index].orientation == 'H'){
			for (int i = 0; i < 22; i++) {
				for (int j = 0; j < 66; j++) {
					plot_pixel(allCars[index].x + j -1, allCars[index].y + i -1, htruckhigh[i][j]);
				}
				draw_htruck(allCars[index].x, allCars[index].y, allCars[index].sprite, 63, 20);
			}
			
		}
		else {
			for (int i = 0; i < 66; i++) {
			for (int j = 0; j < 22; j++) {
				plot_pixel(allCars[index].x + j -1, allCars[index].y + i -1, vtruckhigh[i][j]);
			}
			draw_vcar(allCars[index].x, allCars[index].y, allCars[index].sprite, 20, 63);
		}
	
		
		}
	}
		
            wait_for_vsync();
            pixel_buffer_start = *(pixel_ctrl_ptr + 1);
    }
	
	

}


//function for when mouse is clicked determinig if it was on one of the vehicles in the puzzle
char find_car_at_mouse(int mx, int my) {
    for (int i = 0; i < MAX_CARS; i++) {
		// need to determine car width and height bc  and y only show the top left corner and we want to allow them to press anywhere on car 
        if (allCars[i].x != -1 && allCars[i].y != -1) {
            int car_width = (allCars[i].orientation == 'H') ? (allCars[i].size * TILE_SIZE) : TILE_SIZE;
            int car_height = (allCars[i].orientation == 'V') ? (allCars[i].size * TILE_SIZE) : TILE_SIZE;

            // figure out if a car was clicked 
            if (mx >= allCars[i].x && mx < allCars[i].x + car_width &&
                my >= allCars[i].y && my < allCars[i].y + car_height) {
                return allCars[i].id;  //if its found return its id 
            }
        }
    }
    return selected_car;  //if selection didnt correspond to a car or wasinvalid just keep the same selected car
}


// interrupt handler for ps2 device 
void PS2_ISR(void) {
	
	int PS2_data = *(PS2_DATA);	// read data 
    while (PS2_data& 0x8000) {  // while ravail>0
        byte1 = byte2; //copy over data
        byte2 = byte3;
        byte3 = PS2_data & 0xFF;
		PS2_data = *(PS2_DATA);
    }

  
	
	    // for handling a keyboard input, in this case all arrow keys start with e0
    if (byte2 == 0xE0) { 
        switch (byte3) { // the 3rd byte tells which key pressed use switch
			// for each case call the move car function with the current selected car and the direction
            case 0x75: move_car(selected_car, 'U'); break;  
            case 0x72: move_car(selected_car, 'D'); break;  
            case 0x6B: move_car(selected_car, 'L'); break;  
            case 0x74: move_car(selected_car, 'R'); break;  
        }
		// now we need a redraw 
        redraw_needed = 1;
    } 
   // for handling mouse inpt 
    else { 
	// we need to updae the x direction (since MOUSE IS RELATIVE!! NOT ABSOLUTE MVMNT
    if (byte1 & 0x10) {  
        mouse_x += (byte2 | 0xFFFFFF00);  // for neg x movemebt
    } else {
        mouse_x += byte2;
    }
	// then update y !!use - bc y inverted 
    if (byte1 & 0x20) {  
        mouse_y -= (byte3 | 0xFFFFFF00);  
    } else {
        mouse_y -= byte3;
    }

    // make sure we dont leave bounds off the screen w mouse
    if (mouse_x < 0) mouse_x = 0;
    if (mouse_y < 0) mouse_y = 0;
    if (mouse_x > SCREEN_WIDTH) mouse_x = SCREEN_WIDTH;
    if (mouse_y > SCREEN_HEIGHT) mouse_y = SCREEN_HEIGHT;
		
    // if there wasa left click then we need to attempt to update selected car 
    if (byte1 & 0x01) { 
        selected_car = find_car_at_mouse(mouse_x, mouse_y); // call function to determine what car 
        redraw_needed = 1; // now we must redraw 
    }

   
    }
	
	

    // make sure to clear the status bit now that we have handled the interrupt 
    *INTERRUPT_STATUS |= (1 << 23);
	
}

// 
void __attribute__((interrupt)) irq_handler(void) {
	//!!! using the __asm__ voltailedirectly puts in the riscv instructions 
	 __asm__ volatile ("csrc mstatus, 0x8");  //while handling the interrupt make sure we turn off others so we dont get nested 
   //determine cause of interrupt 
	int mcause;
    __asm__ volatile ("csrr %0, mcause" : "=r"(mcause));

	//if the cause was the ps2 irq call the irq for ps2
    if ((mcause & 0x800000FF) == 0x80000017) { // ps2 is irq23
       
		PS2_ISR();
    }

	 __asm__ volatile ("csrs mstatus, 0x8");  //now reenable the interrupts
}

//function for initializing the ps2/interrupts for use 
void init_ps2() {
	__asm__ volatile (
    "li sp, 0x2000000\n\t"  // set stack pointer 
    "andi sp, sp, -4\n\t"   // align it 
	);
	
    //diable interrupts while setting up 
    __asm__ volatile ("csrw mstatus, zero");  
    __asm__ volatile ("csrw mie, zero");     

    //clear any left over interrupts and ensure theyre disabled
    *INTERRUPT_STATUS = 0xFFFFFFFF;  // clear all pending interrupts
    *INTERRUPT_MASK = 0;             // disable

    // flush the fifo so no old data leftover 
    while (*PS2_DATA & 0x8000) { volatile int temp = *PS2_DATA; }

	// send reset command for ps2
    *PS2_DATA = 0xFF; 
    while (!(*PS2_DATA & 0x8000));  // wait for response
	
	 // read ithe iniial mounse response (this is from de1soc manual)
    int response1 = *PS2_DATA & 0xFF;
    while (!(*PS2_DATA & 0x8000));  
    int response2 = *PS2_DATA & 0xFF; // read second response

	    // check that the responses were correct for setup (this is from de1soc manual)
    if (response1 == 0xAA && response2 == 0x00) {
        *PS2_DATA = 0xF4;  // enable data reporting
        while (!(*PS2_DATA & 0x8000));  // wait for response
        int ack = *PS2_DATA & 0xFF;
    }
	
	
    // enable innterrupts in ps2 control reg
    *PS2_CONTROL = 0x1; 

  
    // allow interrupts for ps2 (irq23)
    *INTERRUPT_MASK = (1 << 23);  

    // set the mtcvec to go to irqhandler when interrupt happens
    int handler_address = (int)&irq_handler;
    __asm__ volatile ("csrw mtvec, %0" :: "r"(handler_address));

    // in the mie bit alllow ps2 as well as external interrupts
    __asm__ volatile (
        "li t0, (1 << 23)\n\t"  
        "csrs mie, t0\n\t"       
		"li t0, (1 << 3)\n\t  "      // enable external interrupts globally in mie
		"csrs mie, t0 \n\t  "       // now allows any external interrupt 
    );

    // now enable global interrupts 
    __asm__ volatile (
        "li t0, 0x8\n\t"  // Set MIE bit (bit 3)
        "csrs mstatus, t0\n\t"
    );

}



//main function
int main() {
	//initialize interrupts/psq
	init_ps2();
    

    // set front buffer to Buffer1
    *(pixel_ctrl_ptr + 1) = (int) &Buffer1;
    wait_for_vsync();
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen();

    // set back buffer to Buffer2
    *(pixel_ctrl_ptr + 1) = (int) &Buffer2;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1);
    clear_screen();
	
    
        clear_screen();
	
		

        setup_board(); //call the function to set up the board 
	
        // draw the titleand board 
        draw_board(80, 60, board, 150, 150);
		draw_title(30, 10, title, 250, 39);

            // loop through cars and call appropriate draw function based on its size/orientation
    for (int i = 0; i < MAX_CARS; i++) {
		//only draw id part of config 
        if (allCars[i].x != -1 && allCars[i].y != -1) {
            if (allCars[i].orientation == 'V') {
                if(allCars[i].size == 2){
                draw_vcar(allCars[i].x, allCars[i].y, (const int (*)[20])allCars[i].sprite,20,42);
                }
                else{
                draw_vcar(allCars[i].x, allCars[i].y, (const int (*)[20])allCars[i].sprite,20,63);
             }
         } 
		else {
                if (allCars[i].size == 2) {
                    draw_hcar(allCars[i].x, allCars[i].y,(const int (*)[42])allCars[i].sprite, 42, 20);
                } 
				else if (allCars[i].size == 3) {
                    draw_htruck(allCars[i].x, allCars[i].y, (const int (*)[63])allCars[i].sprite, 63, 20);
                }
            }
			
   }
		
	}
		
		// swap buffers to display initial screen 
        wait_for_vsync(); 
        pixel_buffer_start = *(pixel_ctrl_ptr + 1);
	
	//loop for during program execution 
	 while (1) {
        if (redraw_needed) { 
			//basically if we need to redraw then redraw all the parts(board,title,cars,highlight)
           // clear_screen(); // Clear screen only when needed

            draw_board(80, 60, board, 150, 150);
            draw_title(30, 10, title, 250, 39);

            for (int i = 0; i < MAX_CARS; i++) {
                if (allCars[i].x != -1 && allCars[i].y != -1) {
                    if (allCars[i].orientation == 'V') {
                        if (allCars[i].size == 2) {
                            draw_vcar(allCars[i].x, allCars[i].y, (const int(*)[20])allCars[i].sprite, 20, 42);
                        } else {
                            draw_vcar(allCars[i].x, allCars[i].y, (const int(*)[20])allCars[i].sprite, 20, 63);
                        }
                    } else {
                        if (allCars[i].size == 2) {
                            draw_hcar(allCars[i].x, allCars[i].y, (const int(*)[42])allCars[i].sprite, 42, 20);
                        } else {
                            draw_htruck(allCars[i].x, allCars[i].y, (const int(*)[63])allCars[i].sprite, 63, 20);
                        }
                    }
                }
            }
			
			//for highlighting 
			
			int index = 0; 
	//find car in array
	for(int i= 0; i<MAX_CARS;i++){
		if(selected_car == allCars[i].id){
		index =  i;
		} 
	}
	
	if(allCars[index].size == 2){
		if(allCars[index].orientation == 'H'){
		for (int i = 0; i < 22; i++) {
			for (int j = 0; j < 45; j++) {
				plot_pixel(allCars[index].x + j -1, allCars[index].y + i -1, hcarhigh[i][j]);
			}
			draw_hcar(allCars[index].x, allCars[index].y, allCars[index].sprite, 42, 20);
		}
			
		}
		else {for (int i = 0; i < 45; i++) {
			for (int j = 0; j < 22; j++) {
				plot_pixel(allCars[index].x + j -1, allCars[index].y + i -1, vcarhigh[i][j]);
			}
			draw_vcar(allCars[index].x, allCars[index].y, allCars[index].sprite, 20, 42);
		}
			
		}
	}
	
	else {
		if(allCars[index].orientation == 'H'){
			for (int i = 0; i < 22; i++) {
				for (int j = 0; j < 66; j++) {
					plot_pixel(allCars[index].x + j -1, allCars[index].y + i -1, htruckhigh[i][j]);
				}
				draw_htruck(allCars[index].x, allCars[index].y, allCars[index].sprite, 63, 20);
			}
			
		}
		else {
			for (int i = 0; i < 66; i++) {
			for (int j = 0; j < 22; j++) {
				plot_pixel(allCars[index].x + j -1, allCars[index].y + i -1, vtruckhigh[i][j]);
			}
			draw_vcar(allCars[index].x, allCars[index].y, allCars[index].sprite, 20, 63);
		}
	
		
		}
	}
		
			

            wait_for_vsync();
            pixel_buffer_start = *(pixel_ctrl_ptr + 1);

            redraw_needed = 0; // reset to avoid unnecessary redraws
        }
    }

	
    }

	