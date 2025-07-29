# Duck Pond Controller - Wiring and Installation Guide

## Component List and Wiring

### ESP32 Pin Assignments
```
ESP32 Pin    | Component           | Wire Color | Notes
-------------|---------------------|------------|------------------
A0 (GPIO36)  | Turbidity Sensor    | Yellow     | Analog signal
GPIO2        | Water Level High    | Blue       | Digital input (pullup)
GPIO3        | Water Level Low     | Green      | Digital input (pullup)
GPIO4        | Manual Button       | White      | Digital input (pullup)
GPIO5        | Pump Relay Signal   | Red        | Digital output
GPIO6        | Valve Relay Signal  | Orange     | Digital output
GPIO7        | Status LED          | Purple     | Digital output
GPIO8        | Buzzer              | Brown      | Digital output
GND          | Common Ground       | Black      | All grounds
3.3V         | Sensor Power        | Red        | Low power sensors
5V           | Relay Power         | Red        | Relay modules
```

## Physical Connections

### Power Supply Requirements
- **ESP32**: 3.3V (via USB or external 5V regulator)
- **Relay Modules**: 5V DC
- **Water Pump**: 12V DC (check pump specifications)
- **Sensors**: 3.3V or 5V (check sensor specifications)

### Recommended Power Supply
- 12V 5A power adapter
- 12V to 5V converter for relay modules
- ESP32 can be powered via USB or 5V input

## Step-by-Step Wiring

### 1. ESP32 to Breadboard/PCB
```
ESP32 → Terminal Block/Breadboard
All connections should be secure and insulated
```

### 2. Sensor Connections

#### Turbidity Sensor
```
Turbidity Sensor → ESP32
VCC (Red)        → 3.3V or 5V
GND (Black)      → GND
Signal (Yellow)  → GPIO36 (A0)
```

#### Water Level Sensors (Float Switches)
```
High Level Sensor → ESP32
Common           → GND
NO (Normally Open) → GPIO2

Low Level Sensor → ESP32
Common           → GND
NO (Normally Open) → GPIO3
```

#### Manual Button
```
Push Button → ESP32
One terminal → GND
Other terminal → GPIO4
(Internal pullup resistor used)
```

### 3. Output Device Connections

#### Relay Modules
```
Relay Module 1 (Pump) → ESP32 & Power
VCC     → 5V
GND     → GND
Signal  → GPIO5

Relay Module 2 (Valve) → ESP32 & Power
VCC     → 5V
GND     → GND
Signal  → GPIO6
```

#### Status Indicators
```
LED → ESP32
Anode (+) → GPIO7
Cathode (-) → 220Ω resistor → GND

Buzzer → ESP32
Positive → GPIO8
Negative → GND
```

### 4. High Power Connections (DANGER - HIGH VOLTAGE)

#### Water Pump Connection
```
12V Power Supply → Relay 1 → Water Pump
Power Supply (+) → Relay 1 Common
Relay 1 NO → Pump (+)
Pump (-) → Power Supply (-)
```

#### Solenoid Valve Connection
```
12V Power Supply → Relay 2 → Solenoid Valve
Power Supply (+) → Relay 2 Common
Relay 2 NO → Valve (+)
Valve (-) → Power Supply (-)
```

## Physical Installation

### Electronics Enclosure
1. **Waterproof Box**: IP65 rated minimum
2. **Mounting**: Secure near pond but protected from splashing
3. **Ventilation**: Small vents with filters to prevent condensation
4. **Access**: Easy access for maintenance and programming

### Sensor Placement

#### Turbidity Sensor
- **Location**: Submerged in pond, away from water inlet/outlet
- **Depth**: 6-12 inches below surface
- **Protection**: Use sensor guard to prevent duck damage
- **Cleaning**: Accessible for monthly cleaning

#### Water Level Sensors
- **High Level**: Mount at desired maximum water level
- **Low Level**: Mount at minimum safe water level
- **Mounting**: Secure to pond wall or support post
- **Float**: Ensure free movement without obstruction

#### Manual Button
- **Location**: Weatherproof button near pond or in house
- **Mounting**: Wall mount or waterproof enclosure
- **Labeling**: Clear "CLEAN POND" label

### Pump and Valve Installation

#### Water Pump
- **Type**: Submersible pump rated for pond use
- **Placement**: At lowest point of pond for complete drainage
- **Intake**: Install intake filter to prevent clogging
- **Outlet**: Direct to waste drain or designated area

#### Solenoid Valve
- **Location**: On clean water supply line
- **Mounting**: Secure and accessible for maintenance
- **Flow Direction**: Mark inlet and outlet clearly
- **Pressure**: Ensure adequate water pressure for filling

## Safety Considerations

### Electrical Safety
- **GFCI Protection**: Use GFCI outlets for all AC power
- **Weatherproofing**: All connections must be waterproof
- **Cable Protection**: Use appropriate conduit for all wiring
- **Grounding**: Ensure proper electrical grounding

### Mechanical Safety
- **Pump Protection**: Install pump protection cage
- **Sharp Edges**: Cover all sharp edges near ducks
- **Secure Mounting**: All components securely mounted
- **Emergency Stop**: Easily accessible manual shutoff

### Animal Safety
- **Duck Protection**: No exposed wires or dangerous edges
- **Water Safety**: Ensure safe water levels during operation
- **Quiet Operation**: Use quiet pumps to avoid startling ducks
- **Regular Inspection**: Weekly safety checks

## Testing Checklist

### Before First Use
- [ ] All connections secure and insulated
- [ ] Power supply voltages correct
- [ ] Sensor readings reasonable
- [ ] Relays operate correctly
- [ ] Pump runs without issues
- [ ] Valve opens and closes
- [ ] WiFi connection established
- [ ] Web interface accessible
- [ ] Emergency stops work
- [ ] No exposed live wires

### Functional Tests
- [ ] Turbidity sensor responds to dirty water
- [ ] Water level sensors trigger at correct levels
- [ ] Manual button starts cleaning cycle
- [ ] Automatic cleaning based on turbidity
- [ ] Safety shutoffs work (max runtime, water levels)
- [ ] Web interface shows correct status
- [ ] System recovers from power loss

## Troubleshooting Common Issues

### Power Issues
- Check all power supply connections
- Verify voltage levels with multimeter
- Ensure adequate current capacity

### Sensor Issues
- Clean sensor surfaces
- Check wire connections
- Verify sensor power supply
- Use serial monitor for debugging

### Pump/Valve Issues
- Check relay operation (LED indicators)
- Verify high voltage connections
- Test manual relay operation
- Check for mechanical blockages

### Network Issues
- Verify WiFi credentials
- Check signal strength at installation location
- Use serial monitor to see IP address
- Test with direct Ethernet connection if available

## Maintenance Schedule

### Weekly
- Visual inspection of all components
- Check water levels and cleanliness
- Verify system operation

### Monthly
- Clean turbidity sensor
- Test manual controls
- Check all connections
- Review system logs

### Quarterly
- Full system calibration
- Deep clean all sensors
- Inspect waterproof seals
- Update software if needed

---

**⚠️ IMPORTANT SAFETY NOTE**: Always disconnect power before working on electrical connections. If you're not comfortable with electrical work, consult a qualified electrician for installation.
