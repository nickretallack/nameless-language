let f = event =>
  Point.{
    x: float_of_int(ReactEvent.Pointer.pageX(event)),
    y: float_of_int(ReactEvent.Pointer.pageY(event)),
  };
