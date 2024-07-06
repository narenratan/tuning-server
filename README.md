Tuning server
=============

Forward MTS-ESP tuning to other computers.

Why?
----
[MTS-ESP](https://github.com/ODDSound/MTS-ESP) retunes synths running on the
same computer. I wanted to retune a synth running on a Raspberry Pi using the
MTS-ESP source in a DAW on my laptop.

How?
----
We can forward the MTS-ESP tuning from the laptop to the Raspberry Pi by
running:

- A tuning server on the laptop, which responds to http requests by sending the
  current MTS-ESP tuning as json.
- A tuning client on the Raspberry Pi, which requests the tuning once per
  second and updates the MTS-ESP tuning on the Raspberry Pi.

In this way the MTS-ESP source on the laptop and the synth on the Raspberry Pi
can use MTS-ESP as normal; they don't need to know they're running on different
computers.

What?
-----
This repo provides:

- `tuning-server` and `tuning-client` command line programs
- An audio plugin `TuningServer.clap` which runs the tuning server in a DAW

The CLAP plugin saves manually running the `tuning-server` CLI program; the
plugin automatically starts and stops the tuning server.

Build!
------
```console
$ git clone --recurse-submodules https://github.com/narenratan/tuning-server
$ cd tuning-server
$ cmake -B build
$ cmake --build build
```
