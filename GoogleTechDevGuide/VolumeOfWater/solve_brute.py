def solve(heights):
    left, right = 0, len(heights) - 1
    volume = 0
    stage = 0
    while True:
        while left < right and 0 >= heights[left] - stage:
            left += 1
        while right > left and 0 >= heights[right] - stage:
            right -= 1

        if left >= right:
            break

        for i in range(left, right + 1):
            volume += int(heights[i] - stage <= 0)

        stage += 1

    return volume


if __name__ == '__main__':
    print(solve([2, 0, 2]))  # 2
    print(solve([0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1]))  # 6
    print(solve([1, 3, 2, 4, 1, 3, 1, 4, 5, 2, 2, 1, 4, 2, 2]))  # 15

