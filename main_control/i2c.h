const uint8_t i2c_wire_device = 8;

//direction left 0 = backwards 1 = forwards
const unsigned short i2c_dir_left = 1;
//direction right
const unsigned short i2c_dir_right = 1 << 1;
//blade/sadl direction
const unsigned short i2c_dir = 1 << 2;
//left
const unsigned short i2c_left = 1 << 3;
//right
const unsigned short i2c_right = 1 << 4;
//blade
const unsigned short i2c_sadl = 1 << 5;
//saddle
const unsigned short i2c_blade = 1 << 6;
//brake
const unsigned short i2c_brake = 1 << 7;

void send_i2c_message(byte power, byte command, unsigned int repeat)
{
    for (unsigned int i = 0; i < repeat; i++)
    {
        if (i != 0)
            delay(3);
        
        Wire.beginTransmission(8); // transmit to device #8
        Wire.write(power); //sends direction to go forward
        Wire.write(command);
        Wire.endTransmission();    // stop transmitting
    }
}

byte forward()
{
    return i2c_dir_left + i2c_dir_right + i2c_left + i2c_right;
}

byte reverse()
{
   byte msg = 0;
   msg = i2c_left + i2c_right;
}
