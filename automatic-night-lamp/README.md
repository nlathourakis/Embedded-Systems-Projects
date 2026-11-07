# Automatic Night Lamp

An LED that turns on automatically when the room gets dark and gets brighter the darker it gets. A photoresistor senses the light level, the Arduino reads it through its ADC, and the LED brightness is controlled with PWM. A hysteresis band around the threshold keeps the lamp from flickering when the light level is close to the switching point.

<!-- Demo video: drag and drop the video here in the GitHub editor -->

![Breadboard](images/auto-night-lamp.jpg)

## Components

| Component | Quantity | Purpose |
|-----------|----------|---------|
| ELEGOO UNO R3 (Arduino Uno compatible) | 1 | Microcontroller |
| Photoresistor (LDR) | 1 | Light sensor |
| 10 kΩ resistor | 1 | Fixed resistor of the voltage divider |
| LED | 1 | Lamp |
| 220 Ω resistor | 1 | LED current limiting |
| Breadboard and jumper wires | – | Connections |

## Wiring

| From | To |
|------|----|
| 5V | LDR (one leg) |
| LDR (other leg) | Midpoint → A0 |
| Midpoint | 10 kΩ resistor → GND |
| Pin 9 (PWM ~) | 220 Ω resistor → LED anode (long leg) |
| LED cathode (short leg) | GND |

![Schematic](images/schematic.svg)

## How it works

### 1. Voltage divider

The Arduino cannot measure resistance, only voltage. The LDR is placed in series with a fixed 10 kΩ resistor, and the voltage at the midpoint is:

$$
V_{out} = 5\,\text{V} \cdot \frac{R}{R_{LDR} + R}
$$

When it gets darker, $R_{LDR}$ increases and $V_{out}$ drops.

The fixed resistor was chosen to be of the same order of magnitude as the LDR. A much smaller or much larger value would keep $V_{out}$ close to 0 V or 5 V regardless of the light, and the sensor would lose its sensitivity.

### 2. Measurements: calculated vs. measured

LDR resistance was first measured on its own with a multimeter. The divider voltage was then calculated and compared with a multimeter measurement and with the Arduino reading.

| Condition | $R_{LDR}$ (multimeter) | $V_{out}$ calculated | $V_{out}$ measured |
|-----------|------------------------|----------------------|--------------------|
| Room light | ≈ 11.5 kΩ | 2.33 V | 2.33 V |
| LDR covered | ≈ 80–100 kΩ | ≈ 0.50 V | ≈ 0.50 V |
| Phone flashlight | ≈ 2.5 kΩ | 4.00 V | 4.00 V |

### 3. ADC (`analogRead`)

The Uno has a 10-bit ADC that maps 0–5 V to an integer from 0 to 1023:

$$
V = \frac{\text{raw}}{1023} \cdot 5\,\text{V}
$$

Both the raw value and the voltage are printed to the Serial Monitor.

### 4. PWM (`analogWrite`)

A digital pin can only output 0 V or 5 V. PWM switches the pin on and off very fast (≈ 490 Hz on pin 9), and the LED's perceived brightness follows the average voltage:

$$
V_{avg} = D \cdot 5\,\text{V}, \qquad D = \frac{\text{value}}{255}
$$

PWM only works on the pins marked with `~` (3, 5, 6, 9, 10, 11).

### 5. Brightness control

Below the threshold $T$, the brightness increases linearly as it gets darker. It is 0 at the threshold and 255 in complete darkness:

$$
y = \frac{(T - x) \cdot 255}{T}
$$

### 6. Hysteresis

With a single threshold, sensor noise near the threshold made the LED flicker on and off. The lamp now uses two thresholds:

| Raw value | Lamp |
|-----------|------|
| $x < 380$ | ON, brightness from the formula |
| $380 \le x \le 420$ | keeps its previous state |
| $x > 420$ | OFF |

The last brightness value is stored in a global variable, so the lamp "remembers" its state inside the dead band.

## Challenges and what I learned

- The biggest challenge was figuring out the correct formula for the lamp's brightness control in order to have optimal results.
- During this project I got to experiment with the Serial Monitor and with the serial functions such as 'Serial.print()' and 'Serial.begin()' and learned the correct syntax through the Arduino's official documentation website.

## My own addition

- Through observation of the LED's behaviour when the brightness was really close to the threshold I noticed the unwanted flickering of the lamp. Quickly I realised that this is a very common problem among embedded circuits and the way to overcome this, is by implementing *hesteresis* to my circuit. This was not a big deal because with the use of a global variable I managed to keep track of the circuit's previous state and act according to the level of brightness. 