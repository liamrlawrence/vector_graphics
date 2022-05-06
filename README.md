# Vector Graphics

# Colors
### Explicit Color Values
When a command needs an explicit color value (such as [BACKGROUND]), the format can either be 

`(f, f, f) where f is a float between 0.0 - 1.0`

or

`(b, b, b) where b is an integer between 0 - 255`

### Name of colors
Most commands take the uppercase name of the color as the input. The list of valid colors are -
```
BLACK  = {0x00, 0x00, 0x00}
SILVER = {0xC3, 0xC3, 0xC3}
WHITE  = {0xFF, 0xFF, 0xFF}
RED    = {0xFF, 0x00, 0x00}
BLUE   = {0x00, 0x00, 0xFF}
GREEN  = {0x00, 0xFF, 0x00}
YELLOW = {0xFF, 0xFF, 0x00}
```


# Commands

When calling a command, it should be on its own line in the format `[COMAND]`.

### Grouping commands
Multiple of the same command can be run together by having them be on consecutive lines, they can be separated by comments but ***never*** a blank line.
```hs
[COMMAND]
(x1, y1) param_a=1 param_b=2
(x2, y2) param_a=3 param_b=4
# You can insert comments between commands
(x3, y3) param_a=5 param_b=6

# ^^ But you CANNOT have a blank line! ^^
# VV--------- This will fail! ---------VV
(x4, y4) param_a=6 paramb_7
```

### [ALPHA]

Sets the transparency level of the background on a scale of (0.0 - 1.0), where 0 is 100% transparent and 1 is opaque.

The alpha level will be updated the next time the [BACKGROUND] command is run.

Arguments:
`(float alpha_level)`

```hs
# Set the alpha level to 75% (25% transparency)
[ALPHA]
(0.75)
```


### [BACKGROUND]

Sets the background color and alpha levels.

Arguments:
`(red color, green color, blue color)`

```hs
# Set the background to Red
[BACKGROUND]
(255, 0, 0)
```



### [OFFSET]

Offset the origin by (x, y) - keep in mind the top left is (0, 0) with X increasing to the right and Y increasing going down.

Arguments:
`(int X offset, int Y offset)`

```hs
# Put the origin in the center
# (width and height of the image is 2560x1440)
[OFFSET]
(1280, 720)
```


### [SCALE]

Scales the values of everything (except [OFFSET]) by (x, y) - This should be set to (1.0, 1.0) for most cases.

Arguments:
`(float X Scale, float Y Scale)`

```hs
# Scale the X-axis by 0.5 and the Y-axis by 2.0
[SCALE]
(0.5, 2.0)
```



### [TEXT]

Write text to the image - Be aware that the current limit is 128 characters per [TEXT] command.

The [TEXT] command also requires a color (using the uppercase name) and a font size.

Arguments:
`(x, y) "Text to Display!" color=COLOR font_size=n`

```hs
# Write "Hello, World!" in blue at coordinates (10, 50)
[TEXT]
(10, 50) "Hello, World!" color=BLUE font_size=18
```



### [STATION] / [STOP]

These two commands are identical, and are named separately for organizational purposes, they will be referred to as [ST] for this section.

The [ST] command draws a circle with an outline at a specific location

Arguments:
`(x, y) radius=r line_width=lw line_color=COLOR fill_color=COLOR`

```hs
# Draw a yellow dot with black border at (100, 350)
[STATION]
(100, 350) radius=26 line_width=18 line_color=BLACK 
fill_color=YELLOW
```



### [LINE]

Draws a vector between two points.

Arguments:
`<(x1, y1), (x2, y2)> width=w color=COLOR`

```hs
# Draw a red box
[Line]
<(0, 0), (-200, 0)> width=20 color=RED
<(-200, 0), (-200, -200)> width=20 color=RED
<(-200, -200), (0, -200)> width=20 color=RED
<(0, -200), (0, 0)> width=20 color=RED
```



## The label argument

There is an optional set of arguments available for [STOP], [STATION], and [LINE] that allow you to immediately create a label for the object you're drawing. These arguments ***must*** come at the end of the command.

Please note, a label's color is always `BLACK`.

Arguments:
`label="Label Name" (label x, label y) font_size=n`

```hs
# Create a Blue station with a label below it
[STATION]
(-50, 275) radius=26 line_width=18 line_color=BLACK fill_color=BLUE label="Blue Station" (-105, 335) font_size=16
```

A label can be turned off by setting `font_size=0` or `label=""`.



## Known Issues:
Running the program in a Docker container after compiling with `-O3` will cause a segfault, unknown cause.
