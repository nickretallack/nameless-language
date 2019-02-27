[%%debugger.chrome];
open Helpers;

let component = ReasonReact.statelessComponent("Connection");

let curveConnect = (sourcePosition: point, sinkPosition: point, nudge: float) => {
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
  Printf.sprintf(
    "h %f a %f,%f 0 0,%i %f,%f v %f a %f,%f 0 0,%i %f,%f h %f",
    delta.x /. 2.0 -. curveSize.x *. direction.x +. nudge,
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
    delta.x /. 2.0 -. curveSize.x *. direction.x -. nudge,
  );
};

let make =
    (
      ~sourcePosition: point,
      ~sinkPosition: point,
      ~isSelected: bool=false,
      ~color: string,
      ~segments: list(float)=[],
      ~nodeWidth: float,
      ~xPadding: float,
      // ~nudge=0,
      // ~maxNudge=1,
      ~onClick=?,
      ~sourceIndex: int,
      _children,
    ) => {
  ...component,
  render: _self => {
    let nudge =
      5.0
      *. float_of_int(
           if (sourceIndex mod 2 == 0) {
             sourceIndex / 2;
           } else {
             - (sourceIndex - 1) / 2;
           },
         );

    let path =
      Printf.sprintf("M%f,%f ", sourcePosition.x, sourcePosition.y)
      ++ (
        if (Belt.List.length(segments) == 0) {
          curveConnect(sourcePosition, sinkPosition, nudge);
        } else {
          let (parts, lastPosition) =
            Belt.List.reduce(
              segments,
              ([], sourcePosition),
              ((acc, lastPosition), segmentY) =>
              (
                [
                  curveConnect(
                    lastPosition,
                    {x: lastPosition.x -. xPadding, y: segmentY +. nudge},
                    nudge,
                  )
                  ++ Printf.sprintf(" h %f ", -. nodeWidth),
                  ...acc,
                ],
                {
                  x: lastPosition.x -. xPadding -. nodeWidth,
                  y: segmentY +. nudge,
                },
              )
            );
          String.concat(
            "               ",
            Belt.List.reverse([
              curveConnect(lastPosition, sinkPosition, nudge),
              ...parts,
            ]),
          );
        }
      );

    <path
      fill="transparent"
      stroke={isSelected ? "red" : color}
      strokeWidth="5"
      pointerEvents="visibleStroke"
      d=path
      ?onClick
    />;
  },
};
