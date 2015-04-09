/**
 * \file MP3Shield_Library_Demo.ino
 *
 * \brief Example sketch of using the MP3Shield Arduino driver, demonstrating all methods and functions.
 * \remarks comments are implemented with Doxygen Markdown format
 *
 * \author Bill Porter
 * \author Michael P. Flaga
 *
 * This sketch listens for commands from a serial terminal (like the Serial
 * Monitor in the Arduino IDE). If it sees 1-9 it will try to play an MP3 file
 * named track00x.mp3 where x is a number from 1 to 9. For eaxmple, pressing
 * 2 will play 'track002.mp3'. A lowe case 's' will stop playing the mp3.
 * 'f' will play an MP3 by calling it by it's filename as opposed to a track
 * number.
 *
 * Sketch assumes you have MP3 files with filenames like "track001.mp3",
 * "track002.mp3", etc on an SD card loaded into the shield.
 */

#include <SPI.h>

//Add the SdFat Libraries
#include <SdFat.h>
#include <SdFatUtil.h>

//and the MP3 Shield Library
#include <SFEMP3Shield.h>

// Below is not needed if interrupt driven. Safe to remove if not using.
#if defined(USE_MP3_REFILL_MEANS) && USE_MP3_REFILL_MEANS == USE_MP3_Timer1
  #include <TimerOne.h>
#elif defined(USE_MP3_REFILL_MEANS) && USE_MP3_REFILL_MEANS == USE_MP3_SimpleTimer
  #include <SimpleTimer.h>
#endif

#define CONN Serial1
#define N_FILES 20
#define NO_PLAY_FRAC 0.5
#define STOP_PLAY_FRAC 0.5

#define LIGHT_PIN A5
#define TEMP_PIN A4
#define REF 2.56

/**
 * \brief Object instancing the SdFat library.
 *
 * principal object for handling all SdCard functions.
 */
SdFat sd;

/**
 * \brief Object instancing the SFEMP3Shield library.
 *
 * principal object for handling all the attributes, members and functions for the library.
 */
SFEMP3Shield MP3player;

//------------------------------------------------------------------------------
/**
 * \brief Setup the Arduino Chip's feature for our use.
 *
 * After Arduino's kernel has booted initialize basic features for this
 * application, such as Serial port and MP3player objects with .begin.
 * Along with displaying the Help Menu.
 *
 * \note returned Error codes are typically passed up from MP3player.
 * Whicn in turns creates and initializes the SdCard objects.
 *
 * \see
 * \ref Error_Codes
 */
void setup() {

  uint8_t result; //result code from some function as to be tested at later time.

  CONN.begin(9600);
  delay(1000);

  analogReference(INTERNAL);

  //Initialize the SdCard.
  if(!sd.begin(SD_SEL, SPI_FULL_SPEED)) sd.initErrorHalt();
  // depending upon your SdCard environment, SPI_HAVE_SPEED may work better.
  if(!sd.chdir("/")) sd.errorHalt("sd.chdir");

  //Initialize the MP3 Player Shield
  result = MP3player.begin();
  //check result, see readme for error codes.

  MP3player.setVolume(20, 20); // Lower is louder
}

//------------------------------------------------------------------------------
/**
 * \brief Main Loop the Arduino Chip
 *
 * This is called at the end of Arduino kernel's main loop before recycling.
 * And is where the user's serial input of bytes are read and analyzed by
 * parsed_menu.
 *
 * Additionally, if the means of refilling is not interrupt based then the
 * MP3player object is serviced with the availaible function.
 *
 * \note Actual examples of the libraries public functions are implemented in
 * the parse_menu() function.
 */
void loop() {
  
  // Stop playing track?
  if( MP3player.getState() == playback ){
    int stop_track = random(100);
    if( stop_track > 100 * STOP_PLAY_FRAC ){
      MP3player.stopTrack();
      //CONN.println("STOPPING");
    }
  }
  
  if( MP3player.getState() != playback ){
    // Pick a random number between 1 and N_FILES + NO_PLAY_FRAC
    int track_no = random(1,N_FILES + N_FILES*NO_PLAY_FRAC + 1);
    //CONN.println(track_no);
    
    if( track_no <= N_FILES ){
      // Do we need to wake up
      if( MP3player.getState() == deactivated ){
        MP3player.begin();
      //CONN.println("WAKING UP");
        delay(100);
      }
      MP3player.playTrack(track_no);
      MP3player.setPlaySpeed(random(1,4));
      //CONN.println("PLAYING");
    } else if ( MP3player.getState() != deactivated ){
      // Shut it down
      MP3player.end();
      //CONN.println("SHUTTING DOWN");
    }
  }
  
  long time = millis();
  while( millis() - time < 5000 ){
    float temp;
    int light;
    
    temp = (REF*analogRead(TEMP_PIN)/1024.0);
    temp = (1.8663 - temp) / 0.01169;
    
    CONN.print("TEMP ");
    CONN.println(temp);
    
    delay(10);
    
    light = analogRead(LIGHT_PIN);
    
    CONN.print("LIGHT ");
    CONN.println(light);
    
    delay(1000); 
  }
}
