# FSR Music Box

Four force-sensing resistors on an Arduino Mega become a drum pad in the browser. Each pad is a voltage divider on an analog pin. The firmware prints pressure readings over USB serial, and the page plays a light or heavy hit from how hard you press. You can keep the built-in kick, snare, hat, and tom, or load your own samples.

The point of the project is the boundary between the board and the browser: an analog sensor, a 10-bit ADC reading, a one-line serial protocol, and the Web Serial API. The Mega does the sensing. The page does the sound.

The live site is [https://aneesh-pradhan.github.io/cmpe344-project/](https://aneesh-pradhan.github.io/cmpe344-project/).

## Course

This is a project for [CMPE 344, Microprocessor Laboratory](https://catalog.csueastbay.edu/preview_course_nopop.php?catoid=44&coid=187609) in the Computer Engineering B.S. at California State University, East Bay. The catalog describes it as microprocessor organization, operation, and programming, with hardware/software interaction, I/O port interfacing, and interfacing to analog and digital devices (3 units: 2 lecture, 1 lab; prerequisites CS 201 and ENGR 230).

The repository name and the commit author address (`apradhan5@horizon.csueastbay.edu`) are what tie the code to that course. The repo does not record a term or an instructor.

## Hardware

The repository has no photographs or screenshots of the build or the page. The wiring below is the circuit the firmware comment and the ADC logic describe.

The sketch reads four divider outputs on `A0`–`A3` and treats a larger ADC count as a harder press. It never calls `analogReference()`, so the Mega uses its default 5 V reference and `analogRead()` returns 0–1023. Samples at or below the firmware deadzone (`DEADZONE`, 30) are not printed.

### Bill of materials

| Qty | Part | Role |
| --- | --- | --- |
| 1 | Arduino Mega 2560 | ATmega2560. Fully qualified board name `arduino:avr:mega`, processor `cpu=atmega2560`. |
| 1 | USB-B cable | Power and serial for an official Mega 2560. Plug it into the computer that opens the page. |
| 4 | Force-sensing resistors | One pad each. The sketch does not name a manufacturer or part number. |
| 4 | Pulldown resistors | Bottom half of each divider. The sketch does not store a value. A 10 kΩ pulldown is a common start for an Interlink-style FSR. |
| 1 | Breadboard and jumper wires | Build the four dividers and land them on `A0`–`A3`, `5V`, and `GND`. |

Size the pulldown so an idle pad sits at ADC 30 or below (otherwise the firmware streams forever) and a firm press climbs past the page threshold, which defaults to 512. If a 10 kΩ pulldown misses either of those, change the resistor rather than the deadzone constant.

### Wiring

Use the Mega's `5V` and `GND` pins, with the board powered over USB. Repeat this divider once per pad. Pad 0 is `A0`, pad 1 is `A1`, pad 2 is `A2`, pad 3 is `A3`.

```
5V ----[ FSR ]----+---- A0 (or A1, A2, A3)
                  |
             [ pulldown ]
                  |
                 GND
```

An FSR is not polarized. Force has to raise the voltage at the analog pin: the fixed resistor goes to ground, and the FSR goes to 5 V. The divider ground is the Mega ground.

| Pad | Page name | Pin | Built-in sounds |
| --- | --- | --- | --- |
| 0 | Pad A | A0 | Kick low / Kick high |
| 1 | Pad B | A1 | Snare low / Snare high |
| 2 | Pad C | A2 | Hat low / Hat high |
| 3 | Pad D | A3 | Tom low / Tom high |

## Build and flash

The sketch is `fsr_music_box_mega/fsr_music_box_mega.ino`. Arduino CLI and the IDE require the folder name to match the `.ino` file, so the sketch lives in that directory. `sketch.yaml` in the same folder pins the board to `arduino:avr:mega:cpu=atmega2560` and the core to Arduino AVR Boards 1.8.8.

Libraries: none. Library Manager has nothing to install. The sketch uses only the AVR core (`Serial`, `analogRead`, `delay`, and the `A0`–`A3` pin constants).

Arduino AVR Boards 1.8.8 brings its own tools. A current install of that core uses:

- `arduino:avr-gcc` 7.3.0-atmel3.6.1-arduino7
- `arduino:avrdude` 8.0.0-arduino1
- `arduino:arduinoOTA` 1.3.0

### Arduino CLI 1.6.0

Install [Arduino CLI 1.6.0](https://github.com/arduino/arduino-cli/releases/tag/v1.6.0) and confirm the binary:

```bash
arduino-cli version
```

The first line should be `arduino-cli  Version: 1.6.0`. From the repository root:

```bash
arduino-cli core update-index
arduino-cli core install arduino:avr@1.8.8
arduino-cli compile --fqbn arduino:avr:mega:cpu=atmega2560 fsr_music_box_mega
```

`compile` only builds. Uploading is a second step, and it needs the Mega plugged in. List ports, then compile and upload in one command. Replace `PORT` with the port from `board list` (`/dev/ttyACM0` for an official Mega on Linux, `/dev/ttyUSB0` for many CH340 clones, `/dev/cu.usbmodem…` on macOS, `COMx` on Windows).

```bash
arduino-cli board list
arduino-cli compile --upload -p PORT --fqbn arduino:avr:mega:cpu=atmega2560 fsr_music_box_mega
```

On Linux, serial-port access usually requires membership in the `dialout` group. After `sudo usermod -a -G dialout "$USER"`, log out and back in before uploading.

### Arduino IDE 2.3.11

[Arduino IDE 2.3.11](https://github.com/arduino/arduino-ide/releases/tag/2.3.11) (12 November 2026) bundles Arduino CLI 1.6.0.

1. Install IDE 2.3.11.
2. Open `fsr_music_box_mega/fsr_music_box_mega.ino`.
3. Open the Boards Manager, search for "Arduino AVR Boards", and install version 1.8.8.
4. Tools → Board → Arduino AVR Boards → **Arduino Mega or Mega 2560**.
5. Tools → Processor → **ATmega2560 (Mega 2560)**.
6. Tools → Port → the Mega's port.
7. Sketch → Upload.

No libraries to add.

### Confirm the serial stream

After a successful upload, open a monitor at 115200 baud. An idle pad prints nothing. Pressing pad 0 prints lines like `0:612`.

```bash
arduino-cli monitor -p PORT --config 115200
```

In the IDE serial monitor, set the baud rate to 115200. The page accepts that compact line, a space-separated `0 612`, or `PAD:0,VAL:612`.

## View the site locally

The page uses the Web Serial API. Its own check asks for Chrome 89 or newer and a page served over HTTPS or localhost. Desktop Edge also provides `navigator.serial`. Open either the live site above or a local server. A `file://` URL will not connect.

From the repository root:

```bash
python3 -m http.server 8000
```

Then open [http://127.0.0.1:8000/main.html](http://127.0.0.1:8000/main.html). You should see the header "FSR Music Box / Arduino Mega" and the "Connect your Arduino" panel. Click **Connect**, choose the Mega, and the four pads appear. GitHub Pages serves the same file at the site root because the deploy workflow copies `main.html` to `dist/index.html`. Locally the file keeps the name `main.html`.

On the pad screen, values above the deadzone and at or below the threshold play the light sample at 65% gain. Values above the threshold play the heavy sample. Both default to a synthesized drum; each slot accepts an `audio/*` file. The threshold slider starts at 512, the page deadzone slider starts at 30, master volume starts at 80%, light pitch starts at 0 semitones, and heavy pitch starts at +5 semitones. The firmware already drops samples at or below 30, so the page never sees those readings on a live board. If the firmware goes quiet for 120 ms, the page treats the pad as released.

## GitHub Pages

[`.github/workflows/deploy-pages.yml`](.github/workflows/deploy-pages.yml) publishes the site on every push to `main`, and it can also be run by hand. The build job checks out the repo, copies `main.html` to `dist/index.html`, adds `dist/.nojekyll`, and uploads a Pages artifact. The deploy job publishes that artifact to the `github-pages` environment. The site built from that workflow is [https://aneesh-pradhan.github.io/cmpe344-project/](https://aneesh-pradhan.github.io/cmpe344-project/).

[`.github/workflows/compile-sketch.yml`](.github/workflows/compile-sketch.yml) compiles `fsr_music_box_mega` with Arduino CLI 1.6.0 and Arduino AVR Boards 1.8.8 for `arduino:avr:mega:cpu=atmega2560` on pull requests and on pushes to `main`. It does not upload to a board.

## License

[MIT](LICENSE). Copyright (c) 2026 Aneesh Pradhan.
