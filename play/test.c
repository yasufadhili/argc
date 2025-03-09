// Simple test program for our compiler

int main() {
    int x = 10;
    print(x);
    
    int y = 20;
    print(y);
    
    int z = x + y;
    print(z);
    
    // Test if statement
    print(99);  // Debug marker
    if (z > 25) {
        print(1);
    } else {
        print(0);
    }
    
    // Test while loop
    print(88);  // Debug marker
    int i = 0;
    print(i);
    while (i < 5) {
        print(i);
        i = i + 1;
    }
    
    return 0;
} 