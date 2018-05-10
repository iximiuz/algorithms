def solve(heights):
    peaks = [(0, heights[0])]
    volume = 0
    for idx, h in enumerate(heights):
        if idx == 0:
            continue

        last_peak_h = peaks[-1][1]
        if h == last_peak_h:
            peaks[-1] = (idx, h)
            continue

        if h < last_peak_h:
            peaks.append((idx, h))
            continue

        if len(peaks) == 1:
            peaks[0] = (idx, h)
            continue

        peaks.pop()
        while peaks:
            peak_pos, peak_h = peaks[-1]
            volume += (idx - peak_pos - 1) * (min(h, peak_h) - last_peak_h)
            if peak_h > h:
                break
            last_peak_h = peak_h
            peaks.pop()

        if not peaks or h < peaks[-1][1]:
            peaks.append((idx, h))

    return volume


if __name__ == '__main__':
    print(solve([2, 0, 2]))  # 2
    print(solve([0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1]))  # 6
    print(solve([1, 3, 2, 4, 1, 3, 1, 4, 5, 2, 2, 1, 4, 2, 2]))  # 15

