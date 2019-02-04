[%%debugger.chrome];
open Helpers;

let component = ReasonReact.statelessComponent("Connection");

let make =
    (
      ~sourcePosition: point,
      ~sinkPosition: point,
      ~nudge=0,
      ~maxNudge=1,
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
      stroke="black"
      strokeWidth="5"
      strokeOpacity=".1"
      d={Printf.sprintf(
        "M%f %f h %f v %f h %f",
        sourcePosition.x,
        sourcePosition.y,
        delta.x /. 2.0,
        delta.y,
        delta.x /. 2.0,
      )}
    />;
  },
};
