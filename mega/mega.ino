void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
}

void loop()
{
   float gas = analogRead(A0);
   float flame = analogRead(A1);
   float gps = 0.00;
   
   Serial1.print(gas);
   Serial1.print(";");
   Serial1.print(flame);
   Serial1.print(";");
   Serial1.print(gps);
   Serial1.print(";");
   
   delay(1000);
  
}
