## **aLEDp**: Aria's LED Protocol specification

This is for the hopefully-temporary aLEDp, which is *A*ria's *LED* *p*rotocol.  It will be used for communication between host devices and devices controlling RGB LEDs, and is very loosely based off of G-code.

## Overall Notes

- All numbers support zero-padding; it will be stripped down to its base value.
- Variables in bold are required unless otherwise noted.
- Variables in italics are optional unless otherwise noted.

---

## Variable Types

### Color Specification
This is a color specified by one or more of the following color specifications.  Most commands that involve color involve setting the values for one of these.

**HTML**
* **X**: The six-character HTML value of the color

**RGB**
* **R**: The red value of the color 
* **G**: The green value of the color
* **B**: The blue value of the color 

**HSV**
* **H**: The hue value of the color
* **S**: The saturation value of the color
* **V**: The brightness value of the color

In general, HTML trumps RGB which trumps HSV; check the individual command for any possible differences.

**Examples**

`A1 X00FF00`  
`A1 R53 G132 B45`  
`A1 H0 S128 V255`  

### Color Translation
This is similar to [Color Specification](#markdown-header-color-specification), except that each value may be expressed as a positive or negative value.  If the sign is omitted, the change is assumed to be positive.  

In general, only RGB or HSV changes make sense; specifying HTML via X will either throw a warning, an error, or be assumed to instead mean setting the color instead of changing it; check the individual command for any possible differences.

**Examples**

`A2 R0 G+64 B+64`  
`A2 V+20`  

### LED Groups
You can specify the *start* (**S**) and *ending* (**E**) indexes of LEDs to be affected by a certain command.  

In general:  
* If only S is specified, *only* the LED at index S is affected.
* If only E is specified, *all* LEDs *up to and including* the LED at index E is affected.
* If neither S or E is specified, *all* LEDs are affected.

## Commands

### **A0**
#### *Device Information*

Return information about the device and its LEDs.

---

### **A1**
#### *Set Color*

Set the color of a specific LED or group of LEDs.

***Variables***
* *S*: The index of the first LED to change (default: 0)
* *E*: The index of the last LED to change (default: last LED index)
* *X*: The HTML hexadecimal value of the color (default: #000000)
* *R*: The red value of the color (default: 0)
* *G*: The green value of the color (default: 0)
* *B*: The blue value of the color (default: 0)
* *H*: The hue of the color
* *S*: The saturation of the color
* *V*: The brightness of the color

***Notes***
* If only S is specified without E, only the LED at index S will be affected.
* If S is more than E, a warning is thrown and 
* If settings from multiple color specifications are specified, they will be applied in the order listed above.
* If no color is specified, the default color ([A997](#markdown-header-A997)) is used.  

***Examples***

---
### **A2**
#### *Modify Color*

Modify the existing color by math translation of the existing colors on specific LEDs.  This differs from [A1](#markdown-header-a1) in modifying existing colors, rather than setting new colors with no regard for the previous settings.

***Variables***
Specify a [Color Translation](#markdown-header-color-translation).  

***Notes***
* If only S is specified without E, only the LED at index S will be affected.
* If S is more than E, W01 is thrown and S and E are reversed for execution.

***Examples***
* *X*: The HTML hexadecimal value of the color (default: #000000)

--- 
### **A997**
#### *Set Default Color*  
Set the default color, similar to setting an LED's color ([A1](#markdown-header-a1)).

***Variables***
* *R*: The change in the red value of the color (default: 0)
* *G*: The change in the green value of the color (default: 0)
* *B*: The change in the blue value of the color (default: 0)
* *H*: The change in the hue of the color (default: 0)
* *S*: The change in the saturation of the color (default: 0)
* *V*: The change in the brightness of the color (default: 0)

--- 
### **A998**
**Reset**

Reset all LEDs and configurable settings to their defaults, defined in each code's specification.

---
### **A999**
#### *Abort*

Complete abort and shutdown of all LED activity immediately.  This should entail some or all of the following:

* All LEDs shut off completely, if controllable in that fashion
* All LEDs set to the lowest possible brightness and power output
* All power to lights cut off


---

## Response Codes

The prefix of the response codes to each command gives the severity of the code in question.

