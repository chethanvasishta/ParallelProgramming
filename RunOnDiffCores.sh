#!/bin/sh

`taskset 0x01 ./Par 1.csv` #CPU 0
`taskset 0x03 ./Par 2.csv` #CPU 0 and 1
`taskset 0x07 ./Par 3.csv` #CPU 0, 1 and 2
`taskset 0x0F ./Par 4.csv`
`taskset 0x1F ./Par 5.csv`
`taskset 0x3F ./Par 6.csv`
`taskset 0x7F ./Par 7.csv`
`taskset 0xFF ./Par 8.csv`
