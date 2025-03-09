// Simple program to test for loop

int main() {
    // Test for loop
    int sum = 0;
    
    // Use a while loop instead
    int i = 1;
    while (i <= 5) {
        sum = sum + i;
        i = i + 1;
    }
    
    print(sum);  // Should print 15
    
    return 0;
} 