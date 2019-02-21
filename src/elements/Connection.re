[%%debugger.chrome];
open Helpers;

let component = ReasonReact.statelessComponent("Connection");

let make =
    (
      ~sourcePosition: point,
      ~sinkPosition: point,
      ~isSelected: bool=false,
      ~nudge=0,
      ~maxNudge=1,
      ~onClick=?,
      _children,
    ) => {
  ...component,
  render: _self => {
    let delta = {
      x: sinkPosition.x -. sourcePosition.x,
      y: sinkPosition.y -. sourcePosition.y,
    };
    let radius = 15.0;
    let direction = {
      x: delta.x < 0.0 ? (-1.0) : 1.0,
      y: delta.y < 0.0 ? (-1.0) : 1.0,
    };
    let leftIsDownward = direction.y < 0.0;
    let curveSize = {
      x: min(abs_float(delta.x) /. 2.0, radius),
      y: min(abs_float(delta.y) /. 2.0, radius),
    };

    <path
      fill="transparent"
      stroke={isSelected ? "red" : "black"}
      strokeWidth="5"
      strokeOpacity={isSelected ? "0.5" : ".1"}
      d={Printf.sprintf(
        "M%f,%f h %f a %f,%f 0 0,%i %f,%f v %f a %f,%f 0 0,%i %f,%f h %f",
        sourcePosition.x,
        sourcePosition.y,
        delta.x /. 2.0 -. curveSize.x *. direction.x,
        curveSize.x,
        curveSize.y,
        leftIsDownward ? 1 : 0,
        curveSize.x *. direction.x,
        curveSize.y *. direction.y,
        delta.y -. curveSize.y *. 2.0 *. direction.y,
        curveSize.x,
        curveSize.y,
        leftIsDownward ? 0 : 1,
        curveSize.x *. direction.x,
        curveSize.y *. direction.y,
        delta.x /. 2.0 -. curveSize.x *. direction.x,
      )}
      ?onClick
    />;
  },
};
