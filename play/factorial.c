// Factorial program to test function calls and recursion

// Factorial function
int factorial(int n) {
    if (n <= 1) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

// Main function
int main() {
    // Test factorial function
    print(factorial(5));  // Should print 120
    
    // Test for loop
    int sum = 0;
    for (int i = 1; i <= 10; i = i + 1) {
        sum = sum + i;
    }
    print(sum);  // Should print 55
    
    return 0;
} 