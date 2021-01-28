# SPSPlot

## Brief Summary
SPSPlot is an application for the Super-Enge SplitPole Spectrograph (SPS) at Florida State University. The goal of SPSPlot is to
calculate bending radii for ejecta of nuclear reactions within SPS, and then plot them. This helps the experimenter then determine what they are
seeing in their spectra. A rough "calibration" can then be obtained during the experimental run.

## Calculation details
Kinematics are described in Iliadis, "Nuclear Physics of Stars", Appendix C. This is the foundation of the entire reaction calculation. Bending radius
is then simply described by qvB = mv^2/r.

### Interpretation
Interpreting what the radial position means for your data is somewhat more complicated than it may seem. Typically, the focal plane detector must be moved (either physically moved or 
moved by a kinematic correction to data) to the kinematic focal plane, where the resolution for a given reaction of interest is optimized. Moving the detector effectively changes the range
of bending radii accepted by the detector, since the detector is of a fixed length and particles intersect the focal plane at approximately 45 degrees. This means that in principle the edges
of the detector (i.e. the range in bending radius) must be "calibrated" for every kinematic focal plane. In most cases this is non-trivial, and not practical for use while actively running an
experiment. This means that to effectively use this tool, it is best to have several known states in the spectrum with which the data can be oriented, along with a separate validation method.
Most often this separate validation is a spectrum taken of a calibration reaction which has a well known spectrum with which one can compare, with the same kinematic correction applied.

As a final note, if the kinematic correction to data method is employed, it may be found that not all states appear in the final spectrum when they are close to the edge of the detector. This is due to
the kinematic correction
1. requiring data from both planes, and since there is an angle at which particles are incident, edges become difficult
2. the kinematic correction alters the location of peaks to match what they would be at a different position, which can result in a radius range that isn't covered by the physical detector

## Dependancies
The only external dependance is the ROOT analysis software from CERN. The program was written and tested for ROOT 6.22, and as such, use will all other versions
is not guaranteed to be succesful.

## Notes
This program is a spiritual successor to the SpecPlot program written by D.W. Visser at Yale while the SPS was at Yale.

## Building the application and Running

To build the executable, simply run:
./make

in the /spsplot directory. Note that this assumes that ROOT is properly configured and root-config exists. Execution is then
./spsplot
again in /spsplot.

To make a clean build run:
./make clean
./make