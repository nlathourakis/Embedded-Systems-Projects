# Traffic Light with Pedestrian Button

An Arduino traffic light that stays green for cars until a pedestrian presses a button, then cycles yellow → red → green. Built **without `delay()`**: all timing uses `millis()` and a simple state machine, so the button is read continuously and no press is ever missed.

This is the first project in my Arduino learning path toward a self-balancing robot.

![Traffic light](traffic-light/traffic-light.jpg)

## Components

| Component | Qty |
|---|---|
| Arduino Uno R3 (ELEGOO) | 1 |
| LEDs: red, yellow, green | 3 |
| 220 Ω resistors | 3 |
| Push button (4-pin tactile) | 1 |
| Breadboard and jumper wires | — |

## Wiring

| Arduino pin | Connected to |
|---|---|
| 12 | 220 Ω → red LED anode; cathode → GND |
| 8 | 220 Ω → yellow LED anode; cathode → GND |
| 7 | 220 Ω → green LED anode; cathode → GND |
| 2 | Push button → GND (internal pull-up enabled) |
| GND | Breadboard ground rail |

![Wiring](images/wiring_top.jpg) <!-- TODO: add KiCad schematic -->

I used pins **without PWM** (no `~`) so the PWM pins stay free for later projects.

### Why 220 Ω?

Each LED has a forward voltage of about 2 V. The resistor drops the rest of the 5 V supply and limits the current:

$$
I = \frac{5\ \text{V} - 2\ \text{V}}{220\ \Omega} \approx 13.6\ \text{mA}
$$

This is below the LED's ~20 mA rating and the ATmega328P's recommended 20 mA per pin.

### Why no external resistor on the button?

`pinMode(BUTTON_PIN, INPUT_PULLUP)` enables the microcontroller's internal pull-up resistor (~20–50 kΩ). When the button is released, the pin is pulled to 5 V (`HIGH`); when pressed, it is connected to GND (`LOW`). Without a pull-up, the released pin would be *floating* and read random values. I confirmed this by testing with plain `INPUT`.

I also checked the button's internal connections with the multimeter's continuity mode: legs on the same side of the breadboard gap are only connected when the button is pressed.

## How it works

The light is always in one of three states:

```
 GREEN ──(button pressed)──► YELLOW ──(2 s)──► RED ──(5 s)──┐
   ▲                                                        │
   └────────────────────────────────────────────────────────┘
```

On every pass through `loop()`:

1. Compute how long we've been in the current state: `millis() - stateStart`.
2. Read the button.
3. Make **at most one** state transition (`if / else if`).
4. Set the LEDs from the current state.

Because there is no `delay()`, `loop()` runs thousands of times per second and the button is always responsive.

## Experiments

- **`delay()` breaks timing.** With two LEDs blinking at 500 ms and 1000 ms using `millis()`, adding a single `delay(2000)` made both LEDs toggle together every 2 s. Predicted sync at 4.0 s, measured **3.99 s**.
- **State timing.** Yellow phase: expected 2.00 s, measured **2.01 s**.
- **Why `else if` matters.** Without `else`, all three transitions fire in the same loop when green ends, so the light jumps straight back to green and yellow/red never appear.

## Challenges and what I learned
- I quickly learned that `delay()` messes with the circuit's timing and the `millis() - reference` structure can resolve this drawback
- What helped me with the coding was thinking like my circuit was a state machine so I was able to map the transitions clearly and execute them carefully one by one
- `INPUT_PULLUP` instead of `INPUT` for the built in resistance and executing the code I want when the pin is on `LOW` rather than `HIGH`
- How the button works as a switch and how to integrate it to the rest of my circuit 


## Code

See [`traffic-light/traffic-light.ino`](traffic-light/traffic-light.ino).
