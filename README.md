# WordBrush
This project takes ASCII input and draws the resultant line, as if you were to 'swipe-to-type' that word across a touchscreen keyboard.


# Notes to be removed before project completion:
 - In the case of words like 'was' and 'lol', which are fully contained in the extremities of the width of a 'keyboard',
 do we want to centre and trim the word in the file output?
    - This would result in more natural output of the word in written-form language
    - This would cause the output image width to be non-standard
 
-[ ] Give the line a coloured gradient from start to finish so we can track direction.
-[ ] Potentially centre and trim if the word is below some given width
-[ ] Make the multiple files output default to TRUE, and if given in CLI, make it the number of words to fit per line of the multi-svg.
-[ ] run through valgrind
-[ ] Multithreading if we have a large wordcount
    -[ ] Simple multithreading
    -[ ] Thread pool
-[ ] Putting multiple words into a single SVG with offsets                 <!-- TODO: CALUM NEXT TASK -->
-[ ] Being able to read input from a file in addition to CLI extra params  <!-- TOM NEXT TASK -->
