# Photos Manager

Photos Manager Application for linux systems.

## Features
* Auto Scanning of home folder
* Auto detection of faces
* Auto categorization of faces detected.
* Auto categorization of photos on the basis of events

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

What things you need to install the software and how to install them

* opencv
* Gtkmm

### Installing
* Clone this repo using `git clone <repo_url>`
* Enter into the repo directory.
* Type `cmake .` to prepare the make system.
* Type `make all` to compile the sources.
* Type `make docs` to generate documentation for this project.
* Type `make install` to install it into /usr/local directory.


## Built With

* gktmm-3.0 - The GUI Framework
* OpenCV - The Computer Vision Library.
* GNU/Linux - The Linux Environment

## Contributing

Please read [CONTRIBUTING.MD](/CONTRIBUTING.MD) for details on our code of conduct, and the process for submitting pull requests to us.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](/tags). 

## Documentation

Documentation for this project are automatically generated using doxygen. To generate documentation for this,
type `make docs` and all the documentations wil be developed inside `docs`. You can click on index.html file in the root folder to access the documentation. The file is a symbolic link and will be broken untill documentation is generated. You will see the documentation [here](documentation/index.md).

## Authors

* **Dinesh Bhattarai** <dineshbhattrai0@gmail.com> 
* **Aashutosh Poudyal** <aashutoshpoudyal@gmail.com>
* **Krishna Upadhyay** <krishnaupadhyay1997@gmail.com>

See also the list of [contributors](/CONTRIBUTORS.MD) who participated in this project.

## License 
[LICENSE.MD](LICENSE.MD)
