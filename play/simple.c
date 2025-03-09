// Simple program to test function calls

// Add function
int add(int a, int b) {
    return a + b;
}

// Main function
int main() {
    // Test add function
    print(add(5, 3));  // Should print 8
    
    // Test for loop
    int sum = 0;
    for (int i = 1; i <= 5; i = i + 1) {
        sum = sum + i;
    }
    print(sum);  // Should print 15
    
    return 0;
} 