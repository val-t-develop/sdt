# SDT - Scientific Document Tool

SDT (Scientific Document Tool) is a command-line utility for processing scientific documents written in a custom XML-like format. It supports configurable document styling and generates PDF output using Apache FOP, with support for custom configuration via JSON files.

## Features
- Converts custom .sdt documents to PDF
- Supports configurable styles via JSON configuration
- Uses Apache FOP for PDF generation
- Command-line interface with flexible arguments
- Written in modern C++ (C++20)

## Build & Installation
This project uses [Meson](https://mesonbuild.com/) as its build system. Make sure you have Meson, Ninja, a C++20 compiler, and the following dependencies installed:
- Boost (with JSON module)
- libxml2

### Build Steps
```sh
# Install dependencies (example for Ubuntu)
sudo apt install meson ninja-build g++ libboost-all-dev libxml2-dev

# Configure and build
meson setup buildDir
meson compile -C buildDir

# Optionally put executable to /usr/bin/
cp buildDir/sdt /usr/bin/sdt
```

## Usage
The tool is run from the command line. Example:
```sh
sdt <input.sdt> -c <config.json> -o <output.pdf>
```

### Arguments
- `<input.sdt>`: Input document in SDT format (required)
- `-c <config.json>`: (Optional) JSON configuration file for styles
- `-o <output.pdf>`: (Optional) Output PDF file name
- `-g` or `-d`: (Optional) Enable debug mode (keeps intermediate files)
- `show c` or `show w`: (Optional) Show license information

### Example
You can find few examples in ```examples``` directory. To compile them you can use commands like this:
```sh
sdt examples/1/example.sdt -c examples/1/conf.json -o examples/1/example.pdf
```
This will generate `example.pdf` in ```examples/1```.

## License
This project is licensed under the GNU General Public License v3.0. See the [LICENSE](LICENSE) file for details.

---
**Author:** Valentyn Tymchyshyn (val.t.develop@gmail.com) 