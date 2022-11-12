struct Position {
    union  {
            double ra, az, lha;
    };
    union {
        double dec, alt;
    };
};


void setup(){
    Serial.begin(9600);
    // Serial.println("Hello World");
    Position loc = {1.0, 2.0};
    // Frame loc::base = {0, 0};
    // Serial.println(loc);

    Serial.println(loc.ra);
    Serial.println(loc.dec);
    Serial.println(loc.az);
}
void loop() {

}