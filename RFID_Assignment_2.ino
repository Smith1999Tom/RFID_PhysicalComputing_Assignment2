#include <rdm630.h>

rdm630 rfid(6, 0);  //TX-pin of RDM630 connected to Arduino pin 6

//ID's are represented as integers.
//1 - 63403. 2 - 36368. 3 - 4716.

int randomArray[3];   //Random order of numbers.
int rfidArray[] {63403, 36368, 4716};   //RFID tags.
unsigned int inputOrder[3];   //Holds the order the rfid tags were scanned.
int playerArray[3];   //Player order of numbers, for comparison with randomArray.
int index = 0;    //Tracks how many tags have been scanned
int ledPin = 8;   //LED pin on arduino

/**
 * Setup function
 * Initializes the program and calls the getRandomOrder() function.
 */
void setup()
{
  Serial.begin(9600);  // start serial to PC
  randomSeed(analogRead(0));    //Generate a random seed. A0 is not used.
  rfid.begin();   //Calls the begin() function from the RDM6300 library.

  Serial.println("Setup");

  getRandomOrder();   //Assigns numbers 0-2 in a random order in the randomArray[] variable.

  for (int i = 0; i < 3; i ++)
  {
    Serial.println(randomArray[i], DEC);
  }
}

/**
 * Loop function.
 * Handles most of the game logic.
 */
void loop()
{
  
  byte data[6];   //Temporarily stores rfid tag
  byte length;

  //Checks if there is an rfid tag to read from
  if (rfid.available()) {

    Serial.println("RFID available");
    rfid.getData(data, length);   //Gets the tag from the rfid tag

    //Following code taken from https://playground.arduino.cc/Main/RDM630RFIDReaderLibrary
    //concatenate the bytes in the data array to one long which can be
    //rendered as a decimal number
    unsigned int result =
      ((unsigned long int)data[1] << 24) +
      ((unsigned long int)data[2] << 16) +
      ((unsigned long int)data[3] << 8) +
      data[4];
      //End taken code

    //Processes the read data, including if the rfid tag has already been scanned
    processInput(result);

    //Checks if all 3 tags have been scanned
    if(inputOrder[2] != 0)
    {
      convertIDToOrder();   //Determines the order blocks were scanned, and assigns it to inputArray[]
      processOutput();    //Determines score, displays output, and resets necessary arrays to 0
    }

    for(int i = 0; i < 3; i ++)
    {
      Serial.println(playerArray[i]);
    }
  }
}

/**
 * Puts numbers 0-2 in a random order.
 */
void getRandomOrder()
{
  //Since 0 is equivalent to block 1, initialize randomArray to -1
  randomArray[0] = -1;
  randomArray[1] = -1;
  randomArray[2] = -1;

  Serial.println("Randomizing");

  for (int i = 0; i < 3; i ++)
  {
    boolean newNumber = false;    //Whether the random number is new
    int randomNo;   //Random number 0-2
    while (!newNumber)    //Repeat until a new number
    {
      newNumber = true;
      randomNo = random(0, 3);    //Get random number
      for (int j = 0; j < 3; j++)
      {
        if (randomArray[j] == randomNo)   //If the random number already exists in the array
          newNumber = false;   
      }
    }
    randomArray[i] = randomNo;    //Assign random number to array
    Serial.print(randomArray[i]);

  }
Serial.println();
}

/**
 * Method to determine if the rfid tag has already been scanned
 */
void processInput(unsigned int result)
{
  boolean contains = false;
    for (int i = 0; i < 3; i ++)
    {
      
      if (result == inputOrder[i])
      {
        contains = true;
      }
    }
    if (!contains)
    {
      inputOrder[index] = result;  
      index++;
    }
}

void convertIDToOrder()
{
  for(int i = 0; i < 3; i ++)
  {
    for(int j = 0; j < 3; j ++)
    {
      if(inputOrder[i] == rfidArray[j])
      {
        playerArray[i] = j;
      }
    }
  }
}

void processOutput()
{
  int score = 0;
  for(int i = 0; i < 3; i ++)
  {
    if(randomArray[i] == playerArray[i])
    {
      score++;
    }
  }

  displayOutput(score);

  
  for(int j = 0; j < 3; j ++)
  {
    inputOrder[j] = 0;
    inputArray[j] = 0;
  }

  index = 0;
  
  delay(2000);
  
  if(score == 3)
  {
    Serial.println("A winner is you!");
  }
  
}

void displayOutput(int score)
{
  Serial.print("Correct blocks - ");
  Serial.println(score);

  for(int i = 0; i < score; i ++)
  {
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(250);
  }
  
}




