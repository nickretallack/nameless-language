let f = event => {
  open Point
  {
    x: float_of_int(ReactEvent.Pointer.pageX(event)),
    y: float_of_int(ReactEvent.Pointer.pageY(event)),
  }
}
