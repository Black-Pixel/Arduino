#include <Wire.h>
#include <HMC5883L.h>

HMC5883L compass;
MagnetometerScaled scaled;
float strength;
float strengthInGauss;
int spkr;
float threshold = 4.0; // The threshold for the speaker
                       // Set to 4.0 according to the default magnetic field strength in my test setup (~3.5)
                       // May not be the best choice

void setup() {
  spkr = 10; // Speaker pin
  pinMode(spkr, OUTPUT); 
  
  Wire.begin();
  
  Serial.begin(9600);  // For PC output
  
  compass = HMC5883L(); 
  
  int error = compass.SetScale(8.1); // Set the scale of the compass. +/- 8.1 Gauss
  if(error != 0) // If there is an error, print it out.
    Serial.println(compass.GetErrorText(error));

  error = compass.SetMeasurementMode(Measurement_Continuous); // Set the measurement mode to Continuous
  if(error != 0) // If there is an error, print it out.
    Serial.println(compass.GetErrorText(error));
}

float getStrength() {    // Get absolute strength using the euclidean distance function
   return sqrt(scaled.XAxis * scaled.XAxis +
               scaled.YAxis * scaled.YAxis +
               scaled.ZAxis * scaled.ZAxis);
}

void loop() {

  scaled = compass.ReadScaledAxis();   // Retrieve the scaled values from the compass (scaled to the configured scale).
   
  strength = getStrength(); // Get the highest value of the three (x,y,z)
  strengthInGauss = strength/230; // Convert to Gauss
                    
  Serial.print("Magnetic Field Strength: "); 
  Serial.print(strengthInGauss);
  Serial.println(" Gauss");

  if (strengthInGauss > threshold) { // The default magnetic field strength when using a scale of 8.1 is between 1.0 and 1.4 here where I live
    analogWrite(spkr, 150); // Speaker on (has to be a value between 0 and 255)
  }
  else {
    analogWrite(spkr, 0); // Speaker off
  } 
  
  delay(1000);
}
