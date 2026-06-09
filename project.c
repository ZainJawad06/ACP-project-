#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROWS 20
#define COLS 60
#define MAX_SHAPES 100

typedef enum {
    SHAPE_CIRCLE,
    SHAPE_RECTANGLE,
    SHAPE_LINE,
    SHAPE_TRIANGLE
} ShapeType;

typedef struct {
    int xc, yc;
    int r;
} CircleParams;

typedef struct {
    int x1, y1;
    int x2, y2;
} RectParams;

typedef struct {
    int x1, y1;
    int x2, y2;
} LineParams;

typedef struct {
    int x1, y1;
    int x2, y2;
    int x3, y3;
} TriParams;

typedef struct {
    int active;
    ShapeType type;
    union {
        CircleParams circle;
        RectParams rect;
        LineParams line;
        TriParams tri;
    } params;
} Shape;

char canvas[ROWS][COLS];
Shape shapes[MAX_SHAPES];

void clear_canvas(char canvas[ROWS][COLS]);
void draw_pixel(char canvas[ROWS][COLS], int x, int y);
void draw_line(char canvas[ROWS][COLS], int x0, int y0, int x1, int y1);
void draw_rectangle(char canvas[ROWS][COLS], int x1, int y1, int x2, int y2);
void draw_circle(char canvas[ROWS][COLS], int xc, int yc, int r);
void draw_triangle(char canvas[ROWS][COLS], int x1, int y1, int x2, int y2, int x3, int y3);
void render_canvas(char canvas[ROWS][COLS], Shape shapes[], int max_shapes);
void display_canvas(char canvas[ROWS][COLS]);
void list_shapes(Shape shapes[], int max_shapes);
void add_shape(Shape shapes[], int max_shapes);
void delete_shape(Shape shapes[], int max_shapes);
void modify_shape(Shape shapes[], int max_shapes);
int get_int_input(const char* prompt, int min_val, int max_val);

int main() {
    for (int i = 0; i < MAX_SHAPES; i++) {
        shapes[i].active = 0;
    }

    printf("=====================================================\n");
    printf("        Welcome to the 2D character Graphics Editor!\n");
    printf("=====================================================\n");
    printf("Instructions:\n");
    printf("- The grid is %d columns (X: 0 to %d) by %d rows (Y: 0 to %d).\n", COLS, COLS - 1, ROWS, ROWS - 1);
    printf("- Shape rendering uses standard shapes drawn with '*' on a background of '_'.\n");
    printf("- Circle drawings include aspect ratio correction for text terminals.\n");
    printf("=====================================================\n\n");

    int choice = 0;
    while (1) {
        printf("\n--- Main Menu ---\n");
        printf("1. Add a shape to the scene\n");
        printf("2. Delete a shape from the scene\n");
        printf("3. Modify a shape in the scene\n");
        printf("4. Display canvas (Render scene)\n");
        printf("5. List active shapes\n");
        printf("6. Clear all shapes\n");
        printf("7. Exit\n");
        
        choice = get_int_input("Enter your choice (1-7): ", 1, 7);

        switch (choice) {
            case 1:
                add_shape(shapes, MAX_SHAPES);
                break;
            case 2:
                delete_shape(shapes, MAX_SHAPES);
                break;
            case 3:
                modify_shape(shapes, MAX_SHAPES);
                break;
            case 4:
                render_canvas(canvas, shapes, MAX_SHAPES);
                display_canvas(canvas);
                break;
            case 5:
                list_shapes(shapes, MAX_SHAPES);
                break;
            case 6:
                for (int i = 0; i < MAX_SHAPES; i++) {
                    shapes[i].active = 0;
                }
                printf("All shapes cleared from the scene.\n");
                break;
            case 7:
                printf("Exiting 2D Graphics Editor. Goodbye!\n");
                return 0;
        }
    }

    return 0;
}

void clear_canvas(char canvas[ROWS][COLS]) {
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            canvas[y][x] = '_';
        }
    }
}

