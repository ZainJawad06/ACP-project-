Act as an expert C software engineer. Design and implement a complete, menu-driven 2D Graphics Editor in C that uses a 2D character array as a drawing canvas. 

### 1. Canvas Specifications
- Define a fixed-size 2D character array (e.g., 20 rows by 40 columns) as the global or main canvas.
- Initialize the canvas so it is entirely filled with the character '_' (underscore).
- All graphical objects must be drawn/rendered onto this canvas using the '*' (asterisk) character.

### 2. Object Management & Data Structures
- Create a structure (`Shape`) to keep track of added objects so they can be modified or deleted. 
- Each shape should have a unique ID, a type (LINE, RECTANGLE, TRIANGLE, CIRCLE), and its corresponding geometric parameters (e.g., coordinates, radius, width, height).
- Maintain an array of these structures to act as the "object list" (up to a maximum of 10-20 shapes).

### 3. Core Geometric Functions
Implement the following rendering functions using standard discrete coordinate algorithms (handle rounding to integers gracefully):
- `void draw_line(int x1, int y1, int x2, int y2)` -> (Tip: Use Bresenham's or a simple DDA line algorithm)
- `void draw_rectangle(int x, int y, int width, int height)` -> Draws the outline of a rectangle.
- `void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3)` -> Draws a triangle by connecting three line vertices.
- `void draw_circle(int xc, int yc, int radius)` -> (Tip: Use Bresenham’s/Midpoint circle algorithm)

### 4. Editor Feature Requirements
The program must be fully menu-driven in the console, operating in a loop with the following options:
1. **Add Object:** Prompt the user for the shape type and its required coordinates/dimensions. Validate that inputs are within canvas bounds, add it to the object tracking list, and refresh the canvas.
2. **Delete Object:** Display the current list of active objects with their IDs. Prompt the user for an ID, remove it from the tracking list, and refresh the canvas.
3. **Modify Object:** Display active objects, prompt for an ID, and allow the user to input new coordinates/dimensions for that specific shape.
4. **Display Picture:** Clear the console and print the current state of the 2D canvas row by row.

### 5. Canvas Refresh Logic
Because objects can overlap, "deleting" or "modifying" an object cannot be done by just erasing pixels. Implement a `refresh_canvas()` function that:
1. Resets the entire 2D array back to underscores ('_').
2. Loops through the active object tracking list and calls the respective `draw` functions to redraw all remaining shapes from scratch.

### 6. Code Quality
- Standard I/O only (`stdio.h`, `stdlib.h`, `math.h`).
- Include robust boundary checking so drawing outside the 2D array bounds does not cause segmentation faults (ignore or clip pixels out of bounds).
- Provide clean, well-commented code with an intuitive user interface.
