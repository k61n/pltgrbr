# PlotGrabber

PG is an open source software that allows to extract data from plots, e.g. those that are used in scientific articles, both cartesian and polar.

The software is written in C++ using Qt 5.12.1.

Current features
================

* Opens images through filedialog, drag and drop as well as from clipboard.
* Supports jpg, png, bmp, tif and other formats.
* Smooth image scaling and resizing.
* Allows to manually set and adjust points, which coordinates are arranged into a table sorted in ascending order by argument.
* Saves table with coordinates as an ASCII file, or copies it to clipboard.
* Allows to set up cartesian or polar axes, in order to recalculate coordinates of points in users' values. Supports logarithm scaling for cartesian plots. If axes are not set, points coordinates are shown in image coordinates.
* Simple axes adjustment and instant recalculation of coordinates.
* A magnifier widget that shows area around cursor with 5x scaling.

Features to implement
=====================

* Some basic image adjustment: rotation, brightness/contrast.
* Automatic scanning of plots, in order to extract data from lines.
* Analysis of extracted curves, such as integration/area calculation, extraction of parameters of solar cells or diodes for IV curves, and the possibility to make users' plugins for that.
