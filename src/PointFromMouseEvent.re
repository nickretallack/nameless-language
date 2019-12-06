let f = event =>
  Point.{
    x: float_of_int(ReactEvent.Mouse.pageX(event)),
    y: float_of_int(ReactEvent.Mouse.pageY(event)),
  };
