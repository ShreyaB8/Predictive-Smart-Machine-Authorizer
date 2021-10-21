//////////////////////////////////////////////
// Code to test all the sensors in BLE Sense 
// Author: Yazid Marzuk K P 
//////////////////////////////////////////////

#include <Arduino_LSM9DS1.h>
#include <Arduino_HTS221.h>
#include <PDM.h>

// buffer to read samples into, each sample is 16-bits
short sampleBuffer[256];

// number of samples read
volatile int samplesRead;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started");
  
  ////////////////////////////////////////////
  // Temperature and Humidity Sensor
  ////////////////////////////////////////////
  
  if (!HTS.begin()) {
    Serial.println("Failed to initialize humidity temperature sensor!");
    while (1);
  }
  ////////////////////////////////////////////
  // IMU 
  ////////////////////////////////////////////

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }


  ////////////////////////////////////////////
  // Accelerometer
  ////////////////////////////////////////////
  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Acceleration in G's");
  Serial.println("X\tY\tZ");
  
  
  
  ////////////////////////////////////////////
  // Gyroscope
  ////////////////////////////////////////////

  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Gyroscope in degrees/second");
  Serial.println("X\tY\tZ");
  
  ////////////////////////////////////////////
  // Gyroscope
  ////////////////////////////////////////////

  Serial.print("Magnetic field sample rate = ");
  Serial.print(IMU.magneticFieldSampleRate());
  Serial.println(" uT");
  Serial.println();
  Serial.println("Magnetic Field in uT");
  Serial.println("X\tY\tZ");

  ////////////////////////////////////////////
  // Microphone Sensor
  ////////////////////////////////////////////

  // configure the data receive callback
  PDM.onReceive(onPDMdata);

  // optionally set the gain, defaults to 20
  // PDM.setGain(30);

  // initialize PDM with:
  // - one channel (mono mode)
  // - a 16 kHz sample rate
  if (!PDM.begin(1, 16000)) {
    Serial.println("Failed to start PDM!");
    while (1);
  }
  ////////////////////////////////////////////
  // Temperature and Humidity Sensor
  ////////////////////////////////////////////
  
  if (!HTS.begin()) {
    Serial.println("Failed to initialize humidity temperature sensor!");
    while (1);
  }
}


void loop() {
  
  ////////////////////////////////////////////
  // Accelerometer
  ////////////////////////////////////////////
  float ax, ay, az;

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(ax, ay, az);
    Serial.print("Accl X :");
    Serial.print(ax);
    Serial.print('\t');
    
    Serial.print("Accl Y :");
    Serial.print(ay);
    Serial.print('\t');
    
    Serial.print("Accl Z :");
    Serial.println(az);
  }

  ////////////////////////////////////////////
  // Gyroscope
  ////////////////////////////////////////////

  float gyro_x, gyro_y, gyro_z;
  
    if (IMU.gyroscopeAvailable()) {
      IMU.readGyroscope(gyro_x, gyro_y, gyro_z);

      Serial.print("Gyro X :");
      Serial.print(gyro_x);
      Serial.print('\t');
      Serial.print("Gyro Y :");
      Serial.print(gyro_y);
      Serial.print('\t');
      Serial.print("Gyro Z :");
      Serial.println(gyro_z);
    }

  ////////////////////////////////////////////
  // Gyroscope
  ////////////////////////////////////////////

  float mx, my, mz;

  if (IMU.magneticFieldAvailable()) {
    IMU.readMagneticField(mx, my, mz);
  }
    Serial.print("Magn X :");
 
    Serial.print(mx);
    Serial.print('\t');\
    Serial.print("Magn Y :");
    Serial.print(my);
    Serial.print('\t');
    Serial.print("Magn Z :");
    Serial.println(mz);


  ////////////////////////////////////////////
  // Humidy and Temp Sensor
  ////////////////////////////////////////////  

  // read all the sensor values
  float temperature = HTS.readTemperature();
  float humidity    = HTS.readHumidity();

  // print each of the sensor values
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity    = ");
  Serial.print(humidity);
  Serial.println(" %");


  ////////////////////////////////////////////
  // Microphone Sensor
  ////////////////////////////////////////////  

 
  // wait for samples to be read
  if (samplesRead) {
    int i =0;
    // print samples to the serial monitor or plotter
    for (int i = 0; i < samplesRead; i++) {
//      delay(1000);
      Serial.println(sampleBuffer[i]);
      // check if the sound value is higher than 500
      if (sampleBuffer[i]>=500){
        digitalWrite(LEDR,LOW);
        digitalWrite(LEDG,HIGH);
        digitalWrite(LEDB,HIGH);
      }
      // check if the sound value is higher than 250 and lower than 500
      if (sampleBuffer[i]>=250 && sampleBuffer[i] < 500){
        digitalWrite(LEDB,LOW);
        digitalWrite(LEDR,HIGH);
        digitalWrite(LEDG,HIGH);
      }
      //check if the sound value is higher than 0 and lower than 250
      if (sampleBuffer[i]>=0 && sampleBuffer[i] < 250){
        digitalWrite(LEDG,LOW);
        digitalWrite(LEDR,HIGH);
        digitalWrite(LEDB,HIGH);
      }
    }
    // clear the read count
    samplesRead = 0;
  }
  
  // print an empty line
  Serial.println();

  // wait 1 second to print again
}


void onPDMdata() {
  // query the number of bytes available
  int bytesAvailable = PDM.available();

  // read into the sample buffer
  PDM.read(sampleBuffer, bytesAvailable);

  // 16-bit, 2 bytes per sample
  samplesRead = bytesAvailable / 2;
}
