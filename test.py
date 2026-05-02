import subprocess
import pytest


def run_program(input_string):
    """Run the compiled program with the given input and return its output."""
    result = subprocess.run(
        ["./main"],
        input=input_string,
        capture_output=True,
        text=True
    )
    return result.stdout + result.stderr, result.returncode


class TestFraction:
    """Test suite for the Fraction class."""

    def test_default_constructor(self):
        """Test that the default constructor is called."""
        output, _ = run_program("3 4 5 6 7 8")
        assert "I am in the default constructor" in output, \
            "Default constructor should be called"

    def test_custom_constructor(self):
        """Test that the custom constructor is called."""
        output, _ = run_program("3 4 5 6 7 8")
        assert "I am in the custom constructor" in output, \
            "Custom constructor should be called"

    def test_custom_constructor_multiple_calls(self):
        """Test that the custom constructor is called multiple times."""
        output, _ = run_program("3 4 5 6 7 8")
        count = output.count("I am in the custom constructor")
        assert count >= 3, \
            f"Custom constructor should be called at least 3 times, found {count}"

    def test_copy_constructor(self):
        """Test that the copy constructor is called."""
        output, _ = run_program("3 4 5 6 7 8")
        assert "I am in the copy constructor" in output, \
            "Copy constructor should be called"

    def test_move_constructor(self):
        """Test that the move constructor is called."""
        output, _ = run_program("3 4 5 6 7 8")
        assert "I am in the move constructor" in output, \
            "Move constructor should be called"

    def test_assignment_operator_lvalue(self):
        """Test that the assignment operator with lvalue is called."""
        output, _ = run_program("3 4 5 6 7 8")
        assert "I am in the assignment operator (lvalue)" in output, \
            "Assignment operator (lvalue) should be called"

    def test_assignment_operator_rvalue(self):
        """Test that the assignment operator with rvalue is called."""
        output, _ = run_program("3 4 5 6 7 8")
        assert "I am in the assignment operator (rvalue)" in output, \
            "Assignment operator (rvalue) should be called"

    def test_plus_operator(self):
        """Test that the plus operator is called."""
        output, _ = run_program("3 4 5 6 7 8")
        assert "I am in the plus operator" in output, \
            "Plus operator should be called"

    def test_multiplication_operator(self):
        """Test that the multiplication operator is called."""
        output, _ = run_program("3 4 5 6 7 8")
        assert "I am in the multiplication operator" in output, \
            "Multiplication operator should be called"

    def test_stream_operator(self):
        """Test that the << operator is called."""
        output, _ = run_program("3 4 5 6 7 8")
        assert "I am in the << operator" in output, \
            "Stream insertion operator should be called"

    def test_stream_operator_multiple_calls(self):
        """Test that the << operator is called multiple times."""
        output, _ = run_program("3 4 5 6 7 8")
        count = output.count("I am in the << operator")
        assert count >= 4, \
            f"Stream operator should be called at least 4 times, found {count}"

    def test_convert_to_decimal(self):
        """Test that convertToDecimal is called."""
        output, _ = run_program("3 4 5 6 7 8")
        assert "I am in the convertToDecimal" in output, \
            "convertToDecimal should be called"

    def test_destructor(self):
        """Test that the destructor is called."""
        output, _ = run_program("3 4 5 6 7 8")
        assert "I am in the destructor" in output, \
            "Destructor should be called"

    def test_fraction_output(self):
        """Test that fractions are printed correctly."""
        output, _ = run_program("3 4 5 6 7 8")
        assert "3/4" in output, \
            "Should display fraction 3/4"

    def test_decimal_output(self):
        """Test that decimal conversion works."""
        output, _ = run_program("3 4 5 6 7 8")
        assert "0.75" in output, \
            "Should convert 3/4 to 0.75"

    def test_addition_result(self):
        """Test that addition produces correct output format."""
        output, _ = run_program("3 4 5 6 7 8")
        # sum of 3/4 + 3/4 should produce a fraction output
        assert "/" in output, \
            "Addition should produce fraction with / symbol"

    def test_addition_computation(self):
        """Test that addition computes correctly."""
        output, _ = run_program("3 4 5 6 7 8")
        # sum of 3/4 + 3/4 = 6/8 + 6/8 = 12/8 = 24/16
        assert "24/16" in output, \
            "Sum of 3/4 + 3/4 should be 24/16"

    def test_multiplication_result(self):
        """Test that multiplication produces correct output format."""
        output, _ = run_program("3 4 5 6 7 8")
        # product of fractions should produce a fraction output
        assert "/" in output, \
            "Multiplication should produce fraction with / symbol"

    def test_multiplication_computation(self):
        """Test that multiplication computes correctly."""
        output, _ = run_program("3 4 5 6 7 8")
        # product of 3/4 * 5/6 = 15/24
        assert "15/24" in output, \
            "Product of 3/4 * 5/6 should be 15/24"

    def test_inheritance(self):
        """Test that Fraction can be used as Decimal pointer."""
        output, _ = run_program("3 4 5 6 7 8")
        assert "f2 as decimal" in output, \
            "Should demonstrate polymorphism with Decimal pointer"

    def test_multiple_destructors(self):
        """Test that destructor is called multiple times for multiple objects."""
        output, _ = run_program("3 4 5 6 7 8")
        destructor_count = output.count("I am in the destructor")
        assert destructor_count >= 6, \
            f"Destructor should be called at least 6 times, found {destructor_count}"

    def test_all_features_present(self):
        """Test that all required features are present in output."""
        output, _ = run_program("3 4 5 6 7 8")
        required_features = [
            "default constructor",
            "custom constructor",
            "copy constructor",
            "move constructor",
            "assignment operator (lvalue)",
            "assignment operator (rvalue)",
            "plus operator",
            "multiplication operator",
            "<< operator",
            "convertToDecimal",
            "destructor"
        ]

        for feature in required_features:
            assert feature in output, \
                f"Required feature '{feature}' not found in output"

    def test_memory_management(self):
        """Test that the program completes without crashes."""
        output, returncode = run_program("3 4 5 6 7 8")
        assert returncode == 0, \
            f"Program should exit successfully, got return code {returncode}"

    def test_divide_by_zero_first_fraction(self):
        """Test exception when first fraction has zero denominator."""
        output, returncode = run_program("3 0 5 6 7 8")
        assert "I am in the custom constructor" in output, \
            "Custom constructor should be called before exception"
        assert "Caught a runtime error!" in output, \
            "Should catch runtime error for divide by zero"
        assert returncode == 0, \
            "Program should exit cleanly after catching exception"

    def test_divide_by_zero_second_fraction(self):
        """Test exception when second fraction has zero denominator."""
        output, returncode = run_program("3 4 5 0 7 8")
        assert "I am in the custom constructor" in output, \
            "Custom constructor should be called"
        assert "Caught a runtime error!" in output, \
            "Should catch runtime error for divide by zero"
        assert returncode == 0, \
            "Program should exit cleanly after catching exception"

    def test_divide_by_zero_third_fraction(self):
        """Test exception when third fraction has zero denominator."""
        output, returncode = run_program("3 4 5 6 7 0")
        assert "I am in the custom constructor" in output, \
            "Custom constructor should be called"
        assert "Caught a runtime error!" in output, \
            "Should catch runtime error for divide by zero"
        assert returncode == 0, \
            "Program should exit cleanly after catching exception"

    def test_valid_fractions_no_exception(self):
        """Test that valid fractions don't throw exceptions."""
        output, _ = run_program("3 4 5 6 7 8")
        assert "Caught a runtime error!" not in output, \
            "Valid fractions should not throw exceptions"

    def test_different_fractions(self):
        """Test with different valid fraction values."""
        output, returncode = run_program("1 2 3 5 7 9")
        assert returncode == 0, \
            "Program should work with different valid fractions"
        assert "1/2" in output, \
            "Should display fraction 1/2"
        assert "0.5" in output, \
            "Should convert 1/2 to 0.5"

    def test_negative_numerators(self):
        """Test with negative numerators."""
        output, returncode = run_program("-3 4 5 6 7 8")
        assert returncode == 0, \
            "Program should work with negative numerators"
        assert "-3/4" in output, \
            "Should display fraction -3/4"

    def test_large_denominators(self):
        """Test with large denominators."""
        output, returncode = run_program("1 100 2 200 3 300")
        assert returncode == 0, \
            "Program should work with large denominators"
        assert "1/100" in output, \
            "Should display fraction 1/100"

    def test_assignment_operators_both_called(self):
        """Test that both assignment operators are called."""
        output, _ = run_program("3 4 5 6 7 8")
        assert "assignment operator (lvalue)" in output, \
            "lvalue assignment should be called"
        assert "assignment operator (rvalue)" in output, \
            "rvalue assignment should be called"

    def test_f2_f3_same_value(self):
        """Test that f2 and f3 have the same value (copy constructor)."""
        output, _ = run_program("3 4 5 6 7 8")
        # Both f2 and f3 should display 3/4
        assert "f2 =" in output, "Should have f2 output"
        assert "f3 =" in output, "Should have f3 output"
        # Check that 3/4 appears in the output (it's the value for both f2 and f3)
        assert "3/4" in output, "Should display 3/4"


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
