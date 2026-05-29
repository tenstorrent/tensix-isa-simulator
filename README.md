# Tensix ISA simulator

## Archived - No longer supported

This repository is no longer supported.  It's functionality can be found at the [tenstorrent/ttsim](https://github.com/tenstorrent/tt-sim) repository.

## Original README

This repository contains code and materials pertaining to Tensix ISA simulator.

Development of the simulator has two main goals:

* Provide a foundation for a convenient toolchain aiming at designing, 
debugging, and testing of Tensix low-level kernels (LLK) on the conventional CPUs 
without using the real Tenstorrent hardware.

* Provide a verifiable formal functional specification of Tensix ISA which can serve
as a basis for the detailed architecture documentation.

The simulator shall be considered work in progress.

The software requires Linux and has been tested on Ubuntu 20.04.

Currently, the simulator version supporting Wormhole B0 architecture is available.
It is located in [this directory](./tensix/whb0).

