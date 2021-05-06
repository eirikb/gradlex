# gradlex

Proof of concept **gradlew** without JDK dependency.

## Install

* [Download from releases](../../releases/latest/download/gradlex)
* sh gradlex

### Goals

* Download gradle
* Install gradle to _.gradle_-folder
* Execute gradle

### Side goals

* Be small (smaller than _gradle-wrapper.jar_)
* Almost no dependencies
* Run on main OSes, distros and architectures

### Assumptions

* User has **sh** installed
* User has an internet connection

### Notes

* Shell can be of any kind (bash, zsh, fish) - but **sh** must be installed
    * For windows this means Mingw (git bash), Cygwin, WSL, WSL2 or something similar

### Rant

I've been in some discussion about this topic, where I came to
suggest [cosmopolitan](https://github.com/jart/cosmopolitan).  
This is a cool project, and it promises to do what what is needed, except it does not support different architectures.  
It says it does, but in reality it requires qemu to be installed.  
My POC above does not require that, and should work on ARM as well as x86 as long as the os/distro/arch is targeted for
build.  
Note one difference is that my POC does not work on pure windows, a shell with **sh** is required. Not sure how to fix.

After some discussions I realize that the current gradlew script-approach is fine, given the extent of **sh** being
available.  
The only problem is that **sh** can't download anything, download _something_, and then the rest would be possible.  
This POC is basically that: Make **sh** able to download and execute something, on any system.

### About distros

Need to build against several distros.  
In reality only need **glibc** + **libc** + any other **libc**-thing.  
For all I know building on **alpine** and **debian** should cover the most.  
Reason for this is to make the executables smaller.  
Statically built binary would not require a special distro, but could get up to as much as >700KB, while shared is 14KB.
So two binaries per architecture doesn't matter much.

### Weaknesses

* Very hacky
* Two/multi stage install
    * Note that if gradle had JDK bundled and was a single binary it would not need several stages
* Does not support **https**
    * Use cheksum instead, because **libcurl** and/or **openssl** makes the binary much larger
    * Note; the current checksum algorithm is a silly placeholder, easy to bypass
    * Possible to re-write
* Does not support Windows double-click or CMD
* Scalability: With more OS/distro/arch support the binary will grow
    * At the same time, as some systems become obsolete they can be removed, and/or special versions can be built

### Under the hood

How it actually works:

* Stage 1:
    * [makeself](https://makeself.io/) archive containing:
        * Simple program written in C using shared libraries for socket downloads a second stage installed over HTTP (
          not https)
        * Simple program built for multiple OS/distro/arch
        * Script executes every binary until one works
        * If one works stage 2 installer will be downloaded, else it quits
        * Stage 2 installer is executed
* Stage 2:
    * [makeself](https://makeself.io/) archive containing:
        * gradle (extracted) and a script
        * Script simply copies gradle to .gradle
