# Prime Map

A simple multi-threaded program that finds primes and sores them in a cool looking bitmap

## Installation

Please use `<make> .\bin\make.exe` to compile the program. You can also use `<make> all`.

## Usage

Run the excutable with these parameters:

| Parameter | Default |                                                                 |
|-----------|---------|-----------------------------------------------------------------|
| -t        | 4       | Use this argument to specify number of threads                  |
| -s        | 5000    | Use this argument to specify the number of simulations          |
| -p        | 25      | Use this argument to specify the maximum number of people       |
| -r        |         | Use this argument to specify the number of people, runs singles |
| -d        | False   | Use this switch to specify whether to display the bitmap        |

Usage:   `.\\BirthdayPDX.exe [-t <threads>] [-s <simulations>] [-p <people>] [-d] [-r <people>]`
Example: `.\\BirthdayPDX.exe -t 7 -s 500000 -p 50`
Example: `.\\BirthdayPDX.exe -t 7 -s 500000 -d -r 50`
  
## Contributing

1. Fork it!
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request :D

## License

Copyright (c) 2017 The Gupta Empire - All Rights Reserved

Copyright (c) 2017 Aryan Gupta
