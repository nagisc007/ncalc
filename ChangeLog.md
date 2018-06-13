# ChangeLog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [1.1.2] - 2018-06-11
### Added
- gpu (converted cpu data to display)

### Changed
- cpu renamed from core

### Refined
- inner methods
- connections between mainwindow and cpu (gpu)

### Removed
- mem class

## [1.1.0] - 2018-05-17
### Added
- common_types
- common_values
- cpu thread
- Result(ACC) display
- Undo, Redo features
- Separate the memory areas

### Removed
- Binary display mode

### Changed
- Main UI design
- Digit changed button(integrated one button)
- GPLv3 from GPLv2 (forget changed)
- Inner arithmetic functions to lambdas from functors
- Converting method the number to display (mainwindow from core)

## [1.0.0] - 2018-01-15
### Added
- Basic UI
- Add, Subtract, Multiply, Devide features
- And, Or, Not, Xor features
- Clear feature
- Back Clear feature
- Decimal, Hexadecimal mode
