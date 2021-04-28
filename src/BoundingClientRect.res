type t = {
  x: float,
  y: float,
  width: float,
  height: float,
  top: float,
  right: float,
  bottom: float,
  left: float,
}
@send external getBoundingClientRect: Dom.element => t = "getBoundingClientRect"
