#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

//#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(-1); //(OLED_RESET)

int b1Pin = 4, b2Pin=2; //buttons pins
bool GAMEOVER = 0;
struct paddle     //game objects
{
  int w=2;
  int h=25;
  int x=0;
  int y=SCREEN_HEIGHT/2 - h/2;
  
}playerPaddle,AIPaddle;
struct Ball
{
  int x = SCREEN_WIDTH/2,y=SCREEN_HEIGHT/2;
  int vx = 10,vy=8;
  int r = 2;  
}ball;

//functions
void displayPaddles(struct paddle p1,struct paddle p2)
{
  display.fillRect(p1.x,p1.y,p1.w,p1.h,1);
  display.fillRect(p2.x,p2.y,p2.w,p2.h,1);
}
void displayBall(struct Ball ball)
{
  display.fillCircle(ball.x,ball.y,ball.r,1);
}
void updateBall(struct Ball &ball)
{
  ball.x += ball.vx;
  ball.y += ball.vy;
  if (ball.x > SCREEN_WIDTH - ball.r || ball.x < ball.r)
    ball.vx *= -1;
  if (ball.y > SCREEN_HEIGHT - ball.r || ball.y < ball.r)
    ball.vy *= -1;
}
void updateAIPaddle(struct paddle &p,struct Ball &ball)
{
  p.y = ball.y - p.h/2;
  if(ball.x>p.x-ball.r)
    ball.vx *= -1;
}
void input(struct paddle &p,int b1,int b2)
{
  int vy = 8;
  if(digitalRead(b1)==HIGH  && p.y<SCREEN_HEIGHT-p.h)
    p.y+= vy;
  if(digitalRead(b2)==HIGH && p.y > 0)
    p.y-= vy;
}
void collision(struct Ball &ball,struct paddle leftPaddle,struct paddle rightPaddle)
{
  if( ball.x <leftPaddle.x+leftPaddle.w+ball.r)
  {
      if(ball.y < leftPaddle.y + leftPaddle.h && ball.y >leftPaddle.y)
        ball.vx *= -1;
      else GAMEOVER = 1;
  }
}

void gameover()
{
  if(GAMEOVER){
    display.clearDisplay();
    display.setCursor(10,10);
    display.setTextSize(2);
    display.setTextColor(1);
    display.println("GAMEOVER");
    display.display();
    //delay(5000); // no need for delay cuz ball can never escape from gameover position
  }  
}
//arduino main loop functions
void setup() {
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  pinMode(b1Pin,INPUT);
  pinMode(b2Pin,INPUT);

  playerPaddle.x = 16;
  AIPaddle.x = SCREEN_WIDTH - playerPaddle.x;

}

void loop() {
  display.clearDisplay();
    input(playerPaddle,b1Pin,b2Pin);
    collision(ball,playerPaddle,AIPaddle);
    updateBall(ball);
    updateAIPaddle(AIPaddle,ball);
    displayPaddles(playerPaddle,AIPaddle);
    displayBall(ball);
    gameover();
  display.display();
}
