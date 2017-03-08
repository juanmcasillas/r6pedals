// ///////////////////////////////////////////////////////////////////////////
//
// R6Pedals
// Juan M. Casillas juanm.casillas@gmail.com 7/March/2017
// simple sketch to map Q and E keys in Rainbow six to a physical buttons,
// to act like a pedals, in order to lean in Rainbow Six Siege.
//
// based on 
// https://github.com/cuddleburrito/gamething-controller/blob/master/gamething/gamething.ino
//
// Differences: 
// 1) Timeout are changed to lower values
// 2) Only 2 pins (inputs) used
// 3) removed LED handling
// 4) Using BOUNCE_LOCK_OUT policy (less latency)
//
// ///////////////////////////////////////////////////////////////////////////

#include <Keyboard.h>
#include <Bounce2.h>

#define BOUNCE_LOCK_OUT 1
#define BOUNCE_WAIT 1
#define BOUNCE_COUNT 1
#define NBUTTONS 2

int buttonPins[] = {
  9, //Q button 1 (left)
  8  //E button 2 (right)
};

char buttonPresets[] = { 
  'q', //button 1
  'e', //button 2
  };

boolean buttonPressed[NBUTTONS]; // Instantiate a Bounce object array to store each debouncer in
Bounce debouncers[NBUTTONS];     //Instantiate a counter array for each button to debounce count timer
int debounceCount[NBUTTONS];

void setup() {
  Keyboard.begin();
  
  for (int i = 0; i < NBUTTONS; i++) {
     debouncers[i] = Bounce();
     debounceCount[i] = 0;
     pinMode(buttonPins[i],INPUT_PULLUP);
     (debouncers[i]).attach(buttonPins[i]);
     (debouncers[i]).interval(BOUNCE_WAIT);
     delay(100);
     buttonPressed[i] = false;
   }
}

void loop() {
  for (int j = 0; j < NBUTTONS; j++) { // iterate over each button (pin)
    
     (debouncers[j]).update();         //check current value
     int value = (debouncers[j]).read();
     
     if ( value == LOW ) { // if button pressed
       
       
       // The button has been held down long enough and it hasn't been previously registered as pressed
       if(debounceCount[j] == BOUNCE_COUNT && buttonPressed[j] == false) { 
          Keyboard.press(char(buttonPresets[j])); //Keyboard.write('1');
          buttonPressed[j] = true;
        } else {
            if(debounceCount[j] < BOUNCE_COUNT) { 
              debounceCount[j] = debounceCount[j] + 1; 
            }
        }
        
      } else { //button is not pressed
        
        if(debounceCount[j] > 0) {
          debounceCount[j] = debounceCount[j] - 1; // keep decreasing count unless 0
        } else {
           Keyboard.release(char(buttonPresets[j])); // if 0 then release button
           buttonPressed[j] = false;
        }
      }
  }
}

