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

In general, only RGB or HSV changes make sense; specifying HTML via X will either throw a warning, an error, or be assumed to instead mean setting the color instead of changing it; check the individual command for any possible differences.

**Examples**

`A2 R0 G+64 B+64`  
`A2 V+20`  

### LED Groups
You can specify the *first* (**F**) and *last* (**L**) indexes of LEDs to be affected by a certain command.  

In general:  
* If only F is specified, *only* the LED at index S is affected.
* If only L is specified, *all* LEDs *up to and including* the LED at index L is affected.
* If neither F nor L is specified, *all* LEDs are affected.
* If F is greater than L, a warning is echoed and the values treated in reverse (L to F).  This can lead to unexpected results for any commands with gradual changes to multiple LEDs.

---

## System Commands

### **echo**
#### **Echo To Output**

Repeat whatever follows the echo command.

***Examples***  
`echo Hello! ; returns "echo: Hello!"`  

--- 

### **rem (;)**
#### **Comment**

Ignore anything following "rem" or ";", including in the middle of a command.

***Examples***  
`rem This is a comment`  
`A1 X00FF00 ; Set all LEDs to green`  
`A1 R255 G255 ; B255 - this will only set RG, since B is in the comment`  

---

## LED Commands

### **A0**
#### **Device Information**

Return information about the device and its LEDs in a series of echo responses.  

---

### **A1**
#### **Set Color**

Set the color of a specific LED or group of LEDs.

***Variables***  
[Color Specification](#markdown-header-color-specification): RGB/HSV/HTML  
[LED Groups](#markdown-header-led-groups): F to L

***Notes***
* If settings from multiple color specifications are specified, they will be applied in the order listed above.
* If no color is specified, the default color ([A997](#markdown-header-a997)) is used.  

***Examples***  
`A1 X00FF00 ; Set all LEDs to green`  
`A1 F2 L12 R255 G192 B203 ; Set LEDs 2 thru 12 to pink`  
`A1 E4 H0 S255 V255; Set first five LEDs (0-4) to red`  

---
### **A2**
#### **Modify Color**

Modify the existing color by math translation of the existing colors on specific LEDs.  This differs from [A1](#markdown-header-a1) in modifying existing colors, rather than setting new colors with no regard for the previous settings.

***Variables***  
[Color Translation](#markdown-header-color-translation): RGB or HSV  
[LED Groups](#markdown-header-led-groups): F to L

***Notes***   
* If settings from multiple color specifications are specified, they will be applied in the order listed above.
* Attempting to specify an HTML color code via X will fail with an error echo.
* If no color has been set yet, the default color ([A997](#markdown-header-a997)) will be used as a base.

***Examples***  
`A2 V-100 ; Dim all of the LEDs`  

--- 
### **A997**
#### **Set Default Color**  
Set the default color, similar to setting an LED's color ([A1](#markdown-header-a1)).

***Variables***  
[Color Translation](#markdown-header-color-translation): RGB/HSV/HTML

--- 
### **A998**
#### **Reset**

Reset all LEDs and configurable settings to their defaults, defined in each code's specification.

---
### **A999**
#### **Abort**

Complete abort and shutdown of all LED activity immediately.  This should entail some or all of the following:

* All LEDs shut off completely, if controllable in that fashion
* All LEDs set to the lowest possible brightness and power output
* All power to lights cut off

---

## Responses

Similar to RepRap and G-code, responses are pretty standardized:

### **ok**
The device is ready for its next command.
### **!!**
A fatal error has occurred; next steps will depend on the device in question, but usually require a reset of either the device, the client, or both.
### **wait**
This is sent as a keep-alive if no commands are received within a certain interval after ok.
### **busy:**
The device is busy and cannot receive or process commands from a client at this time.  Any known reasons for the busy status follow the colon in the response.