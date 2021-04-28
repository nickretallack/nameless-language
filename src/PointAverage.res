let f = (points: list<Point.t>): Point.t =>
	{
		x: ListSum.f(Belt.List.map(points, point => point.x)) /. float(Belt.List.length(points)),
		y: ListSum.f(Belt.List.map(points, point => point.y)) /. float(Belt.List.length(points)),
	}