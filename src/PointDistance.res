let f = (point1: Point.t, point2: Point.t): float => {
	let vector = PointSubtract.f(point2, point1)
	Js.Math.hypot(vector.x, vector.y)
}