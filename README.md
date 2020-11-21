## **aLEDp**: Aria's LED Protocol specification

This is for the hopefully-temporary aLEDp, which is *A*ria's *LED* *p*rotocol.  It will be used for communication between host devices and devices controlling RGB LEDs, and is very loosely based off of G-code.

## Overall Notes

- All numbers support zero-padding; it will be stripped down to its base value.
- Variables in bold are required unless otherwise noted.
- Variables in italics are optional unless otherwise noted.

---

## Variable Types

### Colors
This is a color specified by one or more of the following color specifications.  Most commands that involve color involve setting the values for one of these:

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

Translations 

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
* If S is greater than E, a warning response code is generated and the values treated in reverse (E to S).  This can lead to unexpected results for any commands with gradual changes to multiple LEDs.

## Commands

### **A0**
#### *Device Information*

Return information about the device and its LEDs.

---

### **A1**
#### *Set Color*

Set the color of a specific LED or group of LEDs.

***Variables***  
[Color Specification](#markdown-header-color-specification): RGB/HSV/HTML  
[LED Groups](#markdown-header-led-groups): S to E

***Notes***
* If settings from multiple color specifications are specified, they will be applied in the order listed above.
* If no color is specified, the default color ([A997](#markdown-header-A997)) is used.  

***Examples***  
`A1 X00FF00 ; Set all LEDs to green`  
`A1 S2 E12 R255 G192 B203 ; Set LEDs 2 thru 12 to pink`  
`A1 E4 H0 S255 V255; Set first five LEDs (0-4) to red`  

---
### **A2**
#### *Modify Color*

Modify the existing color by math translation of the existing colors on specific LEDs.  This differs from [A1](#markdown-header-a1) in modifying existing colors, rather than setting new colors with no regard for the previous settings.

***Variables***  
[Color Translation](#markdown-header-color-translation): RGB or HSV  
[LED Groups](#markdown-header-led-groups): S to E

***Notes***   
* If settings from multiple color specifications are specified, they will be applied in the order listed above.
* Attempting to specify an HTML color code via X will throw E03.

***Examples***  
`A2 V-100 ; Dim all of the LEDs`  

--- 
### **A997**
#### *Set Default Color*  
Set the default color, similar to setting an LED's color ([A1](#markdown-header-a1)).

***Variables***  
[Color Translation](#markdown-header-color-translation): RGB/HSV/HTML

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

**R**: Standard responses to commands  
**W**: Warnings; these normally should not stop operation  
**E**: Errors; something went wrong that should stop operation.

For ease of parsing, all codes will work across all prefixes; however, some combinations will have no obvious use.  (For example, a code of R01 would be an out of bounds OK, which makes no logical sense.)

In general, warnings are recoverable; errors aren't.

Each response can also have an accompanying text string after with more information.

### *R00* ###
This is the standard "OK" response from most immediate commands.  
### *E01* ###
Syntax error - more than likely, erroneous commands.
### *W02*/*E02* ###
Out of bounds.
### *E03* ###
Type mismatch (usually specifying HTML values in a translation)  