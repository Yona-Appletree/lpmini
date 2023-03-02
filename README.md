# lightplayer-mini

An application for controlling smart LEDs in interesting ways from an ESP32
configured with a web-based node style programming environment similar to NodeRED.

## Development

Install prerequisites:

```
brew install nvm
nvm install
npm install -g pnpm
``
```

- run `pnpm install` to install dependencies
- `cd packages/liblp && pnpm run test:watch` to interactively build the wasm module and run tests

# Design

## Rationale

I wanted a simple and visual way to build LED visualizations and animations
run from cheap widely available microcontrollers.

None of the available solutions, such as WLED, or Pixelblaze, met my needs
for a simple and visual way to program the animations.

## Goals

- Ability to run on an ESP32 and control WS2811-protocol LEDs
- Simple and visual programming environment inspired by NodeRED and TouchDesigner
- Rich visual debugging capabilities for animations
- Ability to build custom UIs for controlling animations, suitable for use by non-technical users
- Support for hardware input, such as accelerometers, microphones, and buttons
- Performance to run animation consisting of 50 nodes at 30fps on 150 LEDs
- Support for interpolation, if needed, to compensate for poor frame rate

## Non Goals

- Support for LED protocols other than WS2811
- Support for other microcontrollers
- General purpose programming environment
- Integration with home automation or similar

## Architecture

lightplayer-mini is built from three parts:

- liblp: A C++ library implementing the node-based-programming kernel
- weblp: A web application that uses a WebAssembly build of liblp to provide
  the visual programming environment, debugger, and custom UIs.
- embeddedlp: An ESP32 firmware that runs liblp on a microcontroller to drive the LEDs,
  and a webserver to host the web application.

### liblp

liblp a node-programming interpreter built on top of the duktape embedded javascript interpreter.
Duktape provides memory management, stack management, and garbage collection, and utilities for
easy integration with the javascript host.

Using a javascript interpreter is a logical choice, since the web application is written in javascript.
It also allows easy prototyping of new nodes in javascript. They can be rewritten in C++ for performance
if needed.

### weblp

A React applications implementing the UI.

## References

- Based on https://github.com/9oelM/emscripten-cplusplus-webpack-example
