#include <rdm630.h>

rdm630 rfid(6, 0);  //TX-pin of RDM630 connected to Arduino pin 6

//ID's are represented as integers.
//1 - 63403. 2 - 36368. 3 - 4716.

int randomArray[3];   //Random order of numbers.
int rfidArray[] {63403, 36368, 4716};   //RFID tags.
unsigned int inputOrder[3];   //Holds the order the rfid tags were scanned.
int playerArray[3];   //Player order of numbers, for comparison with randomArray.
int index = 0;    //Tracks how many tags have been scanned
int ledPin = 8;


void setup()
{
  Serial.begin(9600);  // start serial to PC
  randomSeed(analogRead(0));
  rfid.begin();

  Serial.println("Setup");

  getRandomOrder();

  for (int i = 0; i < 3; i ++)
  {
    Serial.println(randomArray[i], DEC);
  }


}

void loop()
{
  
  byte data[6];
  byte length;

  if (rfid.available()) {
    
Serial.println(index);
    Serial.println("RFID available");
    rfid.getData(data, length);
    //concatenate the bytes in the data array to one long which can be
    //rendered as a decimal number
    unsigned int result =
      ((unsigned long int)data[1] << 24) +
      ((unsigned long int)data[2] << 16) +
      ((unsigned long int)data[3] << 8) +
      data[4];

    Serial.println(result);
    processInput(result);

    if(inputOrder[2] != 0)
    {
      convertIDToOrder();
      processOutput();
    }

    

    for(int i = 0; i < 3; i ++)
    {
      Serial.println(playerArray[i]);
    }

    //Serial.print("decimal CardID: ");
    //Serial.println(result);
  }
  //delay(100);
}

void getRandomOrder()
{
  randomArray[0] = -1;
  randomArray[1] = -1;
  randomArray[2] = -1;

  for (int i = 0; i < 3; i ++)
  {
    boolean newNumber = false;
    int randomNo;
    while (!newNumber)
    {
      newNumber = true;
      randomNo = random(0, 3);
      for (int j = 0; j < 3; j++)
      {
        if (randomArray[j] == randomNo)
          newNumber = false;
      }
    }
    randomArray[i] = randomNo;

  }

}

void processInput(unsigned int result)
{
  boolean contains = false;
    for (int i = 0; i < 3; i ++)
    {
      Serial.println(inputOrder[i]);
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

  if(score != 3)
  {
    for(int j = 0; j < 3; j ++)
    {
      inputOrder[j] = 0;
    }
    delay(2000);
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




