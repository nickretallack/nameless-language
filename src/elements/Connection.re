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
    <path
      fill="transparent"
      stroke={isSelected ? "red" : "black"}
      strokeWidth="5"
      strokeOpacity={isSelected ? "0.5" : ".1"}
      d={Printf.sprintf(
        "M%f %f h %f v %f h %f",
        sourcePosition.x,
        sourcePosition.y,
        delta.x /. 2.0,
        delta.y,
        delta.x /. 2.0,
      )}
      ?onClick
    />;
  },
};
