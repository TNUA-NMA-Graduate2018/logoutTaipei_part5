/**
 * Chat Server 
 * by Tom Igoe. 
 * 
 * Press the mouse to stop the server.
 */


import processing.net.*;

int portA = 10001;
int portB = 10004;
boolean myServerARunning = true;
boolean myServerBRunning = true;
int bgColor = 0;
int direction = 1;
int textLine = 60;

Server myServerA;
Server myServerB;

boolean modeA = false; //false: self || true: interact
boolean modeB = false; //false: self || true: interact

void setup()
{
  size(400, 400);
  textFont(createFont("SanSerif", 16));
  myServerA = new Server(this, portA); // Starts a myServer on port 10002
  myServerB = new Server(this, portB); // Starts a myServer on port 10002
  rectMode(CENTER);
  background(0);
}

void mousePressed()
{
  if (mouseX<=150 && mouseX >= 50 && mouseY<=250 && mouseY >= 150) {
    modeA = !modeA;
  }
  if (mouseX<=350 && mouseX >= 250 && mouseY<=250 && mouseY >= 150) {
    modeB = !modeB;
  }
}
void buttonPress() {
  pushMatrix();
  translate(100, 200);

  if (modeA) {
    fill(255, 0, 0);
    rect(0, 0, 100, 100);
    fill(255);
    text("Self", -20, 0);
    text("A", 0, -75);
  } else {
    fill(0, 0, 255);
    rect(0, 0, 100, 100);
    fill(255);
    text("Inter", -20, 0);
    text("A", 0, -75);
  }

  translate(200, 0);
  if (modeB) {
    fill(255, 0, 0);
    rect(0, 0, 100, 100);
    fill(255);
    text("Self", -20, 0);
    text("B", 0, -75);
  } else {
    fill(0, 0, 255);
    rect(0, 0, 100, 100);
    fill(255);
    text("Inter", -20, 0);
    text("B", 0, -75);
  }
  popMatrix();
}

void draw()
{
  background(0);
  buttonPress();


  if (myServerARunning == true)
  {
    text("server", 15, 45);
    Client thisClient = myServerA.available();
    if (thisClient != null) {
      if (thisClient.available() > 0) {
        text("mesage from: " + thisClient.ip() + " : " + thisClient.readString(), 15, textLine);
        textLine = textLine + 35;
        if (modeA)
          thisClient.write(0);
        else
          thisClient.write(1);
      }
    }
  } 
  if (myServerBRunning == true)
  {
    text("server", 15, 45);
    Client thisClient = myServerB.available();
    if (thisClient != null) {
      if (thisClient.available() > 0) {
        text("mesage from: " + thisClient.ip() + " : " + thisClient.readString(), 15, textLine);
        textLine = textLine + 35;
        if (modeB)
          thisClient.write(0);
        else
          thisClient.write(1);
      }
    }
  }
}