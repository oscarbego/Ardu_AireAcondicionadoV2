
int btnOnOff  = 2;
int btnBaja   = 4;
int btnBomba  = 6;

int resOnOff  = 0;
int resBaja   = 0;
int resBomba  = 0;

boolean bandeOnOff = false;
boolean bandeBaja  = false;
boolean bandeBomba = false;


void setup() {

  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  
  pinMode(btnOnOff,  INPUT);
  pinMode(btnBaja,   INPUT);
  pinMode(btnBomba,  INPUT);
}

void loop() 
{
  resOnOff = digitalRead(btnOnOff);
  resBaja  = digitalRead(btnBaja);
  resBomba = digitalRead(btnBomba);

  // Se enciende o se apaga
  if(resOnOff == 1)
  {
    digitalWrite(13, bandeOnOff);
    bandeOnOff = !bandeOnOff;
  }

  //Cambia entre la vel de alta y baja
  if(resBaja == 1)
  {
    digitalWrite(12, bandeBaja);
    bandeBaja = !bandeBaja;
  }

  //Enciende o apaga la bomba
  if(resBomba == 1)
  {
    digitalWrite(11, bandeBomba);
    bandeBomba = !bandeBomba;
  }
       
  delay(200);
}
