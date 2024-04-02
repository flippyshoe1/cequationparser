a small "library" (a mere c file and header file) that solves equation strings (and more!)

requirements:
- gcc

dependencies (for debian and derivatives)
```sh
apt install build-essential
```

dependencies (for arch and derivatives)
```sh
pacman -S gcc 
```

to build a binary (the provided main.c is just to test out the equation_solve function)
```sh
git clone https://github.com/flippyshoe1/cequationparser.git
cd cequationparser
make

# if you wish to clean up afterwards
make clean
```