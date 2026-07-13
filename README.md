# Rancilio Silvia Controller

ESPHome-based smart controller for Rancilio Silvia with PID temperature control, automated brew profiles, backflush workflow, and Home Assistant dashboard.

[Русская версия](README.ru.md)

[Telegram discussion group](https://t.me/Rancilio_Silvia) (Russian and English)

Digital controller for the Rancilio Silvia espresso machine, built on ESP32-S3, implemented with ESPHome, and integrated with Home Assistant.

The project is more than an external PID retrofit. The goal is to move the machine control logic to a digital controller: high-voltage loads are switched by relays/SSR, while the original front-panel controls become low-voltage GPIO inputs. This makes the machine controllable from Home Assistant and leaves room for a touchscreen or another digital interface later.

> [!WARNING]
> The espresso machine contains hazardous mains voltage and a hot pressurized boiler. ESPHome is not a replacement for the original thermostat, thermal fuse, protective earth, or any other hardware safety device. Never work on the machine while it is connected to mains power.

![Home Assistant dashboard](images/dashboard.gif)

https://github.com/user-attachments/assets/92bf4580-1ab9-4535-a1f1-395bb5a3d315

## Current Status

The controller is already running on a real Rancilio Silvia. The hardware is still a prototype based on an ESP32-S3 development board and point-to-point wiring.

Current implementation:

- ESP32-S3 with ESP-IDF;
- ESPHome and Home Assistant integration;
- PT100 boiler temperature measurement through MAX31865;
- PID heater control through an SSR;
- `Brew` and `Steam` operating modes;
- automated brew shot with profiles and preinfusion;
- experimental pump power control through a RobotDyn/Robotron AC dimmer using a custom full-cycle skip ESPHome output;
- hot water and steam mode controls;
- water-level sensor input;
- automatic shutdown timer;
- shot counter, backflush reminder, and automated backflush program;
- configurable coffee dose and estimated dry coffee usage;
- PID autotune and automatic storage of successful coefficients;
- runtime pump controls for manual pump power, transition ramp time, and optional start boost.

The next hardware step is pressure sensing for closed-loop pressure profiling. Pump power control is currently an open-loop experiment: the configured percentage is sent to the pump dimmer, but there is no pressure feedback yet.

## Hardware Overview

Main parts used by the current prototype:

- ESP32-S3 development board;
- three-wire PT100 sensor;
- MAX31865 RTD amplifier;
- SSR for the boiler heater;
- relay outputs for machine power, pump, and brew valve;
- RobotDyn/Robotron AC dimmer input for experimental pump control;
- low-voltage inputs for the original power, brew shot, hot water, and steam mode controls;
- XKC-Y25-NPN water-level sensor input.

Planned additions:

- digital I2C pressure sensor for brew pressure measurement and pressure profiling;
- dedicated PCB with pluggable connectors for sensors, relays, and peripheral devices.

## Features

### Temperature And Heating

- PID heater control through ESPHome `climate`;
- separate brew and steam targets;
- adjustable brew-temperature offset used by PID control;
- raw PT100 boiler temperature remains available separately;
- PID `KP`, `KI`, and `KD` controls from Home Assistant;
- PID autotune status and automatic saving of successful autotune coefficients;
- configurable software overtemperature guard;
- SSR lockout when the PT100 reading is invalid.

### Brewing

- automated timed brew shot;
- brew profiles: `Classic`, `Soft Preinfusion`, `Long Preinfusion`, and `Custom`;
- configurable preinfusion pump time, preinfusion pause, and main shot duration;
- live brew-shot phase status with countdown;
- open-loop pump power profiles through the custom `ac_cycle_skip` output;
- physical low-voltage brew shot input;
- manual pump and brew-valve relay controls.

### Water, Steam, And Power

- machine power relay control;
- hot water mode;
- steam mode;
- physical low-voltage inputs for hot water and steam mode;
- inactivity-based automatic shutdown;
- auto-off timer reset on brew shot, hot water, steam mode, pump, and brew-valve activity;
- status LED;
- water-level monitoring and text status.

### Counters And Maintenance

- shot-count based backflush reminder;
- one-button staged backflush program with detergent, rinse-prep delay, and clean-water rinse;
- manual backflush stop and counter reset;
- lifetime shot counter;
- configurable dry coffee dose per shot;
- estimated total dry coffee usage;
- monthly and yearly coffee usage tracking via Home Assistant utility meters.

## Repository Layout

```text
.
├── README.md
├── README.ru.md
├── esphome/
│   ├── components/
│   │   └── ac_cycle_skip/
│   ├── rancilio-silvia-power.yaml
│   └── secrets.example.yaml
├── docs/
│   ├── home-assistant.md
│   ├── safety.md
│   └── wiring.md
└── images/
```

## Quick Start

1. Install ESPHome or the ESPHome Device Builder add-on in Home Assistant.
2. Copy `esphome/rancilio-silvia-power.yaml` and `esphome/components/ac_cycle_skip/` into the ESPHome configuration directory.
3. Create `secrets.yaml` using `esphome/secrets.example.yaml`.
4. Verify every GPIO assignment and the electrical design for your exact board and relay modules.
5. Validate the configuration before compiling the firmware.
6. Keep the machine under constant supervision during the first heater, pump, and valve tests.

## Home Assistant Configuration

Most user-facing values are exposed as Home Assistant entities. Values stored in YAML are initial defaults, not fixed machine specifications.

Adjustable values include:

- brew target temperature;
- steam target temperature;
- brew-temperature offset;
- PID coefficients;
- brew profile;
- preinfusion pump time;
- preinfusion pause;
- main shot duration;
- pump profile;
- manual pump power;
- pump transition ramp time;
- optional pump start boost and boost duration;
- backflush reminder threshold;
- backflush rinse-preparation delay;
- dry coffee dose per shot;
- automatic shutdown time.

### Brew Shot And Profiles

`Silvia Brew Shot` starts an automated shot sequence:

1. open the brew valve;
2. optionally run the pump for preinfusion;
3. optionally pause after preinfusion;
4. run the pump for the configured shot duration;
5. stop the pump and close the brew valve.

`Silvia Brew Profile` provides presets and a custom mode:

- `Classic`: no preinfusion, 25 s shot;
- `Soft Preinfusion`: 2 s pump, 5 s pause, 25 s shot;
- `Long Preinfusion`: 3 s pump, 10 s pause, 28 s shot;
- `Custom`: selected automatically when timing values are edited manually.

`Silvia Brew Shot Status` reports the current automated shot phase and countdown. In the current configuration the published strings are localized:

- `Предсмачивание`: remaining preinfusion pump time;
- `Пауза`: remaining pause time after preinfusion;
- `Пролив`: remaining main brew time;
- `Ожидание`: no automated shot is running.

The dashboard can use this status as the primary live shot timer instead of inferring the phase from entity timestamps.

`Silvia Pump Profile` selects the open-loop pump-power curve used by the custom `ac_cycle_skip` output. The value is still a power command, not a pressure target: without a pressure sensor the controller cannot know or hold the actual brew pressure.

Current pump profiles:

- `Classic`: normal 100% pump behavior;
- `Lever`: smooth `35% -> 75% -> 45%` pump curve;
- `Slayer Style`: smooth `30% -> 55%` low-flow start, then hold;
- `Bloom`: `35%` wetting phase, then smooth ramp to `85%`;
- `Manual`: fixed pump power from `Silvia Manual Pump Power`.

`Silvia Manual Pump Power` is adjustable from `0%` to `100%`. `Silvia Pump Ramp Time` controls how quickly the component moves from one requested power level to another. `Silvia Pump Start Boost` can briefly send full power when starting from `0%`; it is disabled by default, and `Silvia Pump Start Boost Time` sets the boost duration when enabled. `Silvia Pump Gate Delay` and `Silvia Pump Gate Pulse` tune the TRIAC trigger timing in microseconds.

### AC Cycle Skip Pump Output

The pump dimmer uses a local ESPHome external component in `esphome/components/ac_cycle_skip/`. It is not a normal phase-angle dimmer. A phase-angle dimmer cuts every mains half-wave; at low settings the pump receives weak chopped sine fragments and may only buzz.

Example at roughly `30%`:

```text
phase-angle dimmer:
  every half-wave is chopped and weak
  ~~~/    ~~~/    ~~~/    ~~~/

ac_cycle_skip:
  complete mains periods are passed or skipped
  ON period -> skip -> skip -> ON period -> skip -> skip
```

The component keeps a fractional accumulator, so low percentages are spread across time instead of being sent as a single clump. When the target changes, the internal target moves toward the new value over `ramp_ms`.

Example transition from `30%` to `80%` with `ramp_ms: 800`:

```text
requested: 30% ----------------------> 80%
internal:  30% -> 38% -> 46% -> 54% -> 62% -> 70% -> 80%
output:    sparse full cycles gradually become denser
```

The brew profile code updates the requested pump power during the shot, and `ac_cycle_skip` smooths the electrical output between those requested values. This reduces abrupt pump changes while still keeping the output synchronized to zero crossings.

Gate timing and safety details:

- the GPIO zero-cross ISR no longer busy-waits for the gate pulse;
- the TRIAC gate pulse is scheduled with an ESP-IDF GPTimer running at 1 MHz;
- `gate_delay_us` waits briefly after zero-cross before triggering the gate, and `gate_pulse_us` controls how long the gate stays high;
- the defaults are `gate_delay_us: 100` and `gate_pulse_us: 300`, and the delay cannot be set below `10 us`;
- runtime timing values are read atomically, and a generation guard prevents a stale timer callback from raising the gate after an OFF command;
- `write_state(0)` and component shutdown force the gate pin low immediately;
- zero-cross intervals outside the configured valid window force the output off and start resynchronization;
- the component waits for consecutive valid zero-cross intervals before resuming output after a sync error.

### Coffee Usage

`Silvia Coffee Dose Grams` stores the configured dry coffee dose per shot. The default is `14 g`, and the value can be changed from Home Assistant.

`Silvia Coffee Grounds Used` estimates total dry coffee usage:

```text
Coffee Grounds Used = Silvia Lifetime Shots × Silvia Coffee Dose Grams
```

This is an estimate of ground coffee consumption, not the beverage weight in the cup.

Home Assistant can split this cumulative value into calendar periods with `utility_meter`:

- `coffee_grounds_used_monthly` for the current month;
- `coffee_grounds_used_yearly` for the current year;
- `Silvia Coffee Grounds Used` remains the lifetime total.

The coffee dashboard displays all three values as monthly, yearly, and total dry coffee usage.

### Backflush Program

`Silvia Backflush Shots` counts completed automated shots since the last group backflush. `Silvia Lifetime Shots` keeps the total shot count and is not reset by cleaning.

`Silvia Backflush Reminder Shots` sets the reminder threshold. The default is `60` shots. Setting it to `0` disables the reminder. When the counter reaches the threshold, `Silvia Backflush Status` changes to a due state. The reminder is informational only and does not block brewing.

`Silvia Start Backflush` runs a one-button staged cleaning sequence intended for a blind basket:

1. Detergent stage: `8` cycles of `5 s` pump/valve on and `10 s` off.
2. Rinse preparation delay: the machine stops and `Silvia Backflush Status` counts down `Prepare rinse | N s`. During this pause, remove the portafilter, rinse out the detergent, clean the blind basket, empty the drip tray if needed, and lock the clean blind basket back in.
3. Clean-water rinse stage: `8` more cycles of `5 s` on and `10 s` off, with no detergent.

`Silvia Backflush Rinse Delay Seconds` controls the pause between the detergent and rinse stages. The default is `120 s`, adjustable from Home Assistant.

During the program, `Silvia Backflush Status` reports `Cleaning | N/8 cycles`, `Prepare rinse | N s`, and `Rinsing | N/8 cycles`. The backflush shot counter is reset only after the clean-water rinse stage completes.

`Silvia Stop Backflush` aborts the running sequence and turns off the pump and valve. `Silvia Reset Backflush Shots` manually clears the reminder counter.

Recommended workflow:

1. Warm up the machine, insert the blind basket, add the backflush detergent, and press `Silvia Start Backflush`.
2. Wait until the first stage finishes and the status changes to `Prepare rinse | N s`.
3. During the countdown, remove the portafilter, discard/rinse out the detergent, rinse the portafilter and blind basket, and empty the drip tray if it is getting full.
4. Reinstall the clean blind basket without detergent before the countdown reaches zero.
5. Let the clean-water rinse stage finish. When it completes, the backflush reminder counter is reset.

### Brew Temperature Model

`Silvia Brew Target` represents the desired estimated temperature at the coffee puck. In `Brew` mode, PID control uses:

```text
Estimated Brew Temperature = PT100 Boiler Temperature - Brew Temperature Offset
Brew Boiler Target = Brew Target + Brew Temperature Offset
```

For example, a brew target of `93 °C` with a `10 °C` offset produces a boiler target of approximately `103 °C`.

The PT100 entity always reports the unmodified boiler temperature. The offset is not applied in `Steam` mode, and the software overtemperature guard always uses the raw PT100 reading.

The estimated brew temperature is a model, not a direct water measurement. Keep the offset at `0 °C` until it has been calibrated at the group under realistic flow conditions.

## Roadmap

- Add a digital I2C pressure sensor for brew pressure measurement.
- Use pressure feedback for closed-loop pressure profiling.
- Update the Home Assistant dashboard with pressure graphs.
- Design a dedicated PCB with proper connectors.
- Expand wiring and Home Assistant documentation.

## Documentation

- [Wiring and GPIO](docs/wiring.md)
- [Home Assistant](docs/home-assistant.md)
- [Safety](docs/safety.md)

## License

No license has been granted yet. All rights are reserved by the author.
