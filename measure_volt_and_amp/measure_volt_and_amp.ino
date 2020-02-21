const int sensor_IN = A2;
//use 185 for 5A, 100 for 20A Module and 66 for 30A Module
int mv_per_amp = 185;
double voltage = 0;
double v_rms = 0; 
double amps_rms = 0;

void setup(){
     Serial.begin(9600);
}

void loop(){
    voltage = getVPP();
    v_rms = (voltage) * 0.707;
    amps_rms = (v_rms * 1000)/mv_per_amp;
    amps_rms=amps_rms-0.3;
    amps_rms=amps_rms*1.414;
    Serial.print(amps_rms);
    Serial.println(" Amps RMS");
}

float getVPP(){
    float result;
    int read_value;             //value read from the sensor
    int max_value = 0;          // store max value here
    int min_value = 1024;       // store min value here

    uint32_t start_time = millis();
    //sample for 1 Sec
    while((millis()-start_time) < 1000) {
        read_value = analogRead(sensor_IN);
        //Serial.print(read_value);
        // see if you have a new max_value
        if (read_value > max_value){
            /*record the maximum sensor value*/
            max_value = read_value;
        }
        if (read_value < min_value){
            /*record the maximum sensor value*/
            min_value = read_value;
        }
    }
    Serial.print("MAX:");
    Serial.println(max_value);
    Serial.print("MIN:");
    Serial.println(min_value);
    // Subtract min from max
    result = (((max_value - min_value) * 0.5)*5)/1024.0;
    return result;
 }


