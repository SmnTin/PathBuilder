# Path Builder project
The algorithm of finding optimal route through all  the given points was implemented in this project, in other words, traveling salesman problem.It was designed for the Unwayl app to build optimal routes through real places with given timetable. It uses DP with subsets to build path inside the day and a greedy solution for building the whole week. This algorithm works under the http-interface, and also it both gets requests and gives responses in the JSON-format.

## Getting started
### Dependencies
- [Google Test](https://github.com/google/googletest) for the tests
- [Boost.Asio](https://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio.html) for cross-platform sockets
- [Simple-Web-Server](https://gitlab.com/eidheim/Simple-Web-Server) for the http-interface implementation

### Prerequisites
However, **Google Test** and **Simple-Web-Server** will be installed automatically, Boost.Asio will not. So do it manually.
For instance, on Ubuntu/Debian type the following into the terminal :
```bash
sudo apt-get install libboost-all-dev
```
And you also need CMake and G++ compiler to build the project.
On Ubuntu/Debian:
```Shell
sudo apt-get install build-essential
sudo apt-get install cmake
```

### Installation
Firstly, clone the repo and submodules.
```bash
git clone https://github.com/SmnTin/PathBuilder
cd PathBuilder/
git submodule init
git submodule update
```
Then configure the build.
```Shell
mkdir build
cd build
cmake ..
```
And, finally, build.
```
make -j4
```

## Usage
### Executing
Inside the *build* directory run **PathBuilder** executable.
```Shell
./PathBuilder
```
Then it would be available on the http://localhost:9005

### Example inputs and outputs
```TSX
{
  "INPUT": {
    "mode": "full", //to build optimal routes
    //or
    "mode": "route", //to check if it is possible to complete the route in one day
    "starting_pos": {
      "lat": 111,
      "long": 222
    },
    "first_day_start": "10:00",
    "day_start": "9:00",
    "day_end": "23:00",
    "day_of_week": "monday",
    "places": [{
      "id": 0, //in case of "route" mode
      "coords": {
        "lat": 111,
        "long": 222
      },
      "timetable": {
        "monday": [
          //the user can go at any time inside the interval
          {
            "type": "interval",
            "starts": "9:00",
            "ends": "18:00",
            "lasts": "1:00",
            "price": 150
          },
          {
            "type": "interval",
            "starts": "19:00",
            "ends": "20:00",
            "lasts": "1:00",
            "price": 150
          },
          //the user must visit the place only at the corresponding time point
          {
            "type": "time_point",
            "starts": "9:00",
            "lasts": "1:00",
            "price": 150
          }
        ]
      },
      "time_to_get": "1:00"
    }],
    "matrices": [ //values are in minutes
      [0,0,0,0], //time by foot-walking
      [0,0,0,0], //time by taxi
      [0,0,0,0], //time by public transport
      [0,0,0,0]  //chosen transport
    ]
  },
  "OUTPUT_MODE_FULL": {
    "days": [{
      "places": [{
        "id": 0,
        "starts": "9:00",
        "lasts": "1:00",
        "price": 150,
        "transports": { //transports to get out of (V2: to get into) this point
          "0": {//foot-walking
            "possible": true, //if it's possible to use this transport without breaking the order of pts
            "takes": 15 //minutes
          },
          "1": { //taxi
            "possible": true,
            "takes": 5
          },
          "2": { //public transport
            "possible": true,
            "takes": 10
          }
        }
      }],
      "price": 150
    }],
    "full_price": 150
  },
  "OUTPUT_MODE_ROUTE": {
    "possible": true,
    //if possible:
    "day": {
      "places": [{
        "id": 0,
        "starts": "9:00",
        "lasts": "1:00",
        "price": 150,
        "transports": { //transports to get out of (V2: to get into) this point
          "0": {//foot-walking
            "possible": true, //if it's possible to use this transport without breaking the order of pts
            "takes": 15 //minutes
          },
          "1": { //taxi
            "possible": true,
            "takes": 5
          },
          "2": { //public transport
            "possible": true,
            "takes": 10
          }
        }
      }],
      "price": 150
    }
  },
}
```

## Testing
If you want to see if it passes tests, run the following inside the *build* directory:
```Shell
./tests
```

## Contributing
No contributing is available yet.

## Authors
Entirely written by **SmnTin**.