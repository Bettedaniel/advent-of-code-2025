#include<iostream>
#include<vector>
#include<cmath>

using namespace std;

typedef unsigned long long int ll;

int digitCount(ll number) {
	int digits = 0;
	while (number) {
		number /= 10;
		digits++;
	}
	return digits;
}

class PartOne {
	private:
		ll solve(ll start, ll end) {
			int startDigits = digitCount(start);
			int endDigits = digitCount(end);

			ll startLeftHalf = 0;
			ll startRightHalf = 0;
			ll startHalfPow = pow(10, startDigits / 2);

			ll endLeftHalf = 0;
			ll endRightHalf = 0;

			if (startDigits % 2 == 0) {
				startLeftHalf = start / startHalfPow;
				startRightHalf = start % startHalfPow;
			} else {
				startLeftHalf = startHalfPow;
				startRightHalf = startHalfPow;
			}

			if (endDigits % 2 == 0) {
				ll endHalfPow = pow(10, endDigits / 2);
				endLeftHalf = end / endHalfPow;
				endRightHalf = end % endHalfPow;
			} else {
				ll endHalfPowUneven = pow(10, endDigits / 2);
				endLeftHalf = endHalfPowUneven - 1;
				endRightHalf = endHalfPowUneven - 1;
			}

			ll firstValidHalf = 0;
			if (startRightHalf <= startLeftHalf) {
				firstValidHalf = startLeftHalf;
			} else {
				firstValidHalf = startLeftHalf + 1;
			}
			ll lastValidHalf = 0;
			if (endRightHalf >= endLeftHalf) {
				lastValidHalf = endLeftHalf;
			} else {
				lastValidHalf = endLeftHalf - 1;
			}

			ll result = 0;
			for (ll i = firstValidHalf; i <= lastValidHalf; i++) {
				int digits = digitCount(i);
				result += pow(10, digits) * i + i;
			}

			return result;
		}
	public:
		ll solve(vector<ll> ranges) {
			ll result = 0;
			for (int i = 1; i < ranges.size(); i += 2) {
				ll left = ranges[i - 1];
				ll right = ranges[i];
				result += solve(left, right);
			}
			return result;
		}
};

class PartTwo {
	private:
		int smallest_divisor(int number) {
			if (number % 2 == 0) return 2;
			for (int i = 3; i <= number; i += 2) {
				if (number % i == 0) {
					return i;
				}
			}
			return 0;
		}
		bool is_valid(ll number) {
			int digits = digitCount(number);
			if (digits == 1) return false;
			int smallest_div = smallest_divisor(digits);
			int largest_div = (digits > smallest_div) ? digits / smallest_div : smallest_div;
			for (int i = smallest_div; i <= largest_div; i++) {
				if (digits % i == 0 && digits > i) {
					ll power = pow(10, i);
					ll pattern = number % power;
					ll num = pattern;
					for (int j = 1; j < digits / i; j++) {
						num = num * power + pattern;
					}
					if (num == number) {
						return true;
					}
				}
			}
			ll pattern = number % 10;
			ll num = pattern;
			for (int i = 1; i < digits; ++i) {
				num = num * 10 + pattern;
			}
			return num == number;
		}
		ll solve(ll start, ll end) {
			ll result = 0;
			for (ll i = start; i <= end; ++i) {
				bool valid = is_valid(i);
				if (valid) {
					if (result > result + i) {
						cout << "start: " << start << ", end: " << end << endl;
					}
					result += i;
				}
			}
			return result;
		}
	public:
		ll solve(vector<ll> ranges) {
			ll result = 0;
			for (int i = 1; i < ranges.size(); i += 2) {
				ll left = ranges[i - 1];
				ll right = ranges[i];
				result += solve(left, right);
			}
			return result;
		}
};

int main(void) {
	int start = 0;
	int end = 0;

	string line;
	cin >> line;

	vector<ll> ids;
	while (end < line.size()) {
		if (line[end] == ',' || line[end] == '-' || end == line.size() - 1) {
			int substr_length = end == line.size() - 1 ? end - start + 1 : end - start;
			string substr = line.substr(start, substr_length);
			ll num = substr[0] - '0';
			for (int i = 1; i < substr.size(); ++i) {
				ll c = substr[i] - '0';
				num = num * 10 + c;
			}
			ids.push_back(num);
			start = end + 1;
		}
		end++;
	}

	PartOne program1;
	ll result1 = program1.solve(ids);
	cout << "Part One Result: " << result1 << endl;
	PartTwo program2;
	ll result2 = program2.solve(ids);
	cout << "Part Two Result: " << result2 << endl;
}