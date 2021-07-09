[![Build](https://github.com/CalumD/Java-JSON/WordBrush/docker-image/badge.svg)](https://github.com/CalumD/WordBrush/actions/workflows/docker-image.yml)



# WordBrush [![WordBrushLogo](https://github.com/CalumD/WordBrush/blob/master/examples/WordBrush.svg)](https://github.com/CalumD/WordBrush)

This project takes ASCII input and draws the resultant line, as if you were to 'swipe-to-type' that word across a
touchscreen keyboard.

## About

The inspiration for this project was when I was out walking one day and was one-handed typing a message to someone. I
happened to 'pay attention' to the tiny little trail of the swipe text entry on iOS following my finger. I thought it
would be cool if I could see what that whole line looked like for words / sentences at a time.

Simultaneously, I was missing having written code in ```C``` for a while, as I hadn't had any projects in it since ~3rd
year of uni. So I had been trying to come up with an idea for a manageable project which I thought I could write in C,
but which was still going to have a fun/useful endgame.

Enter, this project. An application which can take in text, then draw a line to represent that word in SVG format, so
that it would be easily scalable. I wanted the ability to show/hide the keyboard, as well as have options for
width/height. A sample output for the word "wordbrush" is shown in the logo at the top of the readme.

As an extension piece I knew I also wanted a web interface you could use to input words and have the output shown
visually too. The website ended up being a significant amount more development time than initially anticipated. A lot of
the reason for this can be put down to the fact I chose to write it
in [VueJS 3.0](https://v3.vuejs.org/ "Vue 3's homepage"). Not that I use frontend frameworks a lot, but Vue3 in
particular is relatively new in general, and I have never used it before. It is gaining in popularity though and I found
it's reactivity and learning experience to be very rewarding.

## Deployment

#### <u>Docker (preferred)</u>

I have created a Dockerfile for ease of use. It is also available on the docker
hub [here](https://hub.docker.com/r/agentclum/wordbrush "Dockerhub link to the project."). The container captures the
environment I coded the application under, and allows easy portability and interaction for an end user \*fingers
crossed\*. You can either download from dockerhub directly if you know what you're doing using the 'latest' tag, or you
can use the Dockerfile in this repo.

In order to compile the docker file, use something along the lines of the following:

```
docker build -t wordbrush:latest .
```

And run the container with ~

```
docker run \
    -p 8080:8080 -p 3000:3000 \
    -v </path/to/host/directory>:/wordbrush/resources \
    --env BACKEND_ADDRESS=<hostname/ip of container> \
    --name wordbrush \
    wordbrush:latest
```

Where:

- ```-p``` is used to match the UI port (8080), and API port (3000) to the host's equivalent ports.
- ```-v``` is used to bind-mount a local directory on the host to a pre-defined path inside the container.
    - This option is only necessary if you want to persist the images between docker container reboots, otherwise it can
      be omitted.
    - **Please note**: The application runs under a unix 'wordbrush' user and group. If you choose to use this mount,
      you _must_ ensure that the directory has appropriate read/write permissions for the container. You can
      use ```chown 777 /path/to/host/directory```, on the host machine before running to ensure it will be accessible
      once mounted into the container.
- ```--env``` is used to set the container's environment variable for BACKEND_ADDRESS, to override the default '
  localhost' value for the UI to communicate with the backend API.
    - If you wish to use the application from any machine OTHER than the machine where docker is running, you must
      provide a suitable value here. The reason for this is that the frontend framework needs to make calls to the
      backend API, but the frontend is loaded into the browser of the computer using the application, so the 'localhost'
      of the client is not necessarily the same as the UI/server.
    - When setting this value, please use a network accessible hostname/ip to where the container is being run from.

#### <u>Local compilation</u>

If you choose to faf around with the source in your own environment, that's fine too - however a linux environment is
assumed, native windows support only exists through the docker version.

- All of the C code is present under ```~/compute```.
    - A Makefile is provided with various targets to play around with.

The frontend is split into 2 sub projects, both with their own package.json files. You can
run ```npm install && npm run serve```, in both of these subdirectories to install all the required npm packages, then
boot a development server to play around with.

- ```~/frontend/server```, a Node server for the backend API.
- ```~/frontend/ui```, the VueJS 3 web interface.
- A local directory ```~/resources``` is not provided (as it would be empty), but is expected to exist, as there is a
  decent amount of hard-coding (... yes I know :c ) looking for it.

## Usage

#### <u>The full webservice</u>

If you chose to run the project with docker, using it should be as easy as running the container, then connecting to the
web address where it is running. There is no https version, and remember the port number, if hosting it on your own
machine, this should be <http://localhost:8080>.

However if you are managing everything locally, you will need to:

- make sure you have first compiled the C code
- make sure you have npm installed in BOTH frontend sub directories
- have BOTH the server, and ui running simultaneously with ```npm run serve```

#### <u>Just the Application</u>

This section assumes you don't care about the provided UI, and just want to mess around with the C program (the original
main point of this project) in isolation.

The following is the output from running the compiled program with ```--help```, it pretty much speaks for itself.

```
Usage: wordbrush [OPTION...] [WORD INPUT]

-h, -H, --height=NUM       The height of each individual output image
-i, --input=PATH           Path to a text file to be used as word input.
-k, --keyboard, --keeb     Display the keyboard in the background
-o, --output=PATH          Path to DIRECTORY to use if in multi-file output
                           mode, or path to FILENAME if in single-file output
                           mode.
-s, --sfo=NUM, --columns=NUM   Number of columns per row in single-file
                           output mode.
-w, -W, --width=NUM        The width of each individual output image
-?, --help                 Give this help list
--usage                    Give a short usage message
-V, --version              Print program version

Mandatory or optional arguments to long options are also mandatory or optional
for any corresponding short options.

Report bugs to https://github.com/CalumD/WordBrush/issues.
```

There are only 2 mandatory options.

- ```-o``` is required to tell the code where it should write the image outputs.
- Some method of input, whether this is further CLI args, or an input file pointed to by ```-i```.

A known limitation is that you cannot use an input file, and the columns argument simultaneously. This is because we
need to know the number of words at the beginning of the program to allocate the correct width/height to the output
image, but if an input file is provided, its contents are streamed rather than buffered - this is to allow potentially
massive inputs.

In addition to the ```.svg``` files output, a ```.json``` metadata file will also be automatically written for each
program execution. This file contains a mapping from filename --> word used to create that output.

## Examples

#### The base website:

[![Base Web UI](https://github.com/CalumD/WordBrush/blob/master/examples/base_website.png)]()

#### The options dropdown:

[![Search Options](https://github.com/CalumD/WordBrush/blob/master/examples/search_options.png)]()

#### Previous searches dropdown:

[![Past Searches](https://github.com/CalumD/WordBrush/blob/master/examples/past_searches.png)]()

#### Example of output with colour tracing enabled:

[![A Search With colour tracing enables](https://github.com/CalumD/WordBrush/blob/master/examples/coloured_output.png)]()

#### Example of output with the keyboards drawn, and hovering over the WB logo to see the words used to create the lines:

[![Keyboard background and hovering over WB logo to see words](https://github.com/CalumD/WordBrush/blob/master/examples/output_with_keeb_and_words.png)]()
