# WFSP - Weighted Fair Sequences Problem

The WFSP is a new optimization problem that has a wide spectrum of applications, ranging from task scheduling in real-time systems to automobile production on a mixed-model assembly line.

This program is now used to create a better way to transmit data from the
company to the client.

## Installation

Download the CPLEX installer and use the C++ API.

## Usage example

Usage to alocate the data transmission through Digital TV.

_For more examples and usage, please refer to the [WFSP Website](https://sites.google.com/site/weightedfairsequencesproblem/home)._

./main -v vv -c 9 -t 1 -i ./instances/ins_07_14_1.txt

### Parameters

- -v or --verbose
  - mode with more output
- -c or --cuts

  - implementation and where (root or callback)

  | Number |  Preposition  | Cut |    Where    |
  | :----: | :-----------: | :-: | :---------: |
  |   0    | Without cuts  |     |   Nowhere   |
  |   1    |       4       | C1  |   In root   |
  |   2    |       5       | C2  |   In root   |
  |   3    |       6       | C3  |   In root   |
  |   4    |  Conjecture   |     |   In root   |
  |   6    |       5       | C2  | In Callback |
  |   9    | 4, 5, 6, Conj | All |   In root   |

* -t or --threads

  - number of threads used in the CPLEX, in range [0,16].

* -i or --input
  - instance file path

## Development setup

Describe how to install all development dependencies and how to run an automated test-suite of some kind. Potentially do this for multiple platforms.

To run one instance use the code in CPP folder like the Usage example.
To run the benchmark program, open Python folder and run ./main.py program.

## Release History

- 1.0.0
  - New objective function with conjecture enabled
- 0.0.1
  - Work in progress

## Meta

Lucas Nogueira Nobrega – lucasnnobrega@gmail.com

## Contributing

1. Fork it (<https://github.com/yourname/yourproject/fork>)
2. Create your feature branch (`git checkout -b feature/fooBar`)
3. Commit your changes (`git commit -am 'Add some fooBar'`)
4. Push to the branch (`git push origin feature/fooBar`)
5. Create a new Pull Request
