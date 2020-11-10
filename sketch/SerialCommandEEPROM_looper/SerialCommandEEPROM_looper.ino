// Demo Code for SerialCommand Library
// Craig Versek, Jan 2014
// based on code from Steven Cogswell, May 2011

#include <SerialCommand.h>
#include <EEPROM.h>

#define arduinoLED 5   // Arduino LED on board
  
SerialCommand sCmd(Serial);         // The demo SerialCommand object, initialize with any Stream object

  struct MyObject {
  char obj_ssid[20];
  char obj_password[20];
};

void setup() {
  pinMode(arduinoLED, OUTPUT);      // Configure the onboard LED for output
  digitalWrite(arduinoLED, LOW);    // default to LED off

  Serial.begin(9600);

  // Setup callbacks for SerialCommand commands
  sCmd.addCommand("ON",    LED_on);          // Turns LED on
  sCmd.addCommand("OFF",   LED_off);         // Turns LED off
  sCmd.addCommand("HELLO", sayHello);        // Echos the string argument back
  sCmd.addCommand("P",     processCommand);  // Converts two arguments to integers and echos them back
  sCmd.addCommand("SET_WIFI",     set_wifi);  // Converts two arguments to integers and echos them back
  sCmd.addCommand("GET_WIFI",     get_wifi);  // Converts two arguments to integers and echos them back
  sCmd.setDefaultHandler(unrecognized);      // Handler for command that isn't matched  (says "What?")
  Serial.println("Ready");
}

void loop() {
  int num_bytes = sCmd.readSerial();      // fill the buffer
  if (num_bytes > 0){
    sCmd.processCommand();  // process the command
  }
  delay(10);

  Serial.println("running processes pls wait ..");
  delay(2000);
}


void LED_on(SerialCommand this_sCmd) {
  Serial.println("--------------------");
  this_sCmd.println("LED on");
  digitalWrite(arduinoLED, HIGH);
  Serial.println("--------------------");

}

void LED_off(SerialCommand this_sCmd) {
  this_sCmd.println("LED off");
  digitalWrite(arduinoLED, LOW);
}

void sayHello(SerialCommand this_sCmd) {
  char *arg;
  arg = this_sCmd.next();    // Get the next argument from the SerialCommand object buffer
  if (arg != NULL) {    // As long as it existed, take it
    this_sCmd.print("Hello ");
    this_sCmd.println(arg);
  }
  else {
    this_sCmd.println("Hello, whoever you are");
  }
}


void processCommand(SerialCommand this_sCmd) {
  int aNumber;
  char *arg;

  this_sCmd.println("We're in processCommand");
  arg = this_sCmd.next();
  if (arg != NULL) {
    aNumber = atoi(arg);    // Converts a char string to an integer
    this_sCmd.print("First argument was: ");
    this_sCmd.println(aNumber);
  }
  else {
    this_sCmd.println("No arguments");
  }

  arg = this_sCmd.next();
  if (arg != NULL) {
    aNumber = atol(arg);
    this_sCmd.print("Second argument was: ");
    this_sCmd.println(aNumber);
  }
  else {
    this_sCmd.println("No second argument");
  }
}

void get_wifi(SerialCommand this_sCmd) {
this_sCmd.print("\n-----------");
  int eeAddress = 0; //EEPROM address to start reading from

MyObject customVar; //Variable to store custom object read from EEPROM.
  
  this_sCmd.println("stored wifi credentials:");
  EEPROM.get(eeAddress, customVar);


  this_sCmd.print("SSID: ");
    this_sCmd.println(customVar.obj_ssid);

    this_sCmd.print("password: ");
    this_sCmd.println(customVar.obj_password);

this_sCmd.print("-----------\n");
}


void set_wifi(SerialCommand this_sCmd) {
this_sCmd.print("\n-----------");
  int eeAddress = 0;   //Location we want the data to be put.
  
  char *ssid;
  char *password;

  this_sCmd.println("setting wifi credentials ...");

  ssid = this_sCmd.next();    // Get the next argument from the SerialCommand object buffer
  if (ssid != NULL) {    // As long as it existed, take it
    this_sCmd.print("SSID: ");
    this_sCmd.println(ssid);
  }
  else {
    this_sCmd.println("No SSID or passowrd given");
  }
  password = this_sCmd.next();
   if (password != NULL) {    // As long as it existed, take it
    this_sCmd.print("Password: ");
    this_sCmd.println(password);
  }
  else {
    this_sCmd.println("No password given");
  }
  if ((ssid != NULL) && (password != NULL)) {
    
    //Data to store.
  MyObject customVar;
  strcpy(customVar.obj_ssid, ssid);
  strcpy(customVar.obj_password, password);

  EEPROM.put(eeAddress, customVar);

   this_sCmd.println("Wrote wifi credentials to EEPROM.");
   
  }
  else {
    this_sCmd.println("insufficient parameters given.");
  }

    //eeAddress += sizeof(MyObject); //Move address to the next byte after float 'f'.
this_sCmd.print("-----------\n");
}

// This gets set as the default handler, and gets called when no other command matches.
void unrecognized(SerialCommand this_sCmd) {
  SerialCommand::CommandInfo command = this_sCmd.getCurrentCommand();
  this_sCmd.print("Did not recognize \"");
  this_sCmd.print(command.name);
  this_sCmd.println("\" as a command.");
}
