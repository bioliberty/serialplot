# 

TODO
remove docs/ from git repo

## serialplot fork ideas:
 - [ ] settings creator tool (gui) that lets you change
 - [ ] plot tabe at bottom:  split into two sets of columns so you can see more at one time. 
 - [ ] window cursors:  overlay ac noise, dc noise, slope (linear regression best effort, 1st order), min, max, avg...
  - [ ] ^ later, try 2nd and 3rd order, other types of fit...)
 - [ ] two or more plot groups.  each has a channel selector. 
 - [ ] change to BIN pack - allow more suited type for us. 
 - [ ] build on MAC. 
 - [ ] side panel to show ALL data coming in 
 - [ ] side panel to pick up SWO comms? 
 - [ ] somewhere to keep ideas ?  MD FILE?  docs folders
 - [ ] channel options - add INTERPOLATION (to gain etc)
 - [ ] channel options - add units? 
 - [ ] x axis - be clearer what scale / units are
 - [ ] x-y:  zoom in. 
 - [ ] have a "digitial channel" type (eg for valve pack) - seperate grid, put markers, view in HEX, have a BITFIELD (name + bit,  name appears on y axis)
 - [ ] command structure setup - plug in (wireshark esque)- make one specific for lg01 comms protocol (use the repo json!!)
 - [ ] some type of built in settings or autoloader button click to load different lifehub #xx packs?
 - [ ] somehow allow an external hook into an anaylser (eg my python progs).  OR just an easier/better csv export. 

 ## Complex Frame
  - float is 8B here!  make float 4B (call it stm float?) and drop double (or check and make 8B?)
  - payload size is number of BYTES for the SAMPLES total for all chans.  
    ie if its 2ch u16, and payload = 4. then its 1 SET of 2ch u16 samples, 4B total. 
    MIGHT REWORK THIS TO BE CLEARER.
    MAYBE - change to "sets per frame". so if all u16, 4ch: 1 sets per frame will be frame size of 8B, 2 = 16B etc...
  - when setting up individual channel format, each channel can be set to the type (u8, 16, float..)
      AND, add another type called "pad", with an entry box. can use to specify its x B and drop them
      so first "channel" in lg01 comms msg is pad 5B, to drop the frame. 
      
