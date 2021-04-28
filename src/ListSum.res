let f = (numbers: list<float>): float =>
	Belt.List.reduce(numbers, 0.0, (sum, number) => sum +. number )