import sys
import pulp

def parse_buttons(unparsed_buttons):
    return [[int(str_num) for str_num in unparsed_button[1:-1].split(',')] for unparsed_button in unparsed_buttons]

def parse_joltages(unparsed_joltages):
    return [int(str_num) for str_num in unparsed_joltages[1:-1].split(',')]

def solve(buttons, joltages):
    M = len(joltages)
    N = len(buttons)

    A = [[0 for col in range(N)] for row in range(M)]
    B = [0 for idx in range(M)]
    for i, joltage in enumerate(joltages):
        for j, button in enumerate(buttons):
            for b in button:
                A[b][j] = 1
        B[i] = joltage

    C = [1 for idx in range(N)]

    prob = pulp.LpProblem("Joltage", pulp.LpMinimize)
    x = [pulp.LpVariable(f"x{j}", lowBound=0, cat="Integer") for j in range(N)]
    prob += pulp.lpSum(x)

    for i in range(M):
        prob += pulp.lpSum(A[i][j] * x[j] for j in range(N)) == B[i]

    prob.solve(pulp.PULP_CBC_CMD(msg=0))

    return int(pulp.value(prob.objective))

def solve_part_two(all_buttons, all_joltages):
    result = 0
    for (buttons, joltages) in zip(all_buttons, all_joltages):
        result += solve(buttons, joltages)

    return result

def main():
    lines = sys.stdin.readlines()

    all_buttons = []
    all_joltages = []

    for line in lines:
        parts = line.split()
        lights = parts[0][1:-1]
        unparsed_buttons = parts[1:-1]
        unparsed_joltages = parts[-1]

        buttons = parse_buttons(unparsed_buttons)
        joltages = parse_joltages(unparsed_joltages)

        all_buttons.append(buttons)
        all_joltages.append(joltages)

    result = solve_part_two(all_buttons, all_joltages)
    print(f"Part two result={result}")


if __name__ == "__main__":
    main()