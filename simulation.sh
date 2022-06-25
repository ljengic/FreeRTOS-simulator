#!/bin/bash

declare -i num_of_reports=0
declare -i num_of_tasks=5
declare -i utilization=120

#cd ~
#cd FreeRTOS-simulator
cd Reports

dir=$(pwd)
algoritam=RTO

#echo $dir

num_of_reports=$(ls | wc -l)
num_of_reports=num_of_reports+1

#echo $num_of_reports

location="${dir}/${algoritam}_${utilization}.csv"

echo $location

cd ..

./make_report_file $num_of_tasks $location

for ((i=0;i<=20;i+=1)) do
	./FreeRTOSv202112.00/FreeRTOS/Demo/Posix_GCC/build/posix_demo $utilization $num_of_tasks $location
done
