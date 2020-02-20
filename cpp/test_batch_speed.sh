#!/bin/bash
mkdir build
cd build && cmake ..
make
for ((i=0; i<100; ++i)); do echo "10000 3 2 512 128"; done | xargs -n 5 -P 8 -I {} sh -c "echo {} | ./example" | awk '{print $5; s+=$5; ++n}END{print s/n}'
cd ..
