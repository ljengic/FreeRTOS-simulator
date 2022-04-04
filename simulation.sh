#!/bin/bash

for ((i=50;i<=150;i+=1)) do
	./FreeRTOSv202112.00/FreeRTOS/Demo/Posix_GCC/build/posix_demo $i 3
done
