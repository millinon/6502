const char ADDR_PINS[] = { 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52 };
const int ADDR_BITS = 16;

const char DATA_PINS[] = { 31, 33, 35, 37, 39, 41, 43, 45 };
const int DATA_BITS = 8;

const int OE_PIN = 3;

const int WE_PIN = 2;

static void addr_mode(int mode)
{
    for(int i = 0; i < DATA_BITS; i++){
        pinMode(DATA_PINS[i], mode);
    }
}

static void data_mode(int mode)
{
    for(int i = 0; i < ADDR_BITS; i++){
        pinMode(ADDR_PINS[i], mode);
    }
}

const int delay_microseconds = 100;

static void reset_control_pins(void)
{
    digitalWrite(WE_PIN, HIGH);
    digitalWrite(OE_PIN, HIGH);

    delayMicroseconds(delay_microseconds);
}

static void write_byte(uint16_t addr, uint8_t data)
{
    for(int i = 0; i < ADDR_BITS; i++){
        digitalWrite(ADDR_PINS[i], (addr & (1 << i)) ? HIGH : LOW);
    }

    delayMicroseconds(delay_microseconds);

    for(int i = 0; i < DATA_BITS; i++){
        digitalWrite(DATA_PINS[i], (data & (1 << i)) ? HIGH : LOW);    
    }

    digitalWrite(WE_PIN, LOW);

    delayMicroseconds(delay_microseconds);

    reset_control_pins();
}

static uint8_t read_byte(uint16_t addr)
{
    for(int i = 0; i < ADDR_BITS; i++){
        digitalWrite(ADDR_PINS[i], (addr & (1 << i)) ? HIGH : LOW);
    }

    digitalWrite(OE_PIN, LOW);

    delayMicroseconds(delay_microseconds);

    uint8_t data = 0;

    for(int i = 0; i < DATA_BITS; i++){
        if(digitalRead(DATA_PINS[i])){
            data |= ((uint8_t) (1 << i));
        }
    }

    digitalWrite(OE_PIN, HIGH);

    delayMicroseconds(delay_microseconds);

    reset_control_pins();

    return data;
}

static uint16_t addr = 0x0000;
static uint8_t data = 0;
static bool write_phase = true;
static bool test_fail = false;
static bool test_done = false;

const uint16_t max_addr = 0x3fff;

void setup(){
    pinMode(OE_PIN, OUTPUT);
    pinMode(WE_PIN, OUTPUT);

    reset_control_pins();

    data_mode(OUTPUT);
    addr_mode(OUTPUT);

    Serial.begin(115200);
    Serial.println("Writing test pattern...");
}

static char serial_buf[256];

static uint8_t pattern(uint16_t addr)
{
    return (addr % 2 == 0) ? ((addr & 0xFF00) >> 8) : (addr & 0x00FF);
}

void loop(){
    if(! test_done){
        if(write_phase){
            if(addr <= max_addr){
                data = pattern(addr);

                write_byte(addr, data);
                addr++;
            } else {
                for(int i = 0; i < DATA_BITS; i++){
                    digitalWrite(DATA_PINS[i], LOW);    
                }
                data_mode(INPUT);

                Serial.println("Reading back pattern...");
                write_phase = false;
                addr = 0;
            }
        } else {
            if(addr <= max_addr){
                data = read_byte(addr);

                if(data != pattern(addr)){
                    sprintf(serial_buf, "FAIL: 0x%04hx := 0x%02hx", addr, data);
                    Serial.println(serial_buf);

                    test_fail = true;
                }

                addr++;
            } else {
                if(test_fail){
                    Serial.println("OVERALL FAIL");
                } else {
                    Serial.println("OVERALL PASS");
                }

                test_done = true;
            }
        }
    }
}
