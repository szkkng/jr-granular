# JR-Granular
This is a real-time granular fx plugin made with JUCE and RNBO C++ export. 

<img width="1200" alt="jr-granular" src="https://github.com/szkkng/jr-granular/assets/61953352/4a852f7d-2ecb-4c1f-b614-006cf827b726">

This project is an example of using APVTS (juce::AudioProcessorValueTreeState) to link the exported code with custom JUCE UIs, 
so I will not add any new features to this plugin.

## Installing

Visit the [Releases](https://github.com/szkkng/jr-granular/releases/) page.
## Building
Clone the repository:
```
$ git clone https://github.com/szkkng/jr-granular.git --recursive
$ cd jr-granular
```
Open jr-granular.maxpat and export the RNBO patch with the following configuration options:
- Output Directory: `YOUR_PATH_TO_THIS_PROJECT/jr-granular/RnboExport`
- Export Name: `rnbo_granular.cpp`
<img width="1327" alt="Screenshot 2023-07-06 at 11 57 34" src="https://github.com/szkkng/jr-granular/assets/61953352/60f9dc8f-ca62-4141-b540-a644058185f2">

Build with CMake:
```
$ cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
$ cmake --build build --config Release
```

## UI Manual
- value changes: dragging or arrow keys
- fine mode: shift + dragging or shift + arrow keys
- edit mode: 0-9
- undo: [mac] cmd + z, [win] ctrl + z
- redo: [mac] cmd + shift + z, [win] ctrl + shift + z
- reset: double click

## Links
- Demo: [JR-Granular - JUCE & RNBO C++ Export](https://twitter.com/kng_dev/status/1587611842272055297?s=20&t=urpfRicQtbKBhkqoNniYBw)
- Tutorial: [JR-Granular - JUCE & RNBO C++ Export](https://kengo.dev/posts/jr-granular)

## References
- [Programming a Custom UI with JUCE](https://rnbo.cycling74.com/learn/programming-a-custom-ui-with-juce)
- [RNBO C++ API Reference](https://rnbo.cycling74.com/cpp)
- [Cycling74/gen-plugin-export](https://github.com/Cycling74/gen-plugin-export)