void draw_pixel(char canvas[ROWS][COLS], int x, int y) {
    if (x >= 0 && x < COLS && y >= 0 && y < ROWS) {
        canvas[y][x] = '*';
    }
}

void draw_line(char canvas[ROWS][COLS], int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    for (;;) {
        draw_pixel(canvas, x0, y0);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void draw_rectangle(char canvas[ROWS][COLS], int x1, int y1, int x2, int y2) {
    draw_line(canvas, x1, y1, x2, y1);
    draw_line(canvas, x1, y2, x2, y2);
    draw_line(canvas, x1, y1, x1, y2);
    draw_line(canvas, x2, y1, x2, y2);
}

void draw_circle(char canvas[ROWS][COLS], int xc, int yc, int r) {
    int y_start = yc - r;
    int y_end = yc + r;
    int x_start = xc - (int)(r * 2.2);
    int x_end = xc + (int)(r * 2.2);

    if (y_start < 0) y_start = 0;
    if (y_end >= ROWS) y_end = ROWS - 1;
    if (x_start < 0) x_start = 0;
    if (x_end >= COLS) x_end = COLS - 1;

    for (int y = y_start; y <= y_end; y++) {
        for (int x = x_start; x <= x_end; x++) {
            double dx = x - xc;
            double dy = (y - yc) * 2.0; 
            double dist = sqrt(dx * dx + dy * dy);
            if (fabs(dist - r) < 0.85) {
                draw_pixel(canvas, x, y);
            }
        }
    }
}

void draw_triangle(char canvas[ROWS][COLS], int x1, int y1, int x2, int y2, int x3, int y3) {
    draw_line(canvas, x1, y1, x2, y2);
    draw_line(canvas, x2, y2, x3, y3);
    draw_line(canvas, x3, y3, x1, y1);
}

void render_canvas(char canvas[ROWS][COLS], Shape shapes[], int max_shapes) {
    clear_canvas(canvas);
    for (int i = 0; i < max_shapes; i++) {
        if (shapes[i].active) {
            switch (shapes[i].type) {
                case SHAPE_CIRCLE:
                    draw_circle(canvas, shapes[i].params.circle.xc, shapes[i].params.circle.yc, shapes[i].params.circle.r);
                    break;
                case SHAPE_RECTANGLE:
                    draw_rectangle(canvas, shapes[i].params.rect.x1, shapes[i].params.rect.y1, shapes[i].params.rect.x2, shapes[i].params.rect.y2);
                    break;
                case SHAPE_LINE:
                    draw_line(canvas, shapes[i].params.line.x1, shapes[i].params.line.y1, shapes[i].params.line.x2, shapes[i].params.line.y2);
                    break;
                case SHAPE_TRIANGLE:
                    draw_triangle(canvas, shapes[i].params.tri.x1, shapes[i].params.tri.y1, shapes[i].params.tri.x2, shapes[i].params.tri.y2, shapes[i].params.tri.x3, shapes[i].params.tri.y3);
                    break;
            }
        }
    }
}

void display_canvas(char canvas[ROWS][COLS]) {
    printf("\n");
    printf("     ");
    for (int x = 0; x < COLS; x++) {
        if (x % 10 == 0 || x == 0) {
            printf("%d", x / 10);
        } else {
            printf(" ");
        }
    }
    printf("\n");

    printf("     ");
    for (int x = 0; x < COLS; x++) {
        printf("%d", x % 10);
    }
    printf("\n");

    printf("   +");
    for (int x = 0; x < COLS; x++) {
        printf("-");
    }
    printf("+\n");

    for (int y = 0; y < ROWS; y++) {
        printf("%02d |", y);
        for (int x = 0; x < COLS; x++) {
            printf("%c", canvas[y][x]);
        }
        printf("| %02d\n", y);
    }

    printf("   +");
    for (int x = 0; x < COLS; x++) {
        printf("-");
    }
    printf("+\n");

    printf("     ");
    for (int x = 0; x < COLS; x++) {
        printf("%d", x % 10);
    }
    printf("\n\n");
}

void list_shapes(Shape shapes[], int max_shapes) {
    int count = 0;
    printf("\nActive Shapes:\n");
    printf("-----------------------------------------------------\n");
    for (int i = 0; i < max_shapes; i++) {
        if (shapes[i].active) {
            count++;
            printf("[%d] ", i + 1);
            switch (shapes[i].type) {
                case SHAPE_CIRCLE:
                    printf("Circle: Center=(%d, %d), Radius=%d\n", 
                           shapes[i].params.circle.xc, shapes[i].params.circle.yc, shapes[i].params.circle.r);
                    break;
                case SHAPE_RECTANGLE:
                    printf("Rectangle: Top-Left=(%d, %d), Bottom-Right=(%d, %d)\n", 
                           shapes[i].params.rect.x1, shapes[i].params.rect.y1, 
                           shapes[i].params.rect.x2, shapes[i].params.rect.y2);
                    break;
                case SHAPE_LINE:
                    printf("Line: Start=(%d, %d), End=(%d, %d)\n", 
                           shapes[i].params.line.x1, shapes[i].params.line.y1, 
                           shapes[i].params.line.x2, shapes[i].params.line.y2);
                    break;
                case SHAPE_TRIANGLE:
                    printf("Triangle: P1=(%d, %d), P2=(%d, %d), P3=(%d, %d)\n", 
                           shapes[i].params.tri.x1, shapes[i].params.tri.y1, 
                           shapes[i].params.tri.x2, shapes[i].params.tri.y2, 
                           shapes[i].params.tri.x3, shapes[i].params.tri.y3);
                    break;
            }
        }
    }
    if (count == 0) {
        printf("(No active shapes in the scene)\n");
    }
    printf("-----------------------------------------------------\n");
}

void add_shape(Shape shapes[], int max_shapes) {
    int index = -1;
    for (int i = 0; i < max_shapes; i++) {
        if (!shapes[i].active) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Error: Canvas is full! Please delete some shapes first.\n");
        return;
    }

    printf("\nChoose Shape to Add:\n");
    printf("1. Circle\n");
    printf("2. Rectangle\n");
    printf("3. Line\n");
    printf("4. Triangle\n");
    int choice = get_int_input("Enter choice (1-4): ", 1, 4);

    shapes[index].active = 1;
    switch (choice) {
        case 1:
            shapes[index].type = SHAPE_CIRCLE;
            shapes[index].params.circle.xc = get_int_input("Enter Center X coordinate: ", -50, 150);
            shapes[index].params.circle.yc = get_int_input("Enter Center Y coordinate: ", -50, 150);
            shapes[index].params.circle.r = get_int_input("Enter Radius (>=1): ", 1, 100);
            break;
        case 2:
            shapes[index].type = SHAPE_RECTANGLE;
            shapes[index].params.rect.x1 = get_int_input("Enter Top-Left X coordinate: ", -50, 150);
            shapes[index].params.rect.y1 = get_int_input("Enter Top-Left Y coordinate: ", -50, 150);
            shapes[index].params.rect.x2 = get_int_input("Enter Bottom-Right X coordinate: ", -50, 150);
            shapes[index].params.rect.y2 = get_int_input("Enter Bottom-Right Y coordinate: ", -50, 150);
            break;
        case 3:
            shapes[index].type = SHAPE_LINE;
            shapes[index].params.line.x1 = get_int_input("Enter Start X coordinate: ", -50, 150);
            shapes[index].params.line.y1 = get_int_input("Enter Start Y coordinate: ", -50, 150);
            shapes[index].params.line.x2 = get_int_input("Enter End X coordinate: ", -50, 150);
            shapes[index].params.line.y2 = get_int_input("Enter End Y coordinate: ", -50, 150);
            break;
        case 4:
            shapes[index].type = SHAPE_TRIANGLE;
            shapes[index].params.tri.x1 = get_int_input("Enter P1 X coordinate: ", -50, 150);
            shapes[index].params.tri.y1 = get_int_input("Enter P1 Y coordinate: ", -50, 150);
            shapes[index].params.tri.x2 = get_int_input("Enter P2 X coordinate: ", -50, 150);
            shapes[index].params.tri.y2 = get_int_input("Enter P2 Y coordinate: ", -50, 150);
            shapes[index].params.tri.x3 = get_int_input("Enter P3 X coordinate: ", -50, 150);
            shapes[index].params.tri.y3 = get_int_input("Enter P3 Y coordinate: ", -50, 150);
            break;
    }
    printf("Shape added successfully!\n");
}

void delete_shape(Shape shapes[], int max_shapes) {
    list_shapes(shapes, max_shapes);
    
    int any_active = 0;
    for (int i = 0; i < max_shapes; i++) {
        if (shapes[i].active) {
            any_active = 1;
            break;
        }
    }
    if (!any_active) return;

    int choice = get_int_input("Enter shape number to delete (0 to cancel): ", 0, max_shapes);
    if (choice == 0) return;

    if (shapes[choice - 1].active) {
        shapes[choice - 1].active = 0;
        printf("Shape [%d] deleted successfully!\n", choice);
    } else {
        printf("No active shape exists with that number.\n");
    }
}

void modify_shape(Shape shapes[], int max_shapes) {
    list_shapes(shapes, max_shapes);

    int any_active = 0;
    for (int i = 0; i < max_shapes; i++) {
        if (shapes[i].active) {
            any_active = 1;
            break;
        }
    }
    if (!any_active) return;

    int choice = get_int_input("Enter shape number to modify (0 to cancel): ", 0, max_shapes);
    if (choice == 0) return;

    if (!shapes[choice - 1].active) {
        printf("No active shape exists with that number.\n");
        return;
    }

    Shape *s = &shapes[choice - 1];
    printf("\nModifying Shape [%d]:\n", choice);

    switch (s->type) {
        case SHAPE_CIRCLE:
            s->params.circle.xc = get_int_input("Enter new Center X coordinate: ", -50, 150);
            s->params.circle.yc = get_int_input("Enter new Center Y coordinate: ", -50, 150);
            s->params.circle.r = get_int_input("Enter new Radius (>=1): ", 1, 100);
            break;
        case SHAPE_RECTANGLE:
            s->params.rect.x1 = get_int_input("Enter new Top-Left X coordinate: ", -50, 150);
            s->params.rect.y1 = get_int_input("Enter new Top-Left Y coordinate: ", -50, 150);
            s->params.rect.x2 = get_int_input("Enter new Bottom-Right X coordinate: ", -50, 150);
            s->params.rect.y2 = get_int_input("Enter new Bottom-Right Y coordinate: ", -50, 150);
            break;
        case SHAPE_LINE:
            s->params.line.x1 = get_int_input("Enter new Start X coordinate: ", -50, 150);
            s->params.line.y1 = get_int_input("Enter new Start Y coordinate: ", -50, 150);
            s->params.line.x2 = get_int_input("Enter new End X coordinate: ", -50, 150);
            s->params.line.y2 = get_int_input("Enter new End Y coordinate: ", -50, 150);
            break;
        case SHAPE_TRIANGLE:
            s->params.tri.x1 = get_int_input("Enter new P1 X coordinate: ", -50, 150);
            s->params.tri.y1 = get_int_input("Enter new P1 Y coordinate: ", -50, 150);
            s->params.tri.x2 = get_int_input("Enter new P2 X coordinate: ", -50, 150);
            s->params.tri.y2 = get_int_input("Enter new P2 Y coordinate: ", -50, 150);
            s->params.tri.x3 = get_int_input("Enter new P3 X coordinate: ", -50, 150);
            s->params.tri.y3 = get_int_input("Enter new P3 Y coordinate: ", -50, 150);
            break;
    }
    printf("Shape [%d] updated successfully!\n", choice);
}


int get_int_input(const char* prompt, int min_val, int max_val) {
    int val;
    char term;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d%c", &val, &term) != 2 || term != '\n') {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a valid integer.\n");
        } else if (val < min_val || val > max_val) {
            printf("Value out of range [%d, %d]. Try again.\n", min_val, max_val);
        } else {
            return val;
        }
    }
}

