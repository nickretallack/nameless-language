type t = {
  pointers: PointerMap.t<PointerState.t>,
  error: option<string>,
  selection: Selection.t,
  scroll: Point.t,
  zoom: float,
}
